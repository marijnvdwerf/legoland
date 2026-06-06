#include "legoland.h"
#include "man3d.h"
#include "render3d.h"
#include "llidb.h"
#include <math.h>

extern void *DAT_0081c8cc;
extern unsigned int DAT_0081c8d0;
extern unsigned int DAT_0081c8d4;
extern unsigned int DAT_0081c8d8;
extern unsigned int DAT_0081c8dc;
extern void *DAT_00655a38;
extern void *DAT_0062fea8;
extern void *DAT_0062fef8;
extern void *DAT_0064cd8c;
extern unsigned int DAT_0062feec[];
extern unsigned int DAT_00630108[];
extern unsigned int DAT_00655a4c;
extern unsigned int DAT_0062fef0;
extern unsigned int DAT_00655a50;
extern unsigned int DAT_00638218[];

extern void FUN_0049e4d0(void *block);
#include "image_sprite.h"

struct Viewport {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
};

// FUNCTION: LEGOLAND 0x00441800
void Render_SetViewport(struct Viewport *viewport) {
    DAT_0081c8d0 = viewport->field_0;
    DAT_0081c8d8 = viewport->field_8;
    DAT_0081c8d4 = viewport->field_4;
    DAT_0081c8dc = viewport->field_c;
}

// FUNCTION: LEGOLAND 0x00441830
unsigned int FUN_00441830(void *param_1, unsigned int param_2) { STUB(); }

struct ViewportEntry {
    unsigned char pad_0[0xcc];
    void *field_cc;
};

// FUNCTION: LEGOLAND 0x00441870
void FUN_00441870(struct ViewportEntry *param_1, unsigned int param_2) {
    DAT_0081c8cc = param_1->field_cc;
    FUN_00441830(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00441890
unsigned int FUN_00441890(void *param_1, unsigned int param_2) {
    void **node = (void **)DAT_0081c8cc;
    if (node != NULL) {
        DAT_0081c8cc = *node;
        return FUN_00441830(param_1, param_2);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004418c0
void FUN_004418c0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441910
void FUN_00441910(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441980
void FUN_00441980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441a60
void Put3DBlokesOnRide(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441ad0
void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj) { STUB(); }

struct RenderNode {
    struct RenderNode *next;
    unsigned char pad_4[4];
    void *data_ptr;
    unsigned short id;
};

struct RenderBloke {
    unsigned short id;
};

struct RenderData {
    unsigned char pad_0[0x62];
    unsigned char flags;
};

struct RenderContext {
    unsigned char pad_0[0xcc];
    struct RenderNode *bloke_list;
};

// FUNCTION: LEGOLAND 0x00441b60
void RenderBlokesNotInSeats(unsigned int a1, unsigned int a2) {
    struct RenderContext *context = (struct RenderContext *)a1;
    struct RenderBloke *bloke = (struct RenderBloke *)a2;
    struct RenderNode *node = context->bloke_list;

    while (node != NULL) {
        if (bloke->id == node->id) {
            struct RenderData *data = (struct RenderData *)node->data_ptr;
            if ((data->flags & 0x80) == 0) {
                IP_RenderBlokeIn3DNow((struct Bloke *)data);
            }
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00441ba0
void LoadRin(void) { STUB(); }

struct RinData {
    unsigned char pad_0[0x10];
    int sprite_count;
    int data_count;
    void **sprites;
    void **datas;
};

// FUNCTION: LEGOLAND 0x00441cf0
void UnLoadRin(struct RinData *rin) {
    int i;
    int count;

    count = rin->sprite_count;
    for (i = 0; i < count; i++) {
        KillSprite((unsigned int)rin->sprites[i]);
        count = rin->sprite_count;
    }
    FUN_0049e4d0(rin->sprites);

    count = rin->data_count;
    for (i = 0; i < count; i++) {
        FUN_0049e4d0(rin->datas[i]);
        count = rin->data_count;
    }
    FUN_0049e4d0(rin->datas);

    FUN_0049e4d0(rin);
}

// FUNCTION: LEGOLAND 0x00441d60
void RenderUsingRin(void) { STUB(); }

struct SpriteLLS {
    unsigned char pad_0[8];
    unsigned int *field_8;
};

// FUNCTION: LEGOLAND 0x00441e80
unsigned int GetLLSForSprite(struct SpriteLLS *sprite) {
    if (sprite != NULL) {
        return *sprite->field_8;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00441ea0
unsigned int GetLLSForLayer(unsigned int a, unsigned int b) {
    unsigned int c = *(unsigned int *)(a + 8);
    unsigned int d = *(unsigned int *)(c + 8);
    unsigned int e = *(unsigned int *)(d + b * 4);
    if (e == 0)
        return 0;
    e = *(unsigned int *)(e + 8);
    return *(unsigned int *)e;
}

struct LayerContainer {
    unsigned char pad_0[8];
    struct SpriteListContainer *field_8;
};

struct SpriteListContainer {
    unsigned char pad_0[8];
    unsigned int *field_8;
};

// FUNCTION: LEGOLAND 0x00441ec0
unsigned int GetSpriteForLayer(struct LayerContainer *arg1, unsigned int arg2) {
    struct SpriteListContainer *container = arg1->field_8;
    unsigned int *sprite_array = container->field_8;
    return sprite_array[arg2];
}

// FUNCTION: LEGOLAND 0x00441ee0
void GetRenderOffsetForLayer(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00441f00
void StopLayerPlaying(unsigned int layerID, unsigned int someValue) {
    unsigned int lls_handle = GetLLSForLayer(layerID, someValue);
    LLSStop(lls_handle);
}

// FUNCTION: LEGOLAND 0x00441f20
void LoadPalette(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442040
void FUN_00442040(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004424e0
void FUN_004424e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442580
void FUN_00442580(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004427e0
void FUN_004427e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442860
void FUN_00442860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004428c0
unsigned char *FUN_004428c0(unsigned char *str, int count) {
    unsigned char *result = str;
    if (count > 0) {
        do {
            result = result + strlen((char *)result) + 1;
            count = count - 1;
        } while (count != 0);
    }
    return result;
}

// FUNCTION: LEGOLAND 0x004428f0
void FUN_004428f0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442980
void FUN_00442980(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442c70
unsigned int FUN_00442c70(void) {
    if (DAT_00655a38) {
        FUN_0049e4d0(DAT_00655a38);
    }
    if (DAT_0062fea8) {
        FUN_0049e4d0(DAT_0062fea8);
    }
    if (DAT_0062fef8) {
        FUN_0049e4d0(DAT_0062fef8);
    }
    if (DAT_0064cd8c) {
        FUN_0049e4d0(DAT_0064cd8c);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00442cc0
void GetScreenCoordsForObject(unsigned short *value, void *obj) { STUB(); }

struct AdjustStruct {
    int field0;
    int field4;
};

// FUNCTION: LEGOLAND 0x00442d30
void AdjustOffsetForViewMode(struct AdjustStruct *param_1) {
    int temp0;
    int temp4;

    temp0 = param_1->field0;
    if (temp0 < 0) {
        temp0 = -((-temp0) >> 1);
    } else {
        temp0 = temp0 >> 1;
    }
    param_1->field0 = temp0;

    temp4 = param_1->field4;
    if (temp4 < 0) {
        temp4 = -((-temp4) >> 1);
    } else {
        temp4 = temp4 >> 1;
    }
    param_1->field4 = temp4;
}

// FUNCTION: LEGOLAND 0x00442d60
void AdjustBlokePosition(struct BlokePos *pBloke) {
    pBloke->field_0 -= 0x4b;
    pBloke->field_4 -= 0x4d;
}

// FUNCTION: LEGOLAND 0x00442d80
void UnAdjustBlokePosition(struct BlokePos *pBloke) {
    pBloke->field_0 += 0x4b;
    pBloke->field_4 += 0x4d;
}

struct Vec3 {
    float x;
    float y;
    float z;
};

// FUNCTION: LEGOLAND 0x00442da0
void FUN_00442da0(struct Vec3 *a, struct Vec3 *b, struct Vec3 *out) {
    out->x = b->z * a->y - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

// FUNCTION: LEGOLAND 0x00442de0
float FUN_00442de0(struct Vec3 *param_1, struct Vec3 *param_2) {
    return param_1->z * param_2->z + param_1->y * param_2->y + param_1->x * param_2->x;
}

// FUNCTION: LEGOLAND 0x00442e00
void TMNegParity(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442e90
void RenderItems_New(void) {
    DAT_0062feec[0] = (unsigned int)&DAT_00630108;
    DAT_00655a4c = 0;
}

// FUNCTION: LEGOLAND 0x00442eb0
void RenderItem_Link(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442f20
void AddBlokeToRenderList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00442f50
unsigned int FUN_00442f50(void) {
    unsigned int result = DAT_0062feec[0];
    DAT_0062feec[0] = DAT_0062feec[0] + 16;
    DAT_00655a4c = DAT_00655a4c + 1;
    return result;
}

struct BlokeListNode {
    unsigned int field_0;
    unsigned int field_4;
    struct BlokeListNode *next;
};

struct BlokeListHead {
    struct BlokeListNode *next;
};

// FUNCTION: LEGOLAND 0x00442f70
void RenderBlokeList(struct BlokeListHead *list) {
    struct BlokeListNode *node = list->next;
    while (node) {
        if (node->field_4) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->field_4);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00442fa0
void Ride_SetFlagToNotLetAnyoneOn(struct RideObject *ride) { STUB(); }

// FUNCTION: LEGOLAND 0x00443000
void Ride_ClearFlagToNotLetAnyoneOn(struct RideObject *ride) { STUB(); }

// FUNCTION: LEGOLAND 0x00443060
void RenderItems2_New(void) {
    DAT_0062fef0 = (unsigned int)&DAT_00638218;
    DAT_00655a50 = 0;
}

// FUNCTION: LEGOLAND 0x00443080
void RenderItem2_Link(unsigned int param_1, unsigned int param_2, unsigned int param_3) { STUB(); }

struct RenderItem2 {
    unsigned int field_0;
    unsigned int field_4;
};

// FUNCTION: LEGOLAND 0x004430f0
void RenderItem2_AddItem(unsigned int param_1, unsigned int param_2, unsigned int param_3) {
    struct RenderItem2 *item = (struct RenderItem2 *)FUN_00443120();
    item->field_4 = param_2;
    item->field_0 = param_3;
    RenderItem2_Link(param_1, (unsigned int)item, param_3);
}

// FUNCTION: LEGOLAND 0x00443120
unsigned int FUN_00443120(void) {
    unsigned int result = DAT_0062fef0;
    DAT_0062fef0 = DAT_0062fef0 + 16;
    DAT_00655a50 = DAT_00655a50 + 1;
    return result;
}

struct BlokeSex0 {
    unsigned char pad_0[4];
    struct BlokeSex1 *field_4;
};

struct BlokeSex1 {
    unsigned char pad_0[0x84];
    unsigned int field_84;
};

// FUNCTION: LEGOLAND 0x00443140
unsigned int GetSexOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    return inner->field_84;
}

// FUNCTION: LEGOLAND 0x00443150
void GetFaceTextureNameOfBloke(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004431a0
void GetChestTextureNameOfBloke(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004431f0
void GetLegColourOfBloke(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443220
void GetArmColourOfBloke(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443250
float FUN_00443250(float param_1) {
    return (float)sin(param_1);
}

// FUNCTION: LEGOLAND 0x00443260
float FUN_00443260(float param_1) {
    return (float)cos(param_1);
}

// FUNCTION: LEGOLAND 0x00443270
void MatrixMultiply(float *A, float *B, float *C) {
    C[0] = A[0] * B[0] + A[1] * B[3] + A[2] * B[6];
    C[1] = A[1] * B[4] + A[2] * B[7] + A[0] * B[1];
    C[2] = A[1] * B[5] + A[2] * B[8] + A[0] * B[2];

    C[3] = A[4] * B[3] + A[3] * B[0] + A[5] * B[6];
    C[4] = A[5] * B[7] + A[4] * B[4] + A[3] * B[1];
    C[5] = A[5] * B[8] + A[3] * B[2] + A[4] * B[5];

    C[6] = A[6] * B[0] + A[8] * B[6] + A[7] * B[3];
    C[7] = A[8] * B[7] + A[7] * B[4] + A[6] * B[1];
    C[8] = A[8] * B[8] + A[7] * B[5] + A[6] * B[2];
}

// FUNCTION: LEGOLAND 0x00443360
void BuildYRotationMatrix(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004433b0
void TransformVectorsL(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00443450
void NormaliseVector(struct Vec3 *v) {
    float m = (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = v->x / m;
    v->y = v->y / m;
    v->z = v->z / m;
}

struct Matrix3x3 {
    unsigned int m[9];
};

// FUNCTION: LEGOLAND 0x00443490
void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest) {
    dest->m[0] = src->m[0];
    dest->m[1] = src->m[1];
    dest->m[2] = src->m[2];
    dest->m[3] = src->m[3];
    dest->m[4] = src->m[4];
    dest->m[5] = src->m[5];
    dest->m[6] = src->m[6];
    dest->m[7] = src->m[7];
    dest->m[8] = src->m[8];
}
