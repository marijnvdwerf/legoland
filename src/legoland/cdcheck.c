#include "legoland.h"
#include <windows.h>

#include "globals.h"
#include <string.h>
#include "debug.h"
#include "saveload.h"

// FUNCTION: LEGOLAND 0x00450f30
int FUN_00450f30(char *cd_volume) {
    int result;
    DWORD drives;
    DWORD bit;
    int i;
    char root_path[4];
    char volume_name[256];
    char fs_name[256];
    DWORD serial_number;
    DWORD max_component_length;
    DWORD fs_flags;

    // STRING: LEGOLAND 0x004b8630
    strcpy(root_path, "c:\\");
    result = 0;
    drives = GetLogicalDrives();
    // STRING: LEGOLAND 0x004b8610
    FUN_0047f870("Checking all drives (Mask = %d)", drives);
    FUN_0047f850();
    if (drives == 0) {
        return 0;
    }

    bit = 1;
    i = 0;
    do {
        if (bit & drives) {
            root_path[0] = (char)('A' + i);
            if (GetDriveTypeA(root_path) == DRIVE_CDROM) {
                // STRING: LEGOLAND 0x004b85f4
                FUN_0047f870("Getting Info on drive %s", root_path);
                FUN_0047f850();
                if (GetVolumeInformationA(root_path, volume_name, 256, &serial_number, &max_component_length, &fs_flags, fs_name, 256)) {
                    // STRING: LEGOLAND 0x004b85ec
                    if (strcmp(fs_name, "CDFS") == 0) {
                        if (cd_volume == NULL || strcmp(volume_name, cd_volume) == 0) {
                            strcpy(DAT_00813b04, root_path);
                            // STRING: LEGOLAND 0x004b85c8
                            FUN_0047f870("Drive %s contains the correct CD", DAT_00813b04);
                            FUN_0047f850();
                            result = 1;
                        }
                    }
                }
            }
        }
        i++;
        bit <<= 1;
    } while (i < 32);

    return result;
}

// FUNCTION: LEGOLAND 0x004510e0
void FUN_004510e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004511e0
void FUN_004511e0(void) { return; }

// FUNCTION: LEGOLAND 0x004511f0
void FUN_004511f0(void) { return; }

// FUNCTION: LEGOLAND 0x00451200
void FUN_00451200(void) { return; }

// FUNCTION: LEGOLAND 0x00451210
void FUN_00451210(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451280
void FUN_00451280(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451390
void FUN_00451390(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451410
void FUN_00451410(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451480
int FUN_00451480(void) {
    // STRING: LEGOLAND 0x004b8634
    HANDLE result = CreateFileA("\\\\.\\vwin32", 0, 0, NULL, 0, 0x4000000, NULL);
    return (int)result;
}

// FUNCTION: LEGOLAND 0x004514a0
BOOL __stdcall FUN_004514a0(HANDLE param_1) { return CloseHandle(param_1); }

// FUNCTION: LEGOLAND 0x004514b0
void FUN_004514b0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00451550
void FUN_00451550(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004515e0
int FUN_004515e0(int param_1) { STUB(); }
