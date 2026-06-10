#pragma once

#include "legoland.h"

struct CastleObj;

void FUN_00474880(void);
void FUN_004748a0(void *a);
int FUN_00474920(void);
int FUN_00474970(void);
LEGO_EXPORT int InitGameInterface(int a);
void FUN_00474ed0(void);
unsigned char FUN_00474f80(unsigned int a, unsigned int flags);
void FUN_00475080(void);
unsigned char FUN_00475120(unsigned int a, unsigned int flags, unsigned int c, unsigned int d);
LEGO_EXPORT void DelObjectList(void);
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry);
void FUN_00476000(void);
void FUN_00476050(void);
void FUN_004760a0(void);
void FUN_00476070(int mask, unsigned int value);
void FUN_00476140(int index, int value);
void FUN_004771f0(const char *filename, unsigned int param_2, unsigned int param_3);
struct CastleObj *FUN_004775b0(unsigned int size, unsigned int a, unsigned int b, unsigned int c);
void FUN_004775d0(unsigned int param);
LEGO_EXPORT void DisableSidePanelIcons(void);
LEGO_EXPORT void EnableSidePanelIcons(void);
LEGO_EXPORT void ListChildrenBar(void *node, int group, short x, short y);
LEGO_EXPORT void CloseChildrenBar(void *node, int group, short x, short y);
