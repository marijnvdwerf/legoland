#pragma once

#include "globals.h"
#include "legoland.h"

#include <windows.h>

LEGO_EXPORT int Goto_ProfileDir(void);
LEGO_EXPORT int ReturnFrom_ProfileDir(void);
LEGO_EXPORT char LoadProfilesFormDisk(void);
LEGO_EXPORT void AddNodeToProfileList(int load, struct ProfileData *data, char slot);
LEGO_EXPORT char UpDateCurrentSaveSlotInfo(void);
LEGO_EXPORT char UpDateCurrentProfile(void);
LEGO_EXPORT char SaveProfileToDisk(void);
LEGO_EXPORT int RemoveProfile(unsigned char index);
LEGO_EXPORT void DeleteProfileList(void);
LEGO_EXPORT void NewPrintCent(char *text, int font, RECT rc, int color_flag);
int FUN_00491e40(char *text, int font, RECT rc, int color_flag);

struct ProfileNode {
    /* 0x00 */ struct ProfileNode *next;
    /* 0x04 */ struct ProfileData data;
    /* 0x114 */ int has_header;
    /* 0x118 */ unsigned char slot;
};

struct ProfileNode *FUN_004919a0(unsigned char slot);
char FUN_004920a0(unsigned int param1, unsigned char flags);
int FUN_00492130(void *hwnd);
LEGO_EXPORT void ResetTempProfile(void);
