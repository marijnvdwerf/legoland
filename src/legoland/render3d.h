#pragma once

#include "legoland.h"

struct RideObject;
struct RinData;
struct SpriteLLS;
struct LayerContainer;
struct Matrix3x3;

struct BlokePos {
    unsigned int field_0;
    unsigned int field_4;
};

LEGO_EXPORT void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj);
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir);
LEGO_EXPORT void UnLoadRin(struct RinData *rin);
LEGO_EXPORT unsigned short *LoadPalette(unsigned int path);
LEGO_EXPORT unsigned int GetLLSForSprite(struct SpriteLLS *sprite);
LEGO_EXPORT unsigned int GetSpriteForLayer(struct LayerContainer *arg1, unsigned int arg2);
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C);
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out);
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest);
unsigned int FUN_00442c70(void);
LEGO_EXPORT void GetScreenCoordsForObject(unsigned short *value, void *obj);
LEGO_EXPORT void AdjustBlokePosition(struct BlokePos *pBloke);
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(struct RideObject *ride);
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(struct RideObject *ride);
unsigned int FUN_00443120(void);
