#pragma once

#include "legoland.h"
#include "llidb.h"
#include "math.h"

struct RinData {
    int x; /* RenderUsingRin: x, y and the remap/data tables (zero for the .rin files LoadRin reads) */
    int y;
    int *remap_table;
    int *data_table;
    int sprite_count;
    int data_count;
    struct Sprite **sprites;
    void **datas;
};

/* One face of a 3D person's mesh (0x24 bytes in the mesh file): a solid colour (flags & 0x2000) with the colour
   ramp the loader looks up for it, or a texture cell; then the corners' texture coordinates. */
struct PersonFace {
    /* 0x00 */ unsigned int flags; /* 0x2000: solid colour */
    /* 0x04 */ union {
        struct {
            unsigned char b0;
            unsigned char b1;
            unsigned char b2;
        };
        unsigned char rgb[3];
        int field_4;
    };
    /* 0x08 */ union {
        int cell; /* texture cell index */
        unsigned int texture; /* the texture id */
        void *ramp; /* solid colour: GetOrCreateColorRamp's colour ramp */
    };
    /* 0x0c */ float uv[6]; /* u0 v0 u1 v1 u2 v2 */
};

struct LocFile;
struct Position;
struct Sprite;
struct LLS;
struct Ride;

struct RinData;
struct Matrix3x3;
struct tagRECT;
struct Person;
struct RideNode;

struct RenderItemNode {
    /* 0x00 */ int key;
    /* 0x04 */ void *item; /* the bloke, sprite or slot drawn at this depth */
    /* 0x08 */ struct RenderItemNode *next;
    /* 0x0c */ struct RenderItemNode *prev;
};

struct BlokeSex0;
LEGO_EXPORT unsigned int GetSexOfBloke(struct BlokeSex0 *param_1);
LEGO_EXPORT char *GetFaceTextureNameOfBloke(struct BlokeSex0 *param_1);
LEGO_EXPORT char *GetChestTextureNameOfBloke(struct BlokeSex0 *param_1);
struct Vec3;
LEGO_EXPORT void NormaliseVector(struct Vec3 *v);
LEGO_EXPORT unsigned int GetLegColourOfBloke(struct BlokeSex0 *param_1);
LEGO_EXPORT unsigned int GetArmColourOfBloke(struct BlokeSex0 *param_1);
LEGO_EXPORT void Put3DBlokesOnRide(struct Ride *param_1, unsigned char *param_2, int param_3, struct Position *param_4);
/* obj: the ride's node for the tile, read only as the tile key it starts with */
LEGO_EXPORT void Put3DBlokesOnRide2(struct Ride *ride, void *obj);
LEGO_EXPORT void RenderBlokesNotInSeats(struct Ride *a1, TileId *a2);
LEGO_EXPORT void RenderUsingRin(struct RinData *param_1, int param_2, struct Ride *param_3, unsigned char *param_4);
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir);
LEGO_EXPORT void UnLoadRin(struct RinData *rin);
LEGO_EXPORT unsigned short *LoadPalette(const char *path);
LEGO_EXPORT struct LLS *GetLLSForSprite(struct Sprite *sprite);
LEGO_EXPORT void RenderItems2_New(void);
LEGO_EXPORT void RenderItem2_AddItem(struct RenderItemNode **head, void *param_2, int key);
LEGO_EXPORT struct LLS *GetLLSForLayer(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT void StopLayerPlaying(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT struct Point GetRenderOffsetForLayer(struct Sprite *sprite, int index);
LEGO_EXPORT struct Sprite *GetSpriteForLayer(struct Sprite *sprite, unsigned int index);
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C);
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out);
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest);
unsigned int FreeRender3DBuffers(void);
struct RenderItemNode *AllocRenderItem(void);
struct ResFile;
char *RES_ReadLine(struct ResFile *param_1, char *param_2, int param_3);
int FindIndexInStringList(char *param_1, char *param_2);
LEGO_EXPORT struct Point GetScreenCoordsForObject(TileId *tile, struct Ride *ride);
struct Point;
LEGO_EXPORT void AdjustBlokePosition(struct Point *pos);
LEGO_EXPORT void UnAdjustBlokePosition(struct Point *pos);
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(void *param_1);
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(void *param_1);
struct RenderItemNode *AllocRenderItem2(void);
LEGO_EXPORT void AdjustOffsetForViewMode(struct Point *param_1);
LEGO_EXPORT void RenderItems_New(void);
LEGO_EXPORT void AddBlokeToRenderList(struct RenderItemNode **head, struct RideNode *src, int key);
LEGO_EXPORT void RenderBlokeList(struct RenderItemNode **list);
LEGO_EXPORT void Render_SetViewport(struct tagRECT *viewport);
void *CreatePersonFaceList(struct Person *person, struct LocFile *context, struct PersonFace *arg3, unsigned int arg4, unsigned int arg5);
void LoadPersonAltTextureList(const char *param_1, const char *param_2, const char *param_3, int param_4, unsigned int param_5);
LEGO_EXPORT int TMNegParity(float *param_1);
LEGO_EXPORT void TransformVectorsL(void);
