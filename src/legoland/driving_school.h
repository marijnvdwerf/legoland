#pragma once

#include "obj_instance.h"

struct DSHead;
struct DSRenderRoot;
struct RideObject;

void FUN_00405370(struct DSHead *param_1);
void FUN_00405460();
void FUN_00405570();
void FUN_00405630(unsigned int param_1, int *coords);
void FUN_00405740(struct DSHead *param_1, unsigned int param_2, unsigned int param_3);
void FUN_004058a0(unsigned int param_1, unsigned int param_2);
void FUN_00405940(struct RideObject *obj, TileId tile, unsigned int param_3);
unsigned int *FUN_00405ad0(struct DSCarLayer *arg1, unsigned short arg2);
void FUN_00405b10(struct RideObject *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, unsigned int param_5, unsigned int clip);
void FUN_00405bd0(struct RideObject *obj);
int FUN_00405e70(void);
int FUN_00406050(void);
int FUN_00406070(void);