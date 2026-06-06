#pragma once

int Goto_ProfileDir(void);
int ReturnFrom_ProfileDir(void);
void LoadProfilesFormDisk(void);
void UpDateCurrentSaveSlotInfo(void);
void UpDateCurrentProfile(void);
void SaveProfileToDisk(void);
void RemoveProfile(unsigned char index);
void DeleteProfileList(void);
