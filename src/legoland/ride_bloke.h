#pragma once

#include "legoland.h"
#include "ride_queue.h"

struct NewBloke {
    struct NewBloke *next;
    unsigned short id;
    unsigned char pad_6[0x8 - 0x6];
    int sx;
    int sy;
    union {
        struct Point f;
        struct {
            int fx;
            int fy;
        };
    };
    union {
        struct Point p;
        struct {
            int px;
            int py;
        };
    };
    union {
        struct Point t;
        struct PathPair tp;
        struct {
            int tx;
            int ty;
        };
    };
    int velX;
    int velY;
    union {
        struct Point wp[17]; /* the path's waypoints */
        unsigned int timers[34];
        struct {
            struct Point dest; /* wp[0] */
            struct Point pad_wp[15];
            float dirX; /* wp[16]: the heading */
            float dirY;
        };
    };
    unsigned char f_b8;
    unsigned char f_b9;
    unsigned char f_ba;
    unsigned char f_bb;
    short f_bc;
    unsigned short f_be;
    unsigned short f_c0;
    unsigned char f_c2;
    unsigned char f_c3;
    unsigned char f_c4;
    unsigned char pad_c5;
    unsigned short f_c6;
    unsigned short f_c8;
    unsigned char pad_ca[2];
    union {
        int owner; /* in the saved game: the bloke's number */
        struct Bloke *bloke;
    };
};

struct HistPair {
    int a;
    int b;
};

struct HistBuf {
    unsigned char pad_00[0x30];
    /* 0x30 */ struct HistPair e[17];
    unsigned char pad_b8[3];
    /* 0xbb */ unsigned char count;
};

void ShiftDrivingHistory(struct HistBuf *p);
struct PathPair;
int PickQueueTurn(unsigned short id, struct PathPair *p, int dir);
int GetQueueTurn(unsigned short id, struct PathPair *p, int dir);
int AddDrivingSchoolCar(unsigned short id, struct Bloke *bloke);
unsigned int CountNewBlokesWithId(unsigned short arg0);
void RemoveDrivingSchoolCar(struct NewBloke *b);
void UpdateDrivingSchoolCars(void);
