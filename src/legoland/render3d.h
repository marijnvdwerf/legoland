#pragma once

#include "legoland.h"

struct RideObject;
struct RinData;
struct SpriteLLS;
struct LayerContainer;
struct LayerOffsetHolder;
struct Matrix3x3;
struct AdjustStruct;
struct Viewport;
struct Person;

struct BlokePos {
    unsigned int field_0;
    unsigned int field_4;
};

LEGO_EXPORT void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj);
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir);
LEGO_EXPORT void UnLoadRin(struct RinData *rin);
LEGO_EXPORT unsigned short *LoadPalette(unsigned int path);
LEGO_EXPORT unsigned int GetLLSForSprite(struct SpriteLLS *sprite);
LEGO_EXPORT unsigned int GetLLSForLayer(unsigned int a, unsigned int b);
LEGO_EXPORT void StopLayerPlaying(unsigned int layerID, unsigned int someValue);
LEGO_EXPORT __int64 GetRenderOffsetForLayer(struct LayerOffsetHolder *param_1, int param_2);
LEGO_EXPORT unsigned int GetSpriteForLayer(struct LayerContainer *arg1, unsigned int arg2);
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C);
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out);
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest);
unsigned int FUN_00442c70(void);
unsigned int FUN_00442f50(void);
struct ResFile;
char *FUN_004427e0(struct ResFile *param_1, char *param_2, int param_3);
int FUN_00442860(char *param_1, char *param_2);
LEGO_EXPORT __int64 GetScreenCoordsForObject(unsigned char *param_1, void *param_2);
LEGO_EXPORT void AdjustBlokePosition(struct BlokePos *pBloke);
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(unsigned char *param_1);
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(unsigned char *param_1);
unsigned int FUN_00443120(void);
LEGO_EXPORT void AdjustOffsetForViewMode(struct AdjustStruct *param_1);
LEGO_EXPORT void Render_SetViewport(struct Viewport *viewport);
void *FUN_00442580(struct Person *person, void *context, unsigned int arg3, unsigned int arg4, unsigned int arg5);
void FUN_00442980(const char *param_1, const char *param_2, const char *param_3, int param_4, unsigned int param_5);
