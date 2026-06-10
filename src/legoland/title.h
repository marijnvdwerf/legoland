#pragma once

#include "legoland.h"

#include <windows.h>

struct Profile;

LEGO_EXPORT void InitTitleScreen(void);
unsigned char FUN_0048fe20(unsigned int param_1, unsigned char param_2);
unsigned char FUN_0048feb0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048ff20(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048ff70(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048ffe0(unsigned int param_1, unsigned int param_2);
unsigned int FUN_00490350(void);
void FUN_00490150(void);
void FUN_00490410(void);
void FUN_00490c70(void);
void FUN_004910f0(void);
void FUN_00490600(unsigned int param_1);
void FUN_00490610(const char *param_1);
void FUN_00490850(void);
void FUN_00490880(void);
void FUN_00490fa0(char *text, int font, RECT rc, int color_flag);
unsigned int FUN_004911c0(const char *a, const char *b);
LEGO_EXPORT void InitNewProfilePoPUp(struct Profile *profile);
