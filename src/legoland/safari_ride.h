#pragma once

#include "objclass.h"

struct SafariNode {
    union {
        unsigned short tile_id;
        struct {
            unsigned char tile_x;
            unsigned char tile_y;
        };
    };
    unsigned char pad_2[2];
    int field_4; /* riders seated */
    int field_8;
    int frame;
    struct SafariNode *next;
    unsigned int field_14; /* flags */
    int loops_left;
    int frame_ticks;
    int field_20; /* riders boarding */
    int field_24; /* boarding timer */
};

struct SafariKey;

void *FindSafariNode(struct SafariKey *key);
void ResetSafariRide(struct SafariNode *node);
struct Ride;
void SafariRideGetInterfaces(struct Element *name, struct Ride *interfaces);
struct Element;
void SafariRideUpdate(struct Element *obj);
int GetSafariRiderIndex(struct RideNode *node, unsigned short *key);
struct SafariSample;
void StartSafariRide(struct SafariNode *a1);
LEGO_EXPORT int SaveSafariRide(struct Element *element);
LEGO_EXPORT int LoadSafariRide(struct Element *arg);
