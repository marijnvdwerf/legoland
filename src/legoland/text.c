#include "legoland.h"
#include <windows.h>

#include "print_sprite.h"
#include "text.h"
#include "llidb.h"

struct TextSprite {
    unsigned char pad_0[0x1c];
    unsigned int field_1c;
};

extern unsigned int DAT_006675b4;
extern void *DAT_00813a0c;
extern void *DAT_008139e0;
extern void *DAT_008139e4;
extern void *DAT_008139e8;
extern void *DAT_008139ec;
extern void *DAT_008139f0;
extern void *DAT_008139f4;
extern void *DAT_008139f8;
extern void *DAT_008139fc;
extern void *DAT_00813a00;
extern void *DAT_00813a04;
extern void *DAT_00813a08;
extern void *PTR_0066808c;
extern void *PTR_00668090;
extern void *PTR_00668094;
extern void *PTR_00668098;

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00454910
void LoadBubbleHelpGFX(void) {
    void *element;

    if (DAT_006675b4 != 0) {
        return;
    }

    // STRING: LEGOLAND 0x004b9064
    if (LLIDB_FindElement("SPEECH BUBBLE", (unsigned int *)&element, 0) == 0) {
        DAT_00813a0c = LLIDB_LoadData(element);
    }

    DAT_008139e0 = 0;
    // STRING: LEGOLAND 0x004b9054
    DAT_008139e4 = LoadSprite("mi_hungry.lls", 0);
    // STRING: LEGOLAND 0x004b9044
    DAT_008139e8 = LoadSprite("mi_happy.lls", 0);
    // STRING: LEGOLAND 0x004b9038
    DAT_008139ec = LoadSprite("mi_sad.lls", 0);
    // STRING: LEGOLAND 0x004b902c
    DAT_008139f0 = LoadSprite("mi_home.lls", 0);
    // STRING: LEGOLAND 0x004b9020
    DAT_008139f4 = LoadSprite("mi_eat.lls", 0);
    // STRING: LEGOLAND 0x004b9014
    DAT_008139f8 = LoadSprite("great.lls", 0);
    // STRING: LEGOLAND 0x004b7a78
    DAT_008139fc = LoadSprite("poor.lls", 0);
    // STRING: LEGOLAND 0x004b9004
    DAT_00813a00 = LoadSprite("favourite.lls", 0);
    // STRING: LEGOLAND 0x004b8ff8
    DAT_00813a04 = LoadSprite("opinion.lls", 0);
    // STRING: LEGOLAND 0x004b8fe8
    DAT_00813a08 = LoadSprite("mi_bored.lls", 0);

    DAT_006675b4 = 1;
}

// FUNCTION: LEGOLAND 0x00454a10
void FUN_00454a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454b40
HGDIOBJ SelectFont(HDC hdc, int font_id) {
    switch (font_id) {
    case 1:
        return SelectObject(hdc, PTR_0066808c);
    case 2:
        return SelectObject(hdc, PTR_00668094);
    case 3:
        return SelectObject(hdc, PTR_00668098);
    default:
        return SelectObject(hdc, PTR_00668090);
    }
}

// FUNCTION: LEGOLAND 0x00454ba0
void Print(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454c70
void PrintLimitedText(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454d80
void FUN_00454d80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454e60
void PrintCent(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00454f60
void PrintCentOpaque(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455060
void PrintCentColref(void) { STUB(); }

// FUNCTION: LEGOLAND 0x004551a0
void FUN_004551a0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455220
void FUN_00455220(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455370
void BubbleHelp(unsigned int *table, unsigned int a2, unsigned int a3) { STUB(); }

// FUNCTION: LEGOLAND 0x004557c0
void HTBubbleHelp(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455a10
void FUN_00455a10(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455a50
void FUN_00455a50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455bb0
void FUN_00455bb0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455c80
void FUN_00455c80(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455d40
void FUN_00455d40(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455de0
void FUN_00455de0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455e50
void FUN_00455e50(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455ec0
void FUN_00455ec0(struct TextSprite *param_0, unsigned int param_1, unsigned int param_2) {
    PrintSprite(param_0->field_1c, param_1, param_2, 0, 0);
}

// FUNCTION: LEGOLAND 0x00455ee0
void FUN_00455ee0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455f70
void FUN_00455f70(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00455fc0
void FUN_00455fc0(void) { STUB(); }
