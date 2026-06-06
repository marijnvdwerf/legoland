#pragma once

struct RideObject;
struct RinData;
struct SpriteLLS;
struct LayerContainer;
struct BlokePos;

void Put3DBlokesOnRide2(struct RideObject *ride, struct RideObject *obj);
void UnLoadRin(struct RinData *rin);
unsigned int GetLLSForSprite(struct SpriteLLS *sprite);
unsigned int GetSpriteForLayer(struct LayerContainer *arg1, unsigned int arg2);
unsigned int FUN_00442c70(void);
void GetScreenCoordsForObject(unsigned short *value, void *obj);
void AdjustBlokePosition(struct BlokePos *pBloke);
void Ride_SetFlagToNotLetAnyoneOn(struct RideObject *ride);
void Ride_ClearFlagToNotLetAnyoneOn(struct RideObject *ride);
unsigned int FUN_00443120(void);
