#pragma once

#include <windows.h>
#include "legoland.h"

int __cdecl wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void DebugTrace(const char *fmt, ...);
int GameMain(void);
LEGO_EXPORT unsigned int mystrlen(const char *s);
