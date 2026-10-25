// CD-ROM volume detection, the insert-disc prompt, and legacy Windows 95 media control.
// CdDrivePath remembers the last matching drive root; the VWIN32 handle belongs to the
// optional eject path. This file preserves the original control flow and MSVC6 codegen.
//
// VWIN32 forwards DeviceIoControl request 1 to DOS INT 21h. EAX=0x440d selects generic
// block-device IOCTL; CL selects the operation and CH the device category. Success needs
// both a nonzero Win32 result and a clear DOS carry flag, except for the status query
// in UnlockMedia, which also accepts DOS errors 0x01 and 0xb0.
// Protocol references:
// https://www.betaarchive.com/wiki/index.php/Microsoft_KB_Archive/168180
// https://www.delorie.com/djgpp/doc/rbinter/it/59/15.html
// https://www.delorie.com/djgpp/doc/rbinter/it/67/15.html

#include <windows.h>
#include "legoland.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "cdcheck.h"
#include "debug.h"
#include "globals.h"
#include "saveload.h"
#include "wndenv.h"

// Search the logical-drive mask for CD-ROM drives with CDFS and the expected volume label.
// A NULL expected_volume accepts any CDFS label. All drives are visited, so the last
// match wins: copy its root (for example, "D:\\") to CdDrivePath and return nonzero.
// FUNCTION: LEGOLAND 0x00450f30
int FindCdDriveWithVolume(char *expected_volume) {
    DWORD drive_mask;
    int found;
    DWORD drive_bit;
    int drive_index;
    char drive_root[4];
    char volume_label[256];
    char filesystem_name[256];
    DWORD serial_number;
    DWORD max_component_length;
    DWORD filesystem_flags;

    // STRING: LEGOLAND 0x004b8630
    strcpy(drive_root, "c:\\");
    found = 0;
    drive_mask = GetLogicalDrives();
    // STRING: LEGOLAND 0x004b8610
    DebugTrace("Checking all drives (Mask = %d)", drive_mask);
    Empty_0047f850();
    if (drive_mask != 0) {
        drive_bit = 1;
        drive_index = 0;
        do {
            if (drive_bit & drive_mask) {
                drive_root[0] = (char)('A' + drive_index);
                if (GetDriveTypeA(drive_root) == DRIVE_CDROM) {
                    // STRING: LEGOLAND 0x004b85f4
                    DebugTrace("Getting Info on drive %s", drive_root);
                    Empty_0047f850();
                    if (GetVolumeInformationA(drive_root, volume_label, 256, &serial_number, &max_component_length, &filesystem_flags, filesystem_name, 256)) {
                        // STRING: LEGOLAND 0x004b85ec
                        if (strcmp(filesystem_name, "CDFS") == 0) {
                            if (expected_volume == NULL || strcmp(volume_label, expected_volume) == 0) {
                                found = 1;
                                strcpy(CdDrivePath, drive_root);
                                // STRING: LEGOLAND 0x004b85c8
                                DebugTrace("Drive %s contains the correct CD", CdDrivePath);
                                Empty_0047f850();
                            }
                        }
                    }
                }
            }
            drive_index++;
            drive_bit <<= 1;
        } while (drive_index < 32);
    }

    return found;
}

// Check the previously remembered drive for CDFS and the optional expected label.
// Unlike FindCdDriveWithVolume, this neither scans other drives nor changes CdDrivePath.
// FUNCTION: LEGOLAND 0x004510e0
int IsCdVolumePresent(char *expected_volume) {
    int present;
    char drive_root[4];
    char volume_label[256];
    char filesystem_name[256];
    DWORD serial_number;
    DWORD max_component_length;
    DWORD filesystem_flags;

    strcpy(drive_root, "c:\\");
    present = 0;
    drive_root[0] = CdDrivePath[0];
    // The original calls toupper but discards its return value; keep that call for matching.
    toupper(CdDrivePath[0]);
    if (GetVolumeInformationA(drive_root, volume_label, 256, &serial_number, &max_component_length, &filesystem_flags, filesystem_name, 256)) {
        if (strcmp(filesystem_name, "CDFS") == 0) {
            if (expected_volume == NULL || strcmp(volume_label, expected_volume) == 0) {
                present = 1;
            }
        }
    }
    return present;
}

// Empty in the original (does nothing); nothing calls it.
// FUNCTION: LEGOLAND 0x004511e0
void Empty_004511e0(void) { return; }

// Empty in the original: EjectCdDrive calls it with the drive letter, which it ignores.
// FUNCTION: LEGOLAND 0x004511f0
void Empty_004511f0(int drive_letter) { return; }

// Empty in the original (does nothing); nothing calls it.
// FUNCTION: LEGOLAND 0x00451200
void Empty_00451200(void) { return; }

// Given a drive letter, attempt media unlock/ejection, release the logical-volume lock,
// close the persistent VWIN32 handle, and invalidate it. Individual failures are ignored.
// The empty drive-letter hook still runs when there is no open handle.
// FUNCTION: LEGOLAND 0x00451210
void EjectCdDrive(int drive_letter) {
    int drive_number;

    Empty_004511f0(drive_letter);
    if (VWin32DeviceHandle != INVALID_HANDLE_VALUE) {
        // Only the low byte is written/read by the original DOS-drive conversion.
        *(char *)&drive_number = (char)toupper((char)drive_letter) - 0x40;
        UnlockMedia(VWin32DeviceHandle, drive_number);
        EjectMedia(VWin32DeviceHandle, drive_number);
        UnlockLogicalVolume(VWin32DeviceHandle, drive_number);
        CloseDeviceHandle(VWin32DeviceHandle);
        VWin32DeviceHandle = INVALID_HANDLE_VALUE;
    }
}

// Remove pending media locks from a DOS drive number (1=A:, 2=B:, etc.; low byte only).
// The two-byte protocol block is [operation, pending-lock count]: 2 queries, 1 unlocks.
// The eight-byte scratch array preserves the original stack layout; only bytes 0/1 are used.
// Query failures with carry set and EAX=0x01/0xb0 are treated as success, as in KB Q168180.
// Each retry re-reads the returned count, and failure stops further unlock requests.
//
// Matching detail: preserve the Boolean result and the EAX-before-EBX assignment order.
// Together with the counted loop, these reproduce the shared failure tail in MSVC6.
// drive_number is dead after masking; the original uses its stack slot for bytes returned.
// FUNCTION: LEGOLAND 0x00451280
int UnlockMedia(HANDLE device, int drive_number) {
    DIOC_REGISTERS registers = {0};
    unsigned char unlock_params[8];
    int result;
    int lock_index;
    int dos_drive_number;

    // Start with an empty count, then ask the driver for its current lock status.
    unlock_params[1] = 0;
    dos_drive_number = drive_number & 0xff;
    registers.reg_EDX = (DWORD)(UINT_PTR)unlock_params; /* VWIN32 (Windows 9x) registers are 32 bits */
    unlock_params[0] = 2;
    registers.reg_EAX = 0x440d;
    registers.reg_EBX = dos_drive_number;
    registers.reg_ECX = 0x848;
    result = DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), (LPDWORD)&drive_number, NULL);
    if (result) {
        if (registers.reg_Flags & 1) {
            result = registers.reg_EAX == 0xb0 || registers.reg_EAX == 1;
        }
    }
    if (!result) {
        return 0;
    }
    // Operation 1 releases a single lock; keep the count returned by each request.
    for (lock_index = 0; lock_index < unlock_params[1]; lock_index++) {
        registers.reg_EDX = (DWORD)(UINT_PTR)unlock_params; /* VWIN32 (Windows 9x) registers are 32 bits */
        unlock_params[0] = 1;
        registers.reg_EAX = 0x440d;
        registers.reg_EBX = dos_drive_number;
        registers.reg_ECX = 0x848;
        result = DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), (LPDWORD)&drive_number, NULL);
        result = result && !(registers.reg_Flags & 1);
        if (!result) {
            break;
        }
    }
    return result;
}

// Add a media-removal lock: DOS generic IOCTL 0x0848, parameter operation 0.
// lock_params[0] is the operation; lock_params[1] receives the pending-lock count.
// The byte argument, separate DWORD output, and saved Boolean result matter for matching:
// MSVC6 reuses the dead argument slot for bytes_returned and retains the two byte clears.
// FUNCTION: LEGOLAND 0x00451390
int LockMedia(HANDLE device, unsigned char drive_number) {
    DIOC_REGISTERS registers = {0};
    char lock_params[2];
    DWORD bytes_returned;
    int result;

    lock_params[1] = 0;
    lock_params[0] = 0;
    registers.reg_EAX = 0x440d;
    registers.reg_EBX = drive_number;
    registers.reg_ECX = 0x848;
    registers.reg_EDX = (DWORD)(UINT_PTR)lock_params; /* VWIN32 (Windows 9x) registers are 32 bits */
    result = DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), &bytes_returned, NULL);
    return result && !(registers.reg_Flags & 1);
}

// Request hardware media ejection with DOS generic IOCTL 0x0849. This operation needs no
// parameter block and succeeds only when DeviceIoControl succeeds and DOS carry is clear.
// The masked drive_number is then dead and its slot is reused for the DWORD byte count.
// FUNCTION: LEGOLAND 0x00451410
int EjectMedia(HANDLE device, int drive_number) {
    DIOC_REGISTERS registers = {0};

    registers.reg_EBX = drive_number & 0xff;
    registers.reg_EAX = 0x440d;
    registers.reg_ECX = 0x849;
    if (DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), (LPDWORD)&drive_number, NULL) && !(registers.reg_Flags & 1)) {
        return 1;
    }
    return 0;
}

// Open the Windows 95 VWIN32 virtual device used to forward DOS IOCTL requests.
// The original uses zero access/share/disposition and FILE_FLAG_DELETE_ON_CLOSE (0x04000000).
// Return the actual HANDLE, including INVALID_HANDLE_VALUE on failure.
// FUNCTION: LEGOLAND 0x00451480
HANDLE OpenVWin32Device(void) {
    // STRING: LEGOLAND 0x004b8634
    HANDLE device = CreateFileA("\\\\.\\vwin32", 0, 0, NULL, 0, 0x4000000, NULL);
    return device;
}

// Close a VWIN32/device handle and return the Win32 CloseHandle result unchanged.
// FUNCTION: LEGOLAND 0x004514a0
BOOL __stdcall CloseDeviceHandle(HANDLE device) { return CloseHandle(device); }

// Take a logical-volume lock. BL is the DOS drive number, BH the lock level, and DX the
// low 16 bits of permissions. Permission bits allow writes (0), prohibit new mappings (1),
// or request a formatting lock (2). The requested lock level is separate from a media lock.
// Try FAT32 category 0x48 first, then legacy category 0x08 on failure; CL=0x4a requests locking.
// FUNCTION: LEGOLAND 0x004514b0
BOOL __stdcall LockLogicalVolume(HANDLE device, int drive_number, int lock_level, int permissions) {
    DIOC_REGISTERS registers = {0};
    unsigned char device_category = 0x48;
    BOOL result;
    DWORD bytes_returned;

    for (;;) {
        registers.reg_ECX = MAKEWORD(0x4a, device_category);
        registers.reg_EAX = 0x440d;
        registers.reg_EBX = MAKEWORD(drive_number, lock_level);
        registers.reg_EDX = permissions & 0xffff;
        if (DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), &bytes_returned, NULL) && !(registers.reg_Flags & 1)) {
            result = TRUE;
            break;
        }
        result = FALSE;
        // A failure in the legacy category ends the two-category fallback.
        if (device_category == 8) {
            break;
        }
        device_category = 8;
    }
    return result;
}

// Release one logical-volume lock using minor code 0x6a. As with LockLogicalVolume, retry
// category 0x08 after category 0x48 fails. The caller must balance successful lock requests.
// FUNCTION: LEGOLAND 0x00451550
BOOL __stdcall UnlockLogicalVolume(HANDLE device, int drive_number) {
    DIOC_REGISTERS registers = {0};
    unsigned char device_category = 0x48;
    BOOL result;
    DWORD bytes_returned;

    for (;;) {
        registers.reg_ECX = MAKEWORD(0x6a, device_category);
        registers.reg_EAX = 0x440d;
        registers.reg_EBX = drive_number & 0xff;
        if (DeviceIoControl(device, 1, &registers, sizeof(registers), &registers, sizeof(registers), &bytes_returned, NULL) && !(registers.reg_Flags & 1)) {
            result = TRUE;
            break;
        }
        result = FALSE;
        // A failure in the legacy category ends the two-category fallback.
        if (device_category == 8) {
            break;
        }
        device_category = 8;
    }
    return result;
}

// Wait for a LEGOLAND-labelled CD, returning 1 on success or 0 when the user cancels.
// A nonzero rescan_drives searches all drives; zero checks only the remembered CdDrivePath.
// Minimize the game once while prompting and restore it on both success and cancellation.
// FUNCTION: LEGOLAND 0x004515e0
int WaitForLegolandCd(int rescan_drives) {
    char prompt_text[256];
    int game_minimised;

    game_minimised = 0;
    if (rescan_drives != 0) {
        // STRING: LEGOLAND 0x004b86d0
        while (!FindCdDriveWithVolume("LEGOLAND")) {
            if (!game_minimised) {
                // STRING: LEGOLAND 0x004b86c0
                DebugTrace("Minimising Game");
                Empty_0047f850();
                MinimizeGameWindow();
                game_minimised = 1;
            }
            if (MessageBoxA(WNDENV_Gethwnd(),
                    // STRING: LEGOLAND 0x004b8680
                    "Please insert the LEGOLAND CD-ROM into the CD drive",
                    // STRING: LEGOLAND 0x004b86b4
                    "CD Missing", 0x50015) == IDCANCEL) {
                // STRING: LEGOLAND 0x004b8670
                DebugTrace("Maximising Game");
                Empty_0047f850();
                RestoreGameWindow();
                return 0;
            }
        }
        if (game_minimised) {
            DebugTrace("Maximising Game");
            Empty_0047f850();
            RestoreGameWindow();
        }
        return 1;
    } else {
        while (!IsCdVolumePresent("LEGOLAND")) {
            if (!game_minimised) {
                DebugTrace("Minimising Game");
                Empty_0047f850();
                MinimizeGameWindow();
                game_minimised = 1;
            }
            // STRING: LEGOLAND 0x004b8640
            sprintf(prompt_text, "Please insert the LEGOLAND CD-ROM into drive %s", CdDrivePath);
            if (MessageBoxA(WNDENV_Gethwnd(), prompt_text, "CD Missing", 0x50015) == IDCANCEL) {
                DebugTrace("Maximising Game");
                Empty_0047f850();
                RestoreGameWindow();
                return 0;
            }
        }
        if (game_minimised) {
            DebugTrace("Maximising Game");
            Empty_0047f850();
            RestoreGameWindow();
        }
        return 1;
    }
}
