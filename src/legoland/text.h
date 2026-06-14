#pragma once

#include "legoland.h"

#include <windows.h>

void FUN_00455f70(int param_1);
LEGO_EXPORT void PrintCent(int cx, int y, int width, const char *text, int font);
LEGO_EXPORT void BubbleHelp(int *rect, char *text, int font);
LEGO_EXPORT HGDIOBJ SelectFont(HDC hdc, int font_id);

struct TextCell;
struct TextCell *FUN_00455bb0(char *name, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
struct TextCell *FUN_00455d40(char *name, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
void FUN_00455e50(char *name, unsigned int x, unsigned int y, int width, int height, int font, unsigned int format, unsigned int bg_color, unsigned int text_color);
void FUN_00455ec0(struct TextCell *cell, unsigned int x, unsigned int y);
