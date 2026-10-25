#pragma once
#include <windows.h>

// Register packet for VWIN32 DeviceIoControl request 1 (DOS INT 21h IOCTL).
// The same 28-byte packet is input/output; reg_Flags bit 0 is DOS's carry/error flag.
// Media requests use EAX=0x440d; ECX combines the category in CH and operation in CL.
typedef struct DIOC_REGISTERS {
    /* 0x00 */ DWORD reg_EBX; // BL: drive number; BH: logical-volume lock level when needed.
    /* 0x04 */ DWORD reg_EDX; // Parameter-block address, or logical-volume permissions in DX.
    /* 0x08 */ DWORD reg_ECX; // Device category and IOCTL minor code.
    /* 0x0c */ DWORD reg_EAX; // DOS function on input; DOS error code on failure.
    /* 0x10 */ DWORD reg_EDI; // Unused by these helpers; zero-initialized.
    /* 0x14 */ DWORD reg_ESI; // Unused by these helpers; zero-initialized.
    /* 0x18 */ DWORD reg_Flags; // Carry set means the forwarded DOS operation failed.
} DIOC_REGISTERS;

// Locate/verify CDFS volume labels. A NULL expected_volume accepts any CDFS label.
int FindCdDriveWithVolume(char *expected_volume);
int IsCdVolumePresent(char *expected_volume);

// Original no-op entry points; the empty bodies do not establish their semantic names.
void Empty_004511e0(void);
void Empty_004511f0(int drive_letter);
void Empty_00451200(void);

// Eject by drive letter through the persistent VWin32DeviceHandle, then close the handle.
void EjectCdDrive(int drive_letter);

// Low-level helpers take a DOS drive number, not a character: 1=A:, 2=B:, etc.
// Their nonzero result requires both a successful Win32 call and an accepted DOS status.
int UnlockMedia(HANDLE device, int drive_number);
int LockMedia(HANDLE device, unsigned char drive_number);
int EjectMedia(HANDLE device, int drive_number);
HANDLE OpenVWin32Device(void);
BOOL __stdcall CloseDeviceHandle(HANDLE device);
BOOL __stdcall LockLogicalVolume(HANDLE device, int drive_number, int lock_level, int permissions);
BOOL __stdcall UnlockLogicalVolume(HANDLE device, int drive_number);

// Prompt until the CD is found or cancelled; nonzero rescan_drives searches every drive.
int WaitForLegolandCd(int rescan_drives);
