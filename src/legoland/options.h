#pragma once

#include "legoland.h"

struct EditState;
struct IconNode;
struct ScreenMode;

LEGO_EXPORT void InitExitCheckBox(unsigned int param_1, unsigned int param_2);
LEGO_EXPORT void KillTitleScreenSprites(void);
unsigned char ExitCheckBoxOkClicked(unsigned int param_1, unsigned int param_2);
unsigned char OptionsAcceptIconHandler(unsigned int param_1, unsigned int param_2);
unsigned char OptionsSaveIconHandler(unsigned int param_1, unsigned int param_2);
unsigned char OptionsExitIconHandler(unsigned int param_1, unsigned int param_2);
unsigned char ExitLevelOkClicked(unsigned int param_1, unsigned int param_2);
unsigned char ExitCheckBoxCloseClicked(unsigned int param_1, unsigned int param_2);
unsigned char OptionsLoadIconHandler(unsigned int param_1, unsigned int param_2);
unsigned char SaveGameScreenSaveHandler(unsigned int param_1, unsigned int param_2);
unsigned char OptionsUpIconHandler(struct IconNode *, unsigned int);
unsigned char OptionsDownIconHandler(struct IconNode *, unsigned int);
unsigned char OptionsVolumeMarkerIconHandler(struct IconNode *, unsigned int);
void SaveScreenState(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3);
unsigned int RestoreScreenState(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3);
unsigned char ReturnToTitleClicked(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
int IsFreePlayUnlocked(void);
LEGO_EXPORT void InitOptionScreen(void);
LEGO_EXPORT void PrintExitCheckBox(void);
LEGO_EXPORT void RenderScreen(void);
void PlayPendingFrontEndSpeech(void);
