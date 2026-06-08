#pragma once

#include "legoland.h"

LEGO_EXPORT int Goto_ProfileDir(void);
LEGO_EXPORT int ReturnFrom_ProfileDir(void);
LEGO_EXPORT char LoadProfilesFormDisk(void);
LEGO_EXPORT void AddNodeToProfileList(int load, char *data, char slot);
LEGO_EXPORT void UpDateCurrentSaveSlotInfo(void);
LEGO_EXPORT void UpDateCurrentProfile(void);
LEGO_EXPORT char SaveProfileToDisk(void);
LEGO_EXPORT int RemoveProfile(unsigned char index);
LEGO_EXPORT void DeleteProfileList(void);
