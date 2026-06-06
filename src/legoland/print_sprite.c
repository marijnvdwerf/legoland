#include "legoland.h"

#include "print_sprite.h"
#include "render.h"

struct PrintListNode {
    unsigned int field_0;
    struct PrintListNode *next;
};

struct PersonBlock {
    unsigned char pad_0[8];
    unsigned int field_8;
    unsigned int field_c;
    unsigned char pad_10[12];
    unsigned int field_1c;
};

extern struct PrintListNode *DAT_0066b5a4;
extern unsigned int DAT_0066b5a8;
extern unsigned char DAT_007cb600[1];
extern unsigned int DAT_004bdd00;
extern unsigned int DAT_0066b630;
extern unsigned int DAT_00797e68;
extern unsigned int DAT_00701e58;
extern unsigned int DAT_00701e60;
extern unsigned int DAT_0066be48;
extern unsigned int DAT_0066be40;
extern unsigned int DAT_0066be44;
extern unsigned int DAT_0066be4c;
extern void *DAT_00701e5c;
extern unsigned int DAT_007cb5e0;

extern void *_malloc(unsigned int size);
extern void FUN_0049e4d0(void *ptr);

// FUNCTION: LEGOLAND 0x004853a0
void PrintSprite(unsigned int sprite, unsigned int x, unsigned int y, unsigned int param_4, unsigned int param_5) { STUB(); }

// FUNCTION: LEGOLAND 0x004855d0
void FUN_004855d0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004856a0
void PrintSpriteEx(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004858e0
void PrintTiledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485940
void PrintScaledSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004859b0
void ClearPrintList(void)
{
    struct PrintListNode *node = DAT_0066b5a4;

    while (node != NULL) {
        node = node->next;
    }

    DAT_0066b5a8 = 0;
    DAT_0066b5a4 = NULL;
}

// FUNCTION: LEGOLAND 0x004859d0
void DrawAndClearPrintList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485bd0
void FUN_00485bd0(void *ctx) { STUB(); }

// FUNCTION: LEGOLAND 0x00485cd0
void SortSpriteWithCallback(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485d70
void SortSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485e00
void SortPerson(struct Person *person, unsigned int param_2, void *param_3)
{
    unsigned int original = DAT_0066b5a8;
    struct PersonBlock *block;

    DAT_0066b5a8 += 0x20;
    block = (struct PersonBlock *)&DAT_007cb600[original];
    block->field_8 = param_2;
    block->field_c = 0x2000;
    block->field_1c = (unsigned int)person;
    FUN_00485bd0(block);
}

// FUNCTION: LEGOLAND 0x00485e40
void SortClippedSprite(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00485ef0
void ResetHitInfo(void)
{
    DAT_004bdd00 = 0x100;
}

// FUNCTION: LEGOLAND 0x00485f00
void FUN_00485f00(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
    PrintSprite(param_1, param_2, param_3, 0, 0);
}

// FUNCTION: LEGOLAND 0x00485f20
void FUN_00485f20(void *ptr)
{
    DAT_0066b630 = (unsigned int)ptr;
}

// FUNCTION: LEGOLAND 0x00485f30
void FUN_00485f30(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    DAT_00797e68 = param_1;
    DAT_00701e58 = param_2;
    DAT_00701e60 = param_3;
    DAT_0066be48 = param_4;
}

// FUNCTION: LEGOLAND 0x00485f60
void FUN_00485f60(void)
{
    DAT_0066be40 = 0x80;
    DAT_0066be44 = 0x78;
    DAT_00701e5c = _malloc(0xf000);
    DAT_0066be4c = DAT_0066be40 * 4;
}

// FUNCTION: LEGOLAND 0x00485fa0
void FUN_00485fa0(void)
{
    if (DAT_00701e5c != NULL) {
        FUN_0049e4d0(DAT_00701e5c);
    }
}

// FUNCTION: LEGOLAND 0x00485fc0
void FUN_00485fc0(unsigned int param_1)
{
    DAT_007cb5e0 = param_1;
    FUN_004860f0();
    FUN_00485f60();
    FUN_00486540();
}

// FUNCTION: LEGOLAND 0x00485fe0
void FUN_00485fe0(void) { STUB(); }
