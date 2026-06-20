#pragma once

#include "objclass.h"

struct SafariNode {
    unsigned short field_0;
    unsigned char pad_2[14];
    struct SafariNode *next;
    unsigned char pad_14[0x14];
};

struct SafariKey;
struct SafariLoadArg;

void *FUN_00414a80(struct SafariKey *key);
void FUN_00414b10(struct SafariNode *node);
void FUN_00415030(struct ClassNode *name, struct CallbackTable *interfaces);
void FUN_00415220(void);
LEGO_EXPORT int SaveSafariRide(void);
LEGO_EXPORT int LoadSafariRide(struct SafariLoadArg *arg);
