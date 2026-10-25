#pragma once

struct Sample;

#include "globals.h"
#include "legoland.h"

#include <windows.h>

LEGO_EXPORT int Goto_ProfileDir(void);
LEGO_EXPORT int ReturnFrom_ProfileDir(void);
LEGO_EXPORT char LoadProfilesFormDisk(void);
struct ProfileData;
LEGO_EXPORT void AddNodeToProfileList(int load, struct ProfileData *data, char slot);
LEGO_EXPORT char UpDateCurrentSaveSlotInfo(void);
LEGO_EXPORT char UpDateCurrentProfile(void);
LEGO_EXPORT char SaveProfileToDisk(void);
LEGO_EXPORT int RemoveProfile(unsigned char index);
LEGO_EXPORT void DeleteProfileList(void);
LEGO_EXPORT void NewPrintCent(char *text, int font, RECT rc, INT_PTR color_flag);
int DrawCenteredTextAndGetRightEdge(char *text, int font, RECT rc, INT_PTR color_flag);

struct IconNode;
LEGO_EXPORT void EnterNewProfile(struct IconNode *sprite);
unsigned int TempProfileHasName(void);

struct ProfileNode {
    /* 0x00 */ struct ProfileNode *next;
    /* 0x04 */ struct ProfileData data;
    /* 0x114 */ int has_header;
    /* 0x118 */ unsigned char slot;
};

struct ProfileNode *FindProfileNodeBySlot(unsigned char slot);
char CancelProfileSelectionClicked(unsigned int param1, unsigned char flags);
int InitDirectSound(void *hwnd);
LEGO_EXPORT void ResetTempProfile(void);
void *NewSample(void);
struct Sample *AllocSample(void);
