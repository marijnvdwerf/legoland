#pragma once

// Per-TU header for binv.c — canonical declarations for the BinV model loader.

struct Vertex {
    unsigned char data[20];
};

struct BinVModel {
    int count;
    unsigned char pad_4[4];
    struct Vertex *vertices;
};

void LoadBinV(void);
void FreeBinV(void *binv);
void GetBinVFrame(void);
void GetObjectFromName(void);
struct Vertex *GetVertex(struct BinVModel *model, int index);
void GetZSkew(void);
void GetUnitDepth(void);
