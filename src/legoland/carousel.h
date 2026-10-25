#pragma once

#include "legoland.h"

struct Bloke;
struct Cursor;
struct Element;
struct Ride;
struct RideNode;

/* Carousel ride node (one 0x2c allocation), head at CarouselNodeList. */
struct CarouselNode {
    /* 0x00 */ struct CarouselNode *next;
    /* 0x04 */ unsigned short id;
    /* 0x06 */ unsigned char seated_count;
    /* 0x07 */ unsigned char leaving_count;
    /* 0x08 */ unsigned char frame;
    /* 0x09 */ unsigned char pad_9[0xc - 0x9];
    /* 0x0c */ unsigned int flags;
    /* 0x10 */ unsigned char cycles_left;
    /* 0x11 */ unsigned char pad_11[0x14 - 0x11];
    /* 0x14 */ unsigned int frame_ticks;
    /* 0x18 */ unsigned char boarding_count;
    /* 0x19 */ unsigned char pad_19[0x1c - 0x19];
    /* 0x1c */ unsigned int boarding_timer;
    /* 0x20 */ unsigned char slots[0x2c - 0x20];
};

void AddCarouselNode(unsigned short *param_1);
void RemoveCarouselNode(struct CarouselNode *node);
void FreeAllCarouselNodes(void);
struct CarouselNode *FindCarouselNode(unsigned short *param_1);
void StartCarouselRide(struct CarouselNode *node);
void ResetCarouselRide(struct CarouselNode *node);
void UpdateCarouselNodes(void);
int TakeRandomCarouselSeat(struct RideNode *elem, struct CarouselNode *node, signed char divisor);

void RenderCarousel(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6);
void LoadCarouselResources(struct Element *param_1);
void UnloadCarouselResources(struct Element *input);
void CarouselSetEditMode();
void CarouselRemoveObject(struct Element *param_1, TileId tile, struct Cursor *param_3);
void CarouselAddObject(struct Element *param_1, unsigned char *param_2);
struct RideSpriteInfo *GetCarouselSpriteInfo(struct Element *param1, unsigned short param2);
int Carousel_Save(struct Element *element);
int Carousel_Load(struct Element *param_1);
void CarouselUpdate(struct Element *param_1);
