#pragma once

#include "legoland.h"

#include <windows.h>

void FlushTextCells(int param_1);
void DrawTextOnRenderSurface(char *text, int font, RECT rc, COLORREF color);
LEGO_EXPORT void PrintCent(int cx, int y, int width, const char *text, int font);
LEGO_EXPORT void BubbleHelp(int *rect, char *text, int font);
LEGO_EXPORT void HTBubbleHelp(RECT *rect, char *text, int font);
void HTBubbleHelpWithMood(RECT *rect, const char *text, int font, int mood_icon);
LEGO_EXPORT HGDIOBJ SelectFont(HDC hdc, int font_id);

struct TextCell;
struct TextCell *CreateTextCell(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
struct TextCell *FindTextCellByStyle(const char *name, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
void PrintTextBox(char *name, unsigned int x, unsigned int y, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
void PrintTextCell(struct TextCell *cell, unsigned int x, unsigned int y);
LEGO_EXPORT void Print(int x, int y, const char *text, int font);
LEGO_EXPORT void PrintLimitedText(int x, int y, int width, const char *text, int font, COLORREF color, UINT format);
int MeasureTextHeight(const char *text, int font, int width);
void DrawWrappedTextAt(int x, int y, const char *text, int font, int width);
void UnloadBubbleHelpGFX(void);
LEGO_EXPORT void LoadBubbleHelpGFX(void);
