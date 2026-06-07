#include "legoland.h"
#include "crt.h"

#include "binv.h"

struct Vertex {
    unsigned char data[20];
};

struct BinVModel {
    int count;
    unsigned char pad_4[4];
    struct Vertex *vertices;
};


// FUNCTION: LEGOLAND 0x0044dc90
LEGO_EXPORT void LoadBinV(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044dd60
LEGO_EXPORT void FreeBinV(void *binv) {
    if (binv != NULL) {
        FUN_0049e4d0(binv);
    }
}

// FUNCTION: LEGOLAND 0x0044dd70
LEGO_EXPORT void GetBinVFrame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044dda0
LEGO_EXPORT void GetObjectFromName(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044ddf0
LEGO_EXPORT struct Vertex *GetVertex(struct BinVModel *model, int index) {
    if (model == NULL) {
        return NULL;
    }
    if (index >= model->count) {
        return NULL;
    }
    return model->vertices + index;
}

// FUNCTION: LEGOLAND 0x0044de20
LEGO_EXPORT void GetZSkew(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044de50
LEGO_EXPORT void GetUnitDepth(void) { STUB(); }
