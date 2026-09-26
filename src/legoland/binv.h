#pragma once

#include "legoland.h"

// Per-TU header for binv.c — the BinV (.bnv) animation file: frames of named objects, each
// with an orientation matrix and a list of vertices. On disk the pointer fields hold offsets
// from the start of the file; LoadBinV relocates them in place.

typedef struct BinVFile BinVFile;
typedef struct BinVFrame BinVFrame;
typedef struct BinVObject BinVObject;
typedef struct Vertex Vertex;

struct Vertex {
    /* 0x00 */ short x; /* screen position */
    /* 0x02 */ short y;
    /* 0x04 */ float z;
    /* 0x08 */ float field_8;
    /* 0x0c */ float field_c;
    /* 0x10 */ float depth;
};

struct BinVObject {
    /* 0x00 */ int count;
    /* 0x04 */ union {
        BinVObject *next;
        unsigned int next_ofs;
    };
    /* 0x08 */ union {
        Vertex *vertices;
        unsigned int vertices_ofs;
    };
    /* 0x0c */ union {
        char *name;
        unsigned int name_ofs;
    };
    /* 0x10 */ float m10; /* 3x3 orientation matrix */
    /* 0x14 */ float m14;
    /* 0x18 */ float m18;
    /* 0x1c */ float m1c;
    /* 0x20 */ float m20;
    /* 0x24 */ float m24;
    /* 0x28 */ float m28;
    /* 0x2c */ float m2c;
    /* 0x30 */ float m30;
};

struct BinVFrame {
    /* 0x00 */ int count;
    /* 0x04 */ union {
        BinVObject *objects;
        unsigned int objects_ofs;
    };
    /* 0x08 */ union {
        BinVFrame *next;
        unsigned int next_ofs;
    };
};

struct BinVFile {
    /* 0x00 */ short magic;
    /* 0x02 */ unsigned short frameCount;
    /* 0x04 */ unsigned char pad_4[0x14 - 0x4];
    /* 0x14 */ float field_14;
    /* 0x18 */ unsigned char pad_18[0x20 - 0x18];
    /* 0x20 */ union {
        BinVFrame *frames;
        unsigned int frames_ofs;
    };
};

LEGO_EXPORT BinVFile *LoadBinV(const char *filename);
LEGO_EXPORT float GetUnitDepth(float near_z, float far_z);
LEGO_EXPORT float GetZSkew(BinVFile *file, BinVObject *object, Vertex *vertex);
LEGO_EXPORT void FreeBinV(BinVFile *binv);
LEGO_EXPORT BinVFrame *GetBinVFrame(BinVFile *file, int index);
LEGO_EXPORT BinVObject *GetObjectFromName(BinVFrame *frame, const char *name);
LEGO_EXPORT Vertex *GetVertex(BinVObject *object, int index);
