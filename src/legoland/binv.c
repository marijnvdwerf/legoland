#include "legoland.h"

#include "binv.h"

extern void FUN_0049e4d0(void *block);

// FUNCTION: LEGOLAND 0x0044dc90
void LoadBinV(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044dd60
void FreeBinV(void *binv) {
    if (binv != NULL) {
        FUN_0049e4d0(binv);
    }
}

// FUNCTION: LEGOLAND 0x0044dd70
void GetBinVFrame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044dda0
void GetObjectFromName(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044ddf0
struct Vertex *GetVertex(struct BinVModel *model, int index) {
    if (model == NULL) {
        return NULL;
    }
    if (index >= model->count) {
        return NULL;
    }
    return model->vertices + index;
}

// FUNCTION: LEGOLAND 0x0044de20
void GetZSkew(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044de50
void GetUnitDepth(void) { STUB(); }
