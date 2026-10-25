#pragma once

#include "legoland.h"
#include "objclass.h"

struct SlideNode {
    unsigned short key;
    unsigned char pad_2[6];
    struct SlideNode *next;
    unsigned char pad_c[4];
    unsigned int slots[4];
};

struct Ride;
LEGO_EXPORT void TempleSlide_GetInterfaces(struct Element *head, struct Ride *iface);

struct TempleRide;
void ClearTempleRideVar4AndFlag1(struct TempleRide *arg);
