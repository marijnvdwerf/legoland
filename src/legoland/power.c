#include "power.h"

#include <stdlib.h>

#include "globals.h"
#include "legoland.h"
#include "map_object.h"
#include "sound_music.h"

// FUNCTION: LEGOLAND 0x00452990
void LoadFountainSFX(struct Element *head) {
    unsigned int counter;

    counter = FountainSFXRefCount;
    FountainSFXRefCount++;
    if (counter == 0) {
        Load_FXList(FountainSFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x004529c0
void KillFountainSFX(void) {
    FountainSFXRefCount--;
    if (FountainSFXRefCount == 0) {
        Kill_FXList(FountainSFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x004529e0
void FountainAddObject(struct Element *param_1, int *param_2) {
    struct SampleSource params;
    AddBasicObject(param_1, param_2);
    params.x = param_2[0];
    params.type = 2;
    params.y = param_2[1];
    PlayInstanceOfSample(FountainSFX[0].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00452a30
LEGO_EXPORT void RemoveSoundObject(struct Element *a, unsigned int b, struct Cursor *c) {
    unsigned char *bb = (unsigned char *)&b;
    struct SampleSource params;
    StandardRemoveObject(a, *(TileId *)&b, c);
    params.type = 2;
    params.x = bb[0];
    params.y = bb[1];
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x00452a80
void LoadPowerStationSFX(struct Element *head) {
    unsigned int counter;

    counter = PowerStationSFXRefCount;
    PowerStationSFXRefCount++;
    if (counter == 0) {
        Load_FXList(PowerStationSFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00452ab0
void KillPowerStationSFX(void) {
    PowerStationSFXRefCount--;
    if (PowerStationSFXRefCount == 0) {
        Kill_FXList(PowerStationSFX, 2);
    }
}

// FUNCTION: LEGOLAND 0x00452ad0
void SmallPowerStationAddObject(struct Element *param_1, int *param_2) {
    struct SampleSource params;
    AddBasicObject(param_1, param_2);
    params.x = param_2[0];
    params.type = 2;
    params.y = param_2[1];
    PlayInstanceOfSample(PowerStationSFX[1].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00452b20
void CrystalPowerStationAddObject(struct Element *param_1, int *param_2) {
    struct SampleSource params;
    AddBasicObject(param_1, param_2);
    params.x = param_2[0];
    params.type = 2;
    params.y = param_2[1];
    PlayInstanceOfSample(PowerStationSFX[0].sample, 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x00452b70
void LoadDinoSFX(struct Element *head) {
    unsigned int counter;

    counter = DinoSFXRefCount;
    DinoSFXRefCount++;
    if (counter == 0) {
        Load_FXList(DINO_SFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x00452ba0
void KillDinoSFX(void) {
    DinoSFXRefCount--;
    if (DinoSFXRefCount == 0) {
        Kill_FXList(DINO_SFX, 5);
    }
}

// FUNCTION: LEGOLAND 0x00452bc0
void DinoAddObject(struct Element *param_1, int *param_2) {
    struct SampleSource params;
    unsigned int r;
    AddBasicObject(param_1, param_2);
    params.type = 2;
    params.x = param_2[0];
    params.y = param_2[1];
    r = (unsigned int)rand() % 5;
    PlayInstanceOfSample(DINO_SFX[r].sample, 1, 1, &params);
}
