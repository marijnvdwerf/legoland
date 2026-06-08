#pragma once

#include "legoland.h"

struct LLS {
    short frame;
    unsigned short delay;
    unsigned char pad_4[0x10 - 0x4];
    short frame_count;
    short loop_delay;
    unsigned int flags;
};

struct LLSImage;
struct ILFTable;

LEGO_EXPORT void LLS555To565(struct LLSImage *param_1);

LEGO_EXPORT void LLIDB_FreeILFTable(struct ILFTable *table);

LEGO_EXPORT int LLIDB_FindElement(const char *name, unsigned int *out, int zero);
LEGO_EXPORT unsigned int ElemID(const char *name);
LEGO_EXPORT void LLIDB_LoadICM(void);
LEGO_EXPORT void LLIDB_CloseICM(void);
LEGO_EXPORT unsigned int LLIDB_RegisterNewElement(const char *param_1, unsigned int param_2, unsigned int param_3);
LEGO_EXPORT void LLIDB_ClearOnLevel(void);
LEGO_EXPORT void *LLIDB_LoadData(void *head);
LEGO_EXPORT void LLIDB_UnLoadData(unsigned int handle);

LEGO_EXPORT void LLSStop(unsigned int handle);
LEGO_EXPORT void LLSSetFrame(struct LLS *lls, int index);

LEGO_EXPORT unsigned int SaveGameRead(void *buffer, unsigned int count);
LEGO_EXPORT unsigned int SaveGameWrite(void *buffer, unsigned int count);
