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

/* struct Element's full layout and the page/slot id encoding are private to
 * llidb.c (the owning TU). Other TUs only pass struct Element * around. */
struct Element;

#define LLIDB_FLAG_LEVEL 0x4
#define LLIDB_FLAG_LOADED 0x1
#define LLIDB_FLAG_USEDLL 0x10000
#define LLIDB_FLAG_NODATA 0x40000

#define LLIDB_TYPE_MASK 0xfff0
#define LLIDB_TYPE_ODF 0x10
#define LLIDB_TYPE_TSM 0x20
#define LLIDB_TYPE_TSF 0x40
#define LLIDB_TYPE_NOFILE 0x200
#define LLIDB_TYPE_TXT 0x800
#define LLIDB_TYPE_ILF 0x400
#define LLIDB_TYPE_ODF_DLL 0x1010
#define LLIDB_TYPE_CSP 0x2000

#define LLIDB_ERR_MISMATCH (-1)
#define LLIDB_ERR_ICMWRITE (-2)
#define LLIDB_ERR_NOTFOUND (-3)
#define LLIDB_ERR_NOKEY (-4)
#define LLIDB_ERR_NOFILE (-5)
#define LLIDB_ERR_CANCELED (-6)

struct LLSImage;
struct ILFTable;
struct LLIDBHead;

LEGO_EXPORT void LLS555To565(struct LLSImage *param_1);

void FUN_0047c6a0(struct LLIDBHead *head);

LEGO_EXPORT void LLIDB_FreeILFTable(struct ILFTable *table);

LEGO_EXPORT unsigned int LLIDB_GetCount(void);
LEGO_EXPORT int LLIDB_GetElement(unsigned int index, struct Element **output);
LEGO_EXPORT int LLIDB_FindElement(const char *name, unsigned int *out, unsigned int *index_out);
LEGO_EXPORT int LLIDB_FindElementFromDataPtr(void *data, unsigned int *out, unsigned int *index_out);
LEGO_EXPORT unsigned int ElemID(const char *name);
LEGO_EXPORT void LLIDB_LoadICM(void);
LEGO_EXPORT void LLIDB_CloseICM(void);
LEGO_EXPORT unsigned int LLIDB_RegisterNewElement(const char *param_1, const char *param_2, unsigned int param_3);
LEGO_EXPORT void LLIDB_ClearOnLevel(void);
LEGO_EXPORT void *LLIDB_LoadData(void *head);
LEGO_EXPORT void LLIDB_UnLoadData(unsigned int handle);

LEGO_EXPORT void LLSAuto(void);
void FUN_0047d610(struct LLS *param_1);
LEGO_EXPORT int LLSStop(unsigned int handle);
LEGO_EXPORT void LLSPlay(struct LLS *param_1, unsigned int param_2);
LEGO_EXPORT void LLSPlayOnce(struct LLS *param_1, unsigned int param_2);
LEGO_EXPORT void LLSSetFrame(struct LLS *lls, int index);
LEGO_EXPORT void LLSNextFrame(struct LLS *param_1);

LEGO_EXPORT unsigned int SaveGameRead(void *buffer, unsigned int count);
LEGO_EXPORT unsigned int SaveGameWrite(void *buffer, unsigned int count);
