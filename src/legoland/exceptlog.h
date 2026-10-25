#pragma once

#include <windows.h>

int stackdump(void *exc_info, const char *filename);
void WriteModuleInfo(HANDLE file, UINT_PTR base);
void FormatLocalFileTime(char *buffer, FILETIME ft);
void WriteFileFormatted(HANDLE file, const char *format, ...);
void WriteModuleList(HANDLE file);
void WriteErrorTimeAndSystemInfo(HANDLE file);
const char *GetExceptionDescription(unsigned int code);
char *GetFileNameFromPath(char *path);
