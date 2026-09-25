#pragma once

#include "legoland.h"

struct Position;
struct Person {
    struct Person *prev;
    struct Person *next;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned char pad_24[0x2c - 0x24];
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned char pad_3c[0x40 - 0x3c];
    float field_40;
    float field_44;
    float field_48;
    int field_4c;
    void *field_50;
    unsigned int sort_id;
    union {
        int m[9];
        struct {
            unsigned char pad_58[0x62 - 0x58];
            unsigned char flags;
            unsigned char field_63;
            unsigned char pad_64[0x68 - 0x64];
            int field_68;
            int field_6c;
            unsigned short field_70;
            unsigned char field_72;
            unsigned char pad_73[1];
            unsigned char field_74;
        };
    };
    unsigned int field_7c;
    unsigned int field_80;
    unsigned int random;
    unsigned int field_88;
    unsigned int field_8c;
    unsigned int field_90;
};
struct Bloke;
struct PosHeader;

void FUN_0043f840(struct Person *person);
void FUN_0043f870(struct PosHeader *param_1);

LEGO_EXPORT struct Person *Find3DPersonFromBloke(unsigned int id);
LEGO_EXPORT void SetPersonRotation(struct Person *person, float *src);
LEGO_EXPORT void SetPersonDirection(struct Person *person, unsigned int direction);
LEGO_EXPORT void BlokeSetAnim(struct Bloke *bloke, int anim);
LEGO_EXPORT void BlokeSitAnim(struct Bloke *bloke);
LEGO_EXPORT void BlokeSetFrame(struct Bloke *bloke, int frame);
LEGO_EXPORT int PlayBlokeAnim(struct Bloke *bloke);

LEGO_EXPORT struct Position *LoadPos(const char *path);
LEGO_EXPORT void UnloadPos(struct Position *pos);
LEGO_EXPORT void RenderBlokeIn3D(struct Bloke *bloke);
LEGO_EXPORT void SortBlokeIn3D(struct Bloke *bloke);
LEGO_EXPORT void IP_RenderBlokeIn3DNow(struct Bloke *bloke);
LEGO_EXPORT void UpdatePerson(struct Person *person);
LEGO_EXPORT void Control3DPeople(void);
LEGO_EXPORT void Add3DBlokeToList(struct Bloke *bloke, unsigned int param_2);
LEGO_EXPORT void BlokeWalkAnim(struct Bloke *bloke);
LEGO_EXPORT void BlokePanWithPan(struct Bloke *bloke);
LEGO_EXPORT void BlokeAnimNextFrame(struct Bloke *bloke);
LEGO_EXPORT void BlokeWalkWithPan(struct Bloke *bloke);
LEGO_EXPORT void Render3DPerson(struct Person *person);
void FUN_004401b0(int param_1, int param_2);
void FUN_0043f810(struct Person *person);
void FUN_00440a30(struct Person *person);
LEGO_EXPORT void SetPersonPosition(struct Person *person, unsigned int x, unsigned int y);
