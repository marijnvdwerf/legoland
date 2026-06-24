#pragma once

#include "legoland.h"

struct EditState;
struct IconNode;
struct ScreenMode;

LEGO_EXPORT void InitExitCheckBox(unsigned int param_1, unsigned int param_2);
LEGO_EXPORT void KillTitleScreenSprites(void);
unsigned char FUN_0048ef90(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048efd0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f050(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f0a0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f440(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f4b0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f550(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f5a0(unsigned int param_1, unsigned int param_2);
unsigned char FUN_0048f5f0(struct IconNode *, unsigned int);
unsigned char FUN_0048f760(struct IconNode *, unsigned int);
unsigned char FUN_0048f8d0(struct IconNode *, unsigned int);
void FUN_0048f9f0(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3);
unsigned int FUN_0048fa40(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3);
unsigned char FUN_0048fb80(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
int FUN_0048fc30(void);
LEGO_EXPORT void InitOptionScreen(void);
LEGO_EXPORT void PrintExitCheckBox(void);
LEGO_EXPORT void RenderScreen(void);
void FUN_0048fc00(void);
