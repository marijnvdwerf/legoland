#pragma once

struct Element;
struct RideNode;

#include "objclass.h"

struct SpaceTowerSeat {
    /* 0x00 */ unsigned int flags;
    /* 0x04 */ unsigned char pad_4[4];
    /* 0x08 */ int pos;
    /* 0x0c */ int state;
    /* 0x10 */ int field_10;
    /* 0x14 */ int descending;
    /* 0x18 */ struct RideNode *rider_slot0;
    /* 0x1c */ struct RideNode *rider_slot1;
    /* 0x20 */ signed char delta;
    /* 0x21 */ unsigned char pad_21[3];
};

struct SpaceTowerCar {
    /* 0x00 */ unsigned short var_0;
    /* 0x02 */ unsigned char var_2;
    /* 0x03 */ unsigned char var_3;
    /* 0x04 */ unsigned char var_4;
    /* 0x05 */ unsigned char pad_5[3];
    /* 0x08 */ struct SpaceTowerCar *next;
    /* 0x0c */ unsigned int var_c;
    /* 0x10 */ unsigned int var_10;
    /* 0x14 */ struct SpaceTowerSeat seats[4];
    /* 0xa4 */ unsigned char flags_a4[8];
    /* 0xac */ unsigned char var_ac;
    /* 0xad */ unsigned char var_ad;
    /* 0xae */ unsigned char pad_ae[2];
    /* 0xb0 */ unsigned int var_b0;
};

struct SpaceTowerCar;

struct AnimEntry {
    /* 0x00 */ int count;
    /* 0x04 */ struct AnimStep *steps;
};

struct AnimLayout {
    /* 0x00 */ int count;
    /* 0x04 */ struct AnimEntry **entries;
};

unsigned int TakeRandomSpaceTowerSlot(struct RideNode *param_1, struct SpaceTowerCar *param_2);
void SpaceTowerUpdate(struct Element *param_1);

struct Ride;
void SpaceTowerRide(struct Element *head, struct Ride *iface);
