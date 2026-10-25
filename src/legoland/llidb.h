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
typedef struct LLS LLS;

/* An entry of the list of playing animations (LLSPlayList), advanced by LLSAuto. */
struct LLSNode {
    struct LLS *lls;
    struct LLSNode *next;
    void *param; /* LLSPlay's second argument */
};

/* An LLIDB element (0x14 bytes).  For object classes `data` is the class's Ride
   record; placed-object code (map_object.c, the ride TUs) reaches it as `ride`. */
/* A TSM file entry once loaded: the element and its data; the list ends with both -1. */
struct TSMEntry {
    struct Element *element;
    void *data;
};

struct Element {
    /* 0x00 */ char *name;
    /* 0x04 */ char *path;
    /* 0x08 */ unsigned int flags;
    /* 0x0c */ union {
        void *data;
        struct Ride *ride;
        struct Ride *obj; /* the class, as map_object.c names it */
    };
    /* 0x10 */ unsigned int field_10;
};
typedef struct Element Element;

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

LEGO_EXPORT void LLS555To565(struct LLSImage *param_1);

void LLIDB_UnLoadODF(struct Element *head);

struct SpriteSet;
LEGO_EXPORT void LLIDB_FreeILFTable(struct SpriteSet *table);

LEGO_EXPORT unsigned int LLIDB_GetCount(void);
LEGO_EXPORT int LLIDB_GetElement(unsigned int index, struct Element **output);
struct Sprite;
struct Sprite *LoadObjDescFile(struct Element *elem, char **name2, int *key, char ***after);
LEGO_EXPORT int LLIDB_FindElement(const char *name, struct Element **out, unsigned int *index_out);
LEGO_EXPORT int LLIDB_FindElementFromDataPtr(void *data, struct Element **out, unsigned int *index_out);
LEGO_EXPORT struct Element *ElemID(const char *name);
LEGO_EXPORT int LLIDB_LoadICM(void);
LEGO_EXPORT int LLIDB_CloseICM(void);
LEGO_EXPORT unsigned int LLIDB_RegisterNewElement(const char *param_1, const char *param_2, unsigned int param_3);
LEGO_EXPORT void LLIDB_ClearOnLevel(void);
LEGO_EXPORT void *LLIDB_LoadData(struct Element *head);
LEGO_EXPORT void LLIDB_UnLoadData(struct Element *handle);

LEGO_EXPORT void LLSAuto(void);
void LLSAdvanceFrame(struct LLS *param_1);
LEGO_EXPORT int LLSStop(struct LLS *lls);
LEGO_EXPORT void LLSPlay(struct LLS *param_1, void *param_2);
LEGO_EXPORT void LLSPlayOnce(struct LLS *param_1, void *param_2);
LEGO_EXPORT void LLSSetFrame(struct LLS *lls, int index);
LEGO_EXPORT void LLSNextFrame(struct LLS *param_1);

LEGO_EXPORT unsigned int SaveGameRead(void *buffer, unsigned int count);
LEGO_EXPORT unsigned int SaveGameWrite(void *buffer, unsigned int count);
