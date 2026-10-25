#pragma once

struct Element;
struct RideNode;

struct BarrelNode {
    struct BarrelNode *next;
    unsigned short tile_id;
    unsigned char seated_count;
    unsigned char leaving_count;
    signed char frame;
    unsigned char pad_9[3];
    unsigned int flags;
    unsigned char cycles_left;
    unsigned char pad_11[3];
    int frame_ticks;
    unsigned char boarding_count;
    unsigned char pad_19[3];
    unsigned int boarding_timer;
    signed char layer2_frame;
    signed char slots[0x34 - 0x21];
};

void ResetBarrelRide(struct BarrelNode *node);
void SpinningBarrelsUpdate(struct Element *elem);
unsigned int PickBarrelSeat(struct RideNode *rn, struct BarrelNode *node, signed char n);

struct Ride;
void SpinningBarrelsGetInterfaces(struct Element *str, struct Ride *ride);
