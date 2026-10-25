#pragma once

#include "legoland.h"

#include <windows.h>

struct IconNode;

LEGO_EXPORT void InitTitleScreen(void);
unsigned char TitleLoadGameHandler(unsigned int param_1, unsigned char param_2);
unsigned char TitleNewGameHandler(unsigned int param_1, unsigned int param_2);
unsigned char TitleFreePlayHandler(unsigned int param_1, unsigned int param_2);
unsigned char TitleRegistrationHandler(unsigned int param_1, unsigned int param_2);
unsigned char TitleMovieHandler(unsigned int param_1, unsigned int param_2);
struct Sprite *InitCertificateScreen(void);
void InitAdvertScreen(void);
void RenderCertificateScreen(void);
void InitIntervalScreen(void);
void RenderIntervalScreen(void);
void SetIntervalScreenIsBriefing(unsigned int param_1);
unsigned int LoadIntervalReport(const char *param_1);
void SetPendingMovie(const char *param_1);
void FreeIntervalReport(void);
void FreeIntervalHints(void);
void DrawClippedText(char *text, int font, RECT rc, INT_PTR color_flag);
unsigned int ShowIntervalScreen(const char *a, const char *b);
unsigned int HasIntervalReport(const char *param_1);
LEGO_EXPORT void InitNewProfilePoPUp(struct IconNode *profile);
