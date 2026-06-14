#pragma once

#include "legoland.h"

struct ResFile;
struct ResVolume;

LEGO_EXPORT struct ResVolume *RES_OpenVolume(const char *path);
LEGO_EXPORT unsigned int RES_CloseVolume(struct ResVolume *volume);
LEGO_EXPORT struct ResFile *RES_OpenFile(const char *path);
LEGO_EXPORT unsigned int RES_GetFileSize(struct ResFile *file);
LEGO_EXPORT int RES_ReadFile(struct ResFile *file, void *buffer, int count);
LEGO_EXPORT int RES_SetFilePointer(struct ResFile *file, int offset);
LEGO_EXPORT int RES_GetFilePointer(struct ResFile *file);
LEGO_EXPORT int RES_CloseFile(struct ResFile *file);
