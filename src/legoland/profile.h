#pragma once

#include "legoland.h"

struct IconNode;
struct Sprite;

LEGO_EXPORT void CloseFontEndCheckBox(void);
LEGO_EXPORT void InitListProfiles(void);
LEGO_EXPORT void PrintProfileDetails(void);
LEGO_EXPORT void EnterNewProfileCheckBoxIcons(struct IconNode *param_1);
LEGO_EXPORT void InitProfileCheckBoxIcons(struct IconNode *param_1);
LEGO_EXPORT void LightUpthisDeleteIcon(struct IconNode *icon, int param_2);
LEGO_EXPORT void UpdateProfileCheckBoxIcons(void);
struct Sprite *GetProfileOffSprite(signed char param_1);
void InitDeletePopUp(struct IconNode *param_1);
unsigned char DeleteIconHandler(void *param_1, unsigned int param_2);
unsigned char AcceptProfileClick(unsigned int dummy, unsigned char arg_0);
unsigned char ProfileSelectHandler(struct IconNode *profile, unsigned char param_2);
unsigned char SelectEmptyProfileSlotClick(struct IconNode *profile, unsigned int param_2);
unsigned char ConfirmDeleteProfileClick(unsigned int arg0, unsigned int arg1);
unsigned char DeletePopUpCancelHandler(unsigned int param_1, unsigned int param_2);
void SaveProfileKeyHandlers(void);
void RestoreProfileKeyHandlers(void);
void LoadCurrentProfileFromList(void);
LEGO_EXPORT void KillListProfileSprite(void);
