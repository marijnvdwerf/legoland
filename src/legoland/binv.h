#pragma once

#include "legoland.h"

// Per-TU header for binv.c — canonical declarations for the BinV model loader.

struct BinVFile;
struct BinVFrame;
struct BinVObject;
struct Vertex;

LEGO_EXPORT struct BinVFile *LoadBinV(const char *filename);
LEGO_EXPORT double GetUnitDepth(unsigned int param_1, unsigned int param_2);
LEGO_EXPORT double GetZSkew(struct BinVFile *file, struct BinVObject *object, struct Vertex *vertex);
LEGO_EXPORT void FreeBinV(void *binv);
LEGO_EXPORT struct BinVFrame *GetBinVFrame(struct BinVFile *file, int index);
LEGO_EXPORT struct BinVObject *GetObjectFromName(struct BinVFrame *frame, const char *name);
LEGO_EXPORT struct Vertex *GetVertex(struct BinVObject *object, int index);
