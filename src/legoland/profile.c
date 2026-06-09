#include "legoland.h"
#include "globals.h"

#include "clipping.h"
#include "profile.h"
#include "profile_io.h"
#include "title.h"
#include "icon.h"
#include "sound_music.h"

struct ProfileFlags {
    unsigned char pad_0[0x34];
    unsigned int var_34;
};

struct Profile {
    unsigned char pad_0[0x1c];
    unsigned char var_1c;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x0048c260
LEGO_EXPORT void InitListProfiles(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048c5e0
void FUN_0048c5e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048c650
LEGO_EXPORT void EnterNewProfileCheckBoxIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048c720
LEGO_EXPORT void InitProfileCheckBoxIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048c860
void FUN_0048c860(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048c9a0
LEGO_EXPORT void KillFrontEndCheckBoxSprite(void) {
    if (DAT_00798674 != NULL) {
        KillSprite(DAT_00798674);
        DAT_00798674 = NULL;
    }
    if (DAT_00798678 != NULL) {
        KillSprite(DAT_00798678);
        DAT_00798678 = NULL;
    }
    if (DAT_00798680 != NULL) {
        KillSprite(DAT_00798680);
        DAT_00798680 = NULL;
    }
    if (DAT_0079867c != NULL) {
        KillSprite(DAT_0079867c);
        DAT_0079867c = NULL;
    }
    if (DAT_00798684 != NULL) {
        KillSprite(DAT_00798684);
        DAT_00798684 = NULL;
    }
    if (DAT_00798688 != NULL) {
        KillSprite(DAT_00798688);
        DAT_00798688 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0048ca40
LEGO_EXPORT void KillListProfileSprite(void) {
    struct Sprite *sprite;
    sprite = DAT_0079868c;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_0079868c = NULL;
    }
    sprite = DAT_00798690;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_00798690 = NULL;
    }
    sprite = DAT_00798694;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_00798694 = NULL;
    }
    sprite = DAT_00798698;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_00798698 = NULL;
    }
    sprite = DAT_0079869c;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_0079869c = NULL;
    }
    sprite = DAT_007986a0;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986a0 = NULL;
    }
    sprite = DAT_007986a4;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986a4 = NULL;
    }
    sprite = DAT_007986a8;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986a8 = NULL;
    }
    sprite = DAT_007986ac;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986ac = NULL;
    }
    sprite = DAT_007986b0;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986b0 = NULL;
    }
    sprite = DAT_007986b4;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986b4 = NULL;
    }
    sprite = DAT_007986b8;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986b8 = 0;
    }
    sprite = DAT_007986bc;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986bc = NULL;
    }
    sprite = DAT_007986c0;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986c0 = NULL;
    }
    sprite = DAT_007986c4;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986c4 = NULL;
    }
    sprite = DAT_007986c8;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986c8 = NULL;
    }
    sprite = DAT_007986cc;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986cc = NULL;
    }
    sprite = DAT_007986d0;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986d0 = NULL;
    }
    sprite = DAT_007986d4;
    if (sprite != NULL) {
        KillSprite(sprite);
        DAT_007986d4 = NULL;
    }
}

// FUNCTION: LEGOLAND 0x0048cc10
LEGO_EXPORT void CloseFontEndCheckBox(void) {
    RemoveIconGroup(0xE);
    KillFrontEndCheckBoxSprite();
    DAT_004bef9c = 1;
}

// FUNCTION: LEGOLAND 0x0048cc30
void FUN_0048cc30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048cd50
LEGO_EXPORT void LightUpthisDeleteIcon(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048ce20
LEGO_EXPORT void UpdateProfileCheckBoxIcons(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048cf10
LEGO_EXPORT void PrintProfileDetails(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048d230
void FUN_0048d230(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0048d300
unsigned char FUN_0048d300(unsigned int dummy, unsigned char arg_0) {
    if (DAT_007986e4 == 0 && (arg_0 & 0x2) != 0 && ((((struct ProfileFlags *)DAT_007986e0)->var_34 >> 8) & 0x4) == 0 && DAT_0080ffa0.field_43 != 0) {
        if (DAT_007986e8 != 0) {
            SaveProfileToDisk();
            DeleteProfileList();
            LoadProfilesFormDisk();
            RemoveIconGroup(0x15);
            CloseFontEndCheckBox();
            DAT_007986e8 = 0;
        }
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff88 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048d390
unsigned char FUN_0048d390(struct Profile *profile, unsigned char param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 0x2) != 0) {
        DAT_0080ffa0.field_43 = profile->var_1c;
        FUN_0048a800();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048d3c0
unsigned char FUN_0048d3c0(struct Profile *profile, unsigned int param_2) {
    if (DAT_004bef9c != 0) {
        if (param_2 & 0x2) {
            DAT_0080ffa0.field_43 = profile->var_1c;
            DAT_007986e8 = 1;
            InitNewProfilePoPUp(profile);
            DAT_004bef9c = 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048d400
unsigned char FUN_0048d400(unsigned int arg0, unsigned int arg1) {
    if (arg1 & 0x2) {
        if (DAT_0080ffa0.field_43) {
            CloseFontEndCheckBox();
            DAT_007986e4 = 0;
            RemoveProfile(DAT_0080ffa0.field_43);
            DAT_0080ffa0.field_43 = 0;
            DAT_0080ff84 = 0xffffffff;
            DAT_0080ff88 = 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048d450
unsigned char FUN_0048d450(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        DAT_007986e4 = 0;
        CloseFontEndCheckBox();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048d470
void FUN_0048d470(void) {
    DAT_007986f8 = DAT_006687bc;
    DAT_007986f4 = DAT_006687c0;
}

// FUNCTION: LEGOLAND 0x0048d490
void FUN_0048d490(void) {
    DAT_006687bc = DAT_007986f8;
    DAT_006687c0 = DAT_007986f4;
}
