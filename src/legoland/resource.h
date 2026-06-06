#pragma once

struct ResFile;

struct ResFile *RES_OpenFile(const char *path);
unsigned int RES_GetFileSize(struct ResFile *file);
int RES_ReadFile(struct ResFile *file, void *buffer, int count);
int RES_CloseFile(struct ResFile *file);
