#pragma once

#include "legoland.h"
#include "math.h"

struct Sprite;
struct LLS;
struct Ride;

struct RideObject;
struct RinData;
struct SpriteLLS;
struct LayerContainer;
struct LayerOffsetHolder;
struct Matrix3x3;
struct tagRECT;
struct Person;
struct ViewportEntry;
struct RinRender;
struct BlokeRenderSrc;
struct BlokeListHead;

struct RenderItemNode {
    /* 0x00 */ int key;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ struct RenderItemNode *next;
    /* 0x0c */ struct RenderItemNode *prev;
};

LEGO_EXPORT void Put3DBlokesOnRide(struct ViewportEntry *param_1, unsigned char *param_2, int param_3, int *param_4);
LEGO_EXPORT void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj);
LEGO_EXPORT void RenderBlokesNotInSeats(unsigned int a1, unsigned int a2);
LEGO_EXPORT void RenderUsingRin(struct RinRender *param_1, int param_2, struct ViewportEntry *param_3, unsigned char *param_4);
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir);
LEGO_EXPORT void UnLoadRin(struct RinData *rin);
LEGO_EXPORT unsigned short *LoadPalette(unsigned int path);
LEGO_EXPORT unsigned int GetLLSForSprite(struct SpriteLLS *sprite);
LEGO_EXPORT struct LLS *GetLLSForLayer(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT void StopLayerPlaying(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT struct Point GetRenderOffsetForLayer(struct Sprite *sprite, int index);
LEGO_EXPORT struct Sprite *GetSpriteForLayer(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C);
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out);
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest);
unsigned int FUN_00442c70(void);
unsigned int FUN_00442f50(void);
struct ResFile;
char *FUN_004427e0(struct ResFile *param_1, char *param_2, int param_3);
int FUN_00442860(char *param_1, char *param_2);
LEGO_EXPORT struct Point GetScreenCoordsForObject(TileId *tile, struct Ride *ride);
LEGO_EXPORT void AdjustBlokePosition(struct Point *pos);
LEGO_EXPORT void UnAdjustBlokePosition(struct Point *pos);
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(void *param_1);
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(void *param_1);
unsigned int FUN_00443120(void);
LEGO_EXPORT void AdjustOffsetForViewMode(struct Point *param_1);
LEGO_EXPORT void RenderItems_New(void);
LEGO_EXPORT void AddBlokeToRenderList(struct RenderItemNode **head, struct BlokeRenderSrc *src, int key);
LEGO_EXPORT void RenderBlokeList(struct BlokeListHead *list);
LEGO_EXPORT void Render_SetViewport(struct tagRECT *viewport);
void *FUN_00442580(struct Person *person, void *context, unsigned int arg3, unsigned int arg4, unsigned int arg5);
void FUN_00442980(const char *param_1, const char *param_2, const char *param_3, int param_4, unsigned int param_5);
