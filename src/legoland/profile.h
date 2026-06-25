#pragma once

#include "legoland.h"

struct IconNode;
struct Sprite;

LEGO_EXPORT void CloseFontEndCheckBox(void);
LEGO_EXPORT void InitListProfiles(void);
LEGO_EXPORT void PrintProfileDetails(void);
LEGO_EXPORT void EnterNewProfileCheckBoxIcons(struct IconNode *param_1);
LEGO_EXPORT void InitProfileCheckBoxIcons(struct IconNode *param_1);
LEGO_EXPORT void UpdateProfileCheckBoxIcons(void);
struct Sprite *FUN_0048c5e0(signed char param_1);
void FUN_0048c860(struct IconNode *param_1);
unsigned char FUN_0048cc30(void *param_1, unsigned int param_2);
unsigned char FUN_0048d400(unsigned int arg0, unsigned int arg1);
unsigned char FUN_0048d450(unsigned int param_1, unsigned int param_2);
void FUN_0048d470(void);
void FUN_0048d490(void);
