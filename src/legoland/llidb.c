#include "legoland.h"

#include "llidb.h"
#include "tilemap.h"
#include "globals.h"

extern void FUN_0049e4d0(void *block);
#include "image_sprite.h"
extern unsigned int FUN_0049f4ca(int fd, void *buffer, unsigned int count);
extern int FUN_004a63e4(int fd, void *buffer, unsigned int count);

struct ILFTable {
    unsigned char pad_0[4];
    int count;
    unsigned int *sprites;
    void *data_c;
    void *data_10;
};

struct SpriteManager {
    unsigned short var_0;
    unsigned char pad_2[0x4 - 0x2];
    int count;
    unsigned int *sprites;
    void *data_c;
    void *data_10;
    void *data_14;
};

struct LLSImage {
    unsigned char pad_0[0xc];
    unsigned int format;
    short height;
    unsigned char pad_12[0x2];
    unsigned char flags;
};

// FUNCTION: LEGOLAND 0x0047aff0
void LLIDB_LoadICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b2d0
unsigned int LLIDB_GetCount(void) {
    return DAT_006691a4;
}

// FUNCTION: LEGOLAND 0x0047b2e0
int LLIDB_GetElement(unsigned int index, unsigned int *output) {
    if (index < DAT_006691a4) {
        if (output != NULL) {
            *output = (unsigned int)DAT_006691a8[index >> 8] + (index & 0xff) * 20;
        }
        return 0;
    }
    if (output != NULL) {
        *output = 0;
    }
    return -3;
}

// FUNCTION: LEGOLAND 0x0047b330
int LLIDB_FindElement(const char *name, unsigned int *out, int zero) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b3f0
unsigned int ElemID(const char *name) {
    LLIDB_FindElement(name, (unsigned int *)&name, 0);
    return (unsigned int)name;
}

// FUNCTION: LEGOLAND 0x0047b410
void LLIDB_FindElementFromDataPtr(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b4c0
void LLIDB_ClearOnLevel(void) {
    unsigned int i = 0;

    if (DAT_006691a4 > 0) {
        do {
            DAT_006691a8[i >> 8][(i & 0xff) * 5 + 2] &= 0xfffffffb;
            i++;
        } while (i < DAT_006691a4);
    }
}

// FUNCTION: LEGOLAND 0x0047b500
void FUN_0047b500(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b5a0
void FUN_0047b5a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b610
unsigned int LLIDB_RegisterNewElement(unsigned int param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b7b0
void FUN_0047b7b0(unsigned int param_1) { STUB(); }

// FUNCTION: LEGOLAND 0x0047b860
unsigned int LLIDB_RegisterNewElementB(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    unsigned int result = LLIDB_RegisterNewElement(param_1, param_2, param_3);
    if (result != 0) {
        FUN_0047b7b0(result);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x0047b890
void FUN_0047b890(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047bc20
void LLIDB_SelectElement(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047bc80
void LLIDB_SaveICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047be00
void LLIDB_CloseICM(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047bef0
void LLIDB_FreeILFTable(struct ILFTable *table) {
    if (table == NULL) {
        return;
    }

    if (table->data_c != NULL) {
        FUN_0049e4d0(table->data_c);
    }
    if (table->data_10 != NULL) {
        FUN_0049e4d0(table->data_10);
    }

    if (table->sprites != NULL) {
        int i = 0;
        if (table->count > 0) {
            do {
                if (table->sprites[i] != 0) {
                    KillSprite(table->sprites[i]);
                    table->sprites[i] = 0;
                }
                i++;
            } while (i < table->count);
        }
        FUN_0049e4d0(table->sprites);
    }

    FUN_0049e4d0(table);
}

// FUNCTION: LEGOLAND 0x0047bf70
void LLIDB_LoadODFData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047c6a0
void FUN_0047c6a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047c7f0
void FUN_0047c7f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047cba0
void LLIDB_LoadTSFData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047cdd0
void FUN_0047cdd0(struct SpriteManager *param_1) {
    struct SpriteManager *si = (struct SpriteManager *)param_1->data_c;
    int i;

    FUN_0049e4d0(si->data_c);
    FUN_0049e4d0(si->data_10);

    i = 0;
    if (si->count > 0) {
        do {
            KillSprite(si->sprites[i]);
            i++;
        } while (i < si->count);
    }

    FreeTileSpace(si->var_0, si->count);
    if (si->data_14 != NULL) {
        LLIDB_UnLoadData((unsigned int)si->data_14);
    }

    FUN_0049e4d0(si);
}

// FUNCTION: LEGOLAND 0x0047ce40
void LLIDB_LoadTSMData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047cf80
void FUN_0047cf80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047cfc0
void LLIDB_LoadILFData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d1a0
void LLIDB_LoadCSPData(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d3a0
void *LLIDB_LoadData(void *head) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d450
void LLIDB_UnLoadData(unsigned int handle) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d4c0
void LLSStop(unsigned int handle) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d520
void LLSPlay(struct LLS *param_1, unsigned int param_2) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d580
void LLSPlayOnce(struct LLS *param_1, unsigned int param_2) {
    LLSPlay(param_1, param_2);
    param_1->flags |= 0x4;
}

// FUNCTION: LEGOLAND 0x0047d5a0
void LLSSetFrame(struct LLS *param_1, int index) {
    if (param_1 == NULL) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= param_1->frame_count) {
        index = param_1->frame_count - 1;
    }
    param_1->frame = (short)index;
}

// FUNCTION: LEGOLAND 0x0047d5d0
void LLSNextFrame(struct LLS *param_1) {
    if (param_1 != NULL) {
        param_1->frame++;
        if (param_1->frame == param_1->frame_count) {
            param_1->frame = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0047d5f0
void LLSSetDelay(struct LLS *param_1, unsigned int param_2) {
    param_1->delay = (unsigned short)param_2;
    param_1->loop_delay = (short)param_2;
}

// FUNCTION: LEGOLAND 0x0047d610
void FUN_0047d610(struct LLS *param_1) {
    param_1->frame++;
    if (param_1->frame >= param_1->frame_count) {
        param_1->frame = 0;
    }
}

// FUNCTION: LEGOLAND 0x0047d630
void LLSAuto(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d6a0
void LLS555To565(struct LLSImage *param_1) {
    unsigned char *esi = (unsigned char *)param_1 + 0x18;

    if (param_1->format == 8) {
        unsigned short *data = (unsigned short *)(esi + *(int *)(esi + 4) + 8);
        int counter = 0x100;
        do {
            unsigned short pixel = *data;
            *data = ((pixel & 0xffe0) << 1) | (pixel & 0x1f);
            data++;
            counter--;
        } while (counter != 0);
    } else if (param_1->format == 0x10) {
        int rows = param_1->height;
        if (param_1->flags & 0x1) {
            rows++;
        }
        if (rows > 0) {
            int row_count = rows;
            do {
                int width = *(int *)(esi + 4);
                unsigned short *data = (unsigned short *)(esi + 0x10);
                if (width > 0) {
                    do {
                        unsigned short pixel = *data;
                        *data = ((pixel & 0xffe0) << 1) | (pixel & 0x1f);
                        data++;
                        width--;
                    } while (width != 0);
                }
                esi += *(unsigned int *)esi;
                row_count--;
            } while (row_count != 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x0047d730
unsigned int SaveGameRead(void *buffer, unsigned int count) {
    return FUN_0049f4ca(DAT_006691b0, buffer, count) == count;
}

// FUNCTION: LEGOLAND 0x0047d760
unsigned int SaveGameWrite(void *buffer, unsigned int count) {
    return (unsigned int)FUN_004a63e4(DAT_006691b0, buffer, count) == count;
}
