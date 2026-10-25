#pragma once

struct PlaneRideNode {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    char b4;
    char b5;
    unsigned char pad_6[2];
    unsigned int flags;
    char b12;
    unsigned char pad_d[3];
    unsigned int f10;
    unsigned char b14;
    unsigned char pad_15[3];
    int f18;
    signed char slots[4];
    struct PlaneRideNode *next;
};

struct Element;

struct PlaneRideNode;
void ResetPlaneRide(struct PlaneRideNode *node);
void StartPlaneRide(struct PlaneRideNode *node);
void PlaneRideUpdate(struct Element *elem);

struct Ride;
void PlaneRide_GetInterfaces(struct Element *name, struct Ride *iface);
