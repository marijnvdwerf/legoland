#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "legoland.h"

#include "balloonz.h"
#include "binv.h"
#include "bloke.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"

struct BalloonRideObj {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ struct BalloonRide *ride;
};

struct BalloonRide {
    /* 0x00 */ unsigned char pad_0[0xc];
    /* 0x0c */ unsigned int x;
    /* 0x10 */ unsigned int y;
    /* 0x14 */ unsigned int field_14;
    /* 0x18 */ unsigned int field_18;
    /* 0x1c */ unsigned int flags;
    /* 0x20 */ unsigned char pad_20[0x64 - 0x20];
    /* 0x64 */ void *layer;
    /* 0x68 */ unsigned char pad_68[0xcc - 0x68];
    /* 0xcc */ struct BalloonListElem *list;
};

struct BalloonListElem {
    /* 0x00 */ struct BalloonListElem *next;
    /* 0x04 */ unsigned char pad_4[0x8 - 0x4];
    /* 0x08 */ struct Bloke *bloke;
    /* 0x0c */ unsigned short id;
};

struct CursorState {
    unsigned char pad_0[0x3c];
    unsigned int var_3c;
};

// FUNCTION: LEGOLAND 0x0042a7b0
void FUN_0042a7b0(struct BalloonRideObj *param_1) {
    DAT_0081cde4 = param_1->ride;
    ((struct BalloonRide *)DAT_0081cde4)->flags |= 0x420;
    DAT_00616044 = ((struct BalloonRide *)DAT_0081cde4)->layer;
    ((struct BalloonRide *)DAT_0081cde4)->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b64ac
    DAT_00616048 = LoadSprite("Ballbasem1.lls", 1);
    // STRING: LEGOLAND 0x004b649c
    DAT_0061604c = LoadSprite("Ballbasem2.lls", 1);
    // STRING: LEGOLAND 0x004b648c
    DAT_00616050 = LoadSprite("Ballbasem3.lls", 1);
    // STRING: LEGOLAND 0x004b647c
    DAT_00616054 = LoadSprite("BZRedCarM1.lls", 1);
    // STRING: LEGOLAND 0x004b6468
    DAT_00616058 = LoadSprite("BZGreenCarM1.lls", 1);
    // STRING: LEGOLAND 0x004b6458
    DAT_0061605c = LoadSprite("BZBlueCarM1.lls", 1);
    // STRING: LEGOLAND 0x004b6448
    DAT_00616040 = LoadSprite("z_Balloon2.lls", 1);
    DAT_0081cde8 = DAT_00616040;
    // STRING: LEGOLAND 0x004b6430
    DAT_00616010 = LoadBinV("Zbuffers\\balloonz.bnv");
    DAT_00616018 = DAT_00616010;
    HideLayer(DAT_00616044, 2);
    StopLayerPlaying((unsigned int)DAT_00616044, 2);
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 2), 0);
    HideLayer(DAT_00616044, 1);
    StopLayerPlaying((unsigned int)DAT_00616044, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 1), 0);
}

// FUNCTION: LEGOLAND 0x0042a8f0
void FUN_0042a8f0(unsigned short *param_1) {
    struct BalloonNode *node;
    unsigned int *fill;
    int i;

    node = (struct BalloonNode *)malloc(sizeof(struct BalloonNode));
    if (node != NULL) {
        fill = (unsigned int *)node;
        for (i = 8; i != 0; i--) {
            *fill = 0;
            fill++;
        }
        node->id = *param_1;
        node->next = DAT_00616060;
        *(int *)((char *)node + 8) = 0;
        *(char *)((char *)node + 0xc) = 0;
        *(int *)((char *)node + 0xd) = 0;
        *(short *)((char *)node + 0x11) = 0;
        *(char *)((char *)node + 0x13) = 0;
        *(char *)((char *)node + 0x14) = 0;
        *(char *)((char *)node + 0x15) = 0;
        *(char *)((char *)node + 0x16) = 0;
        *(char *)((char *)node + 0x17) = 0;
        *(int *)((char *)node + 0x18) = 0;
        *(int *)((char *)node + 0x1c) = 0;
        DAT_00616060 = node;
    }
}

// FUNCTION: LEGOLAND 0x0042a950
void FUN_0042a950(unsigned int param_1, unsigned char *param_2) {
    unsigned char *src = param_2;
    unsigned char b0 = param_2[0];
    unsigned char b4 = param_2[4];

    *(unsigned char *)&param_2 = b0;
    *((unsigned char *)&param_2 + 1) = b4;
    AddBasicObject(param_1, (unsigned int)src);
    FUN_0042a8f0((unsigned short *)&param_2);
}

// FUNCTION: LEGOLAND 0x0042a980
struct BalloonNode *FUN_0042a980(unsigned short *param_1) {
    struct BalloonNode *node;

    if (DAT_00616060 != NULL) {
        node = DAT_00616060;
        if (*param_1 == ((struct BalloonNode *)DAT_00616060)->id) {
            return DAT_00616060;
        }
        while (1) {
            node = node->next;
            if (node == NULL) {
                break;
            }
            if (*param_1 == node->id) {
                return node;
            }
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0042a9b0
void FUN_0042a9b0(struct BalloonNode *param_1) {
    struct BalloonNode *cur;
    struct BalloonNode *prev;

    if (DAT_00616060 == param_1) {
        DAT_00616060 = param_1->next;
        free(param_1);
        return;
    }
    cur = ((struct BalloonNode *)DAT_00616060)->next;
    prev = DAT_00616060;
    while (cur != param_1) {
        prev = prev->next;
        if (prev == NULL) {
            goto done;
        }
        cur = prev->next;
    }
    if (prev != NULL) {
        prev->next = param_1->next;
    }
done:
    free(param_1);
}

// FUNCTION: LEGOLAND 0x0042a9f0
void FUN_0042a9f0(void) {
    void *current_handle;

    current_handle = DAT_00616060;
    if (current_handle == NULL) {
        return;
    }
    do {
        FUN_0042a9b0(current_handle);
        current_handle = DAT_00616060;
    } while (current_handle != NULL);
}

// FUNCTION: LEGOLAND 0x0042aa10
void FUN_0042aa10(struct BalloonRideObj *param_1, unsigned int param_2, unsigned int param_3) {
    struct BalloonNode *node;

    node = FUN_0042a980((unsigned short *)&param_2);
    if (node != NULL) {
        FUN_0042a9b0(node);
    }
    StandardRemoveObject((unsigned int)param_1, param_2, param_3);
    RemoveAllBlokesFromRide((unsigned int)param_1->ride, (void *)param_2);
}

// FUNCTION: LEGOLAND 0x0042aa60
int FUN_0042aa60(char param_1, char param_2) {
    if (param_2 == '\x01' && '\x17' < param_1) {
        return 0;
    }
    return (int)param_1 / 8 + param_2 * 3;
}

// FUNCTION: LEGOLAND 0x0042aa90
void FUN_0042aa90(struct BalloonRideObj *param_1) {
    struct BalloonRide *ride;
    struct BalloonListElem *elem;
    struct BalloonListElem *nextNode;
    struct BalloonNode *node;
    struct BalloonNode *aiNode;
    int blokepos;
    char cVar9;
    char cVar15;
    char cVar1;
    char cVar11;
    char cVar10;
    unsigned char bVar8;
    int iVar4;
    int iVar5;
    int iVar16;
    int iVar18;
    int local_2c;
    unsigned int uVar13;
    int bVar6;
    int randv;
    char local_32;
    unsigned int local_10;
    unsigned short local_c;
    // STRING: LEGOLAND 0x004b64bc
    char buf[8] = "Bloke??";

    ride = param_1->ride;
    elem = ride->list;
    while (1) {
        node = DAT_00616060;
        if (elem == NULL) {
            for (; node != NULL; node = node->next) {
                blokepos = *(int *)((char *)node + 8);
                cVar9 = *(char *)((char *)node + 0xc);
                cVar15 = *(char *)((char *)node + 0x14);
                iVar5 = *(int *)((char *)node + 0x1c);
                local_10 = *(unsigned int *)((char *)node + 0xd);
                bVar6 = 0;
                cVar1 = *(char *)((char *)node + 0x17);
                local_c = *(unsigned short *)((char *)node + 0x11);
                cVar11 = *(char *)((char *)node + 0x13);
                uVar13 = (int)cVar15 & 0x80000007;
                iVar4 = 0;
                if (((int)uVar13 < 0 ? (uVar13 - 1 | 0xfffffff8) == 0xffffffff : uVar13 == 0) ||
                    cVar15 == '\0') {
                    cVar10 = (char)FUN_0042aa60(cVar15, cVar11);
                    if (blokepos != 0 && cVar9 < '\x06' &&
                        *((char *)&local_10 + (int)cVar10) == '\0' && rand() % 3 == 0) {
                        iVar4 = 1;
                        bVar6 = 1;
                    }
                    if (*((char *)&local_10 + (int)cVar10) == '\x01') {
                        bVar6 = 1;
                    }
                    if (cVar1 != '\0' && *((char *)&local_10 + (int)cVar10) == '\x03') {
                        iVar5 = 1;
                        bVar6 = 1;
                    }
                }
                if (cVar9 != '\0' && bVar6 == 0) {
                    cVar15 = cVar15 + '\x01';
                    if ('\x17' < cVar15) {
                        cVar15 = '\0';
                        cVar11 = cVar11 + '\x01';
                        if ('\x01' < cVar11) {
                            cVar11 = '\0';
                        }
                    }
                    iVar5 = 0;
                    iVar4 = 0;
                }
                *(char *)((char *)node + 0xc) = cVar9;
                *(unsigned int *)((char *)node + 0xd) = local_10;
                *(int *)((char *)node + 8) = blokepos;
                *(unsigned short *)((char *)node + 0x11) = local_c;
                *(char *)((char *)node + 0x13) = cVar11;
                *(char *)((char *)node + 0x14) = cVar15;
                *(char *)((char *)node + 0x17) = cVar1;
                *(int *)((char *)node + 0x18) = iVar4;
                *(int *)((char *)node + 0x1c) = iVar5;
            }
            return;
        }
        nextNode = elem->next;
        blokepos = (int)elem->bloke;
        aiNode = FUN_0042a980(&elem->id);
        if (aiNode == NULL) {
            return;
        }
        cVar9 = *(char *)((int)aiNode + 0xc);
        local_2c = *(int *)((int)aiNode + 8);
        local_10 = *(unsigned int *)((int)aiNode + 0xd);
        cVar15 = *(char *)((int)aiNode + 0x14);
        local_c = *(unsigned short *)((int)aiNode + 0x11);
        cVar1 = *(char *)((int)aiNode + 0x13);
        iVar4 = *(int *)((int)aiNode + 0x1c);
        local_32 = *(char *)((int)aiNode + 0x17);
        iVar5 = *(int *)((int)aiNode + 0x18);
        *(short *)**(int **)((char *)DAT_0081cde8 + 8) = (short)*(char *)((int)aiNode + 0x15);
        iVar18 = ride->x + (unsigned int)*(unsigned char *)((int)elem + 0xc);
        iVar16 = (unsigned int)*(unsigned char *)((int)&elem->id + 1) + ride->y;
        if (*(short *)(blokepos + 0xe) == 0) {
            switch (*(unsigned char *)(blokepos + 0x60)) {
            case 0:
                *(unsigned char *)(blokepos + 0x62) |= 8;
                iVar16 = iVar16 * 0x100 + 0xfa;
                iVar18 = iVar18 * 0x100 + -0x9c;
                *(int *)(blokepos + 0x24) = iVar18;
                *(int *)(blokepos + 0x28) = iVar16;
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(unsigned int *)(blokepos + 0x5c) = 500;
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 1:
                *(int *)(blokepos + 0x24) = iVar18 * 0x100 + -0x100;
                iVar16 = (iVar16 + 7) * 0x100;
                *(int *)(blokepos + 0x28) = iVar16;
                iVar18 = *(int *)(blokepos + 0x24);
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 2:
                *(int *)(blokepos + 0x24) = (iVar18 + -2) * 0x100;
                iVar16 = iVar16 + 7;
                iVar16 = iVar16 << 8;
                *(int *)(blokepos + 0x28) = iVar16;
                iVar18 = *(int *)(blokepos + 0x24);
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 3:
                iVar18 = (iVar18 + -2) * 0x100;
                iVar16 = (iVar16 + 1) * 0x100;
                *(int *)(blokepos + 0x24) = iVar18;
                *(int *)(blokepos + 0x28) = iVar16;
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 4:
                iVar18 = (iVar18 + -4) * 0x100;
                iVar16 = (iVar16 + 1) * 0x100;
                *(int *)(blokepos + 0x24) = iVar18;
                *(int *)(blokepos + 0x28) = iVar16;
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 5:
                *(int *)(blokepos + 0x24) = (iVar18 + -6) * 0x100;
                iVar16 = iVar16 + 2;
                iVar16 = iVar16 << 8;
                *(int *)(blokepos + 0x28) = iVar16;
                iVar18 = *(int *)(blokepos + 0x24);
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 6:
                iVar16 = (iVar16 + 3) * 0x100;
                iVar18 = iVar18 * 0x100 + -0x632;
                *(int *)(blokepos + 0x24) = iVar18;
                *(int *)(blokepos + 0x28) = iVar16;
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                local_2c = local_2c + 1;
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 7:
                *(unsigned char *)(blokepos + 0x72) = 3;
                if (iVar5 == 1) {
                    iVar16 = (iVar16 + 3) * 0x100;
                    iVar18 = iVar18 * 0x100 + -0x564;
                    *(int *)(blokepos + 0x24) = iVar18;
                    *(int *)(blokepos + 0x28) = iVar16;
                    cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                    *(short *)(blokepos + 0xe) = 7;
                    *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                    NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                    bVar8 = (unsigned char)FUN_0042aa60(cVar15, cVar1);
                    *(unsigned char *)(blokepos + 0x36) = bVar8;
                    *((char *)&local_10 + (unsigned int)bVar8) = 1;
                    randv = rand();
                    *(int *)(blokepos + 0x58) = (randv % 3 + 4) * 0x32;
                    cVar9 = cVar9 + '\x01';
                    local_2c = local_2c + -1;
                    *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                }
                break;
            case 8:
                *(int *)(blokepos + 0x24) = (iVar18 + -5) * 0x100;
                iVar16 = iVar16 * 0x100 + 0x26a;
                *(int *)(blokepos + 0x28) = iVar16;
                iVar18 = *(int *)(blokepos + 0x24);
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 9:
                *(unsigned char *)(blokepos + 0x62) |= 0x80;
                *(struct Sprite **)(*(int *)(blokepos + 4) + 0x2c) = DAT_00616040;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 1;
                *(float *)(*(int *)(blokepos + 4) + 0x3c) = (float)GetUnitDepth(0xc9c578e3, 0xc9c57f82);
                // STRING: LEGOLAND 0x004b4704
                sprintf(&buf[1], "%02d", *(unsigned char *)(blokepos + 0x36));
                SetBlokePositionFromBNV(DAT_00616010, blokepos, buf, 0, 0xc9c578e3, 0xc9c57f82, 0);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 10:
                bVar8 = *(unsigned char *)(blokepos + 0x36);
                *((char *)&local_10 + (unsigned int)bVar8) = 2;
                sprintf(&buf[1], "%02d", (unsigned int)bVar8);
                SetBlokePositionFromBNV(DAT_00616010, blokepos, buf, (int)cVar15 + cVar1 * 0x18, 0xc9c578e3, 0xc9c57f82, 0);
                iVar16 = *(int *)(blokepos + 0x58) + -1;
                *(int *)(blokepos + 0x58) = iVar16;
                if (iVar16 == 0) {
                    *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                }
                break;
            case 0xb:
                sprintf(&buf[1], "%02d", *(unsigned char *)(blokepos + 0x36));
                SetBlokePositionFromBNV(DAT_00616010, blokepos, buf, (int)cVar15 + cVar1 * 0x18, 0xc9c578e3, 0xc9c57f82, 0);
                local_32 = local_32 + '\x01';
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                *((char *)&local_10 + (unsigned int)*(unsigned char *)(blokepos + 0x36)) = 3;
                break;
            case 0xc:
                sprintf(&buf[1], "%02d", *(unsigned char *)(blokepos + 0x36));
                SetBlokePositionFromBNV(DAT_00616010, blokepos, buf, (int)cVar15 + cVar1 * 0x18, 0xc9c578e3, 0xc9c57f82, 0);
                if (iVar4 == 1) {
                    cVar11 = (char)FUN_0042aa60(cVar15, cVar1);
                    if ((int)cVar11 == (unsigned int)*(unsigned char *)(blokepos + 0x36)) {
                        *(unsigned short *)(blokepos + 0x62) &= 0xff7f;
                        iVar16 = (iVar16 + 3) * 0x100;
                        iVar18 = iVar18 * 0x100 + -0x564;
                        *(int *)(blokepos + 0x24) = iVar18;
                        *(int *)(blokepos + 0x28) = iVar16;
                        cVar10 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                        *(unsigned char *)(blokepos + 0x73) = cVar10 + '\x10';
                        *(short *)(blokepos + 0xe) = 7;
                        *(unsigned int *)(*(int *)(blokepos + 4) + 0x2c) = 0;
                        *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 0;
                        NewDirForAction(blokepos, (*(unsigned char *)(blokepos + 0x73) >> 5) + 3);
                        *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                        cVar9 = cVar9 + -1;
                        *((char *)&local_10 + (int)cVar11) = 1;
                    }
                }
                break;
            case 0xd:
                iVar16 = iVar16 + 3;
                *(int *)(blokepos + 0x24) = iVar18 * 0x100 + -0x632;
                iVar16 = iVar16 << 8;
                *(int *)(blokepos + 0x28) = iVar16;
                iVar18 = *(int *)(blokepos + 0x24);
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 0xe:
                iVar16 = (iVar16 + 9) * 0x100;
                iVar18 = (iVar18 + -5) * 0x100;
                *(int *)(blokepos + 0x24) = iVar18;
                *(int *)(blokepos + 0x28) = iVar16;
                cVar11 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar18, iVar16, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar11 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar11 + 0x10) >> 5) + 3);
                cVar11 = *(char *)(blokepos + 0x60);
                *((char *)&local_10 + (unsigned int)*(unsigned char *)(blokepos + 0x36)) = 0;
                local_32 = local_32 + -1;
                *(char *)(blokepos + 0x60) = cVar11 + '\x01';
                break;
            case 0xf:
                RemoveBlokeFromRide(ride, elem);
                *(unsigned short *)(blokepos + 0x62) &= 0xfff7;
                break;
            }
        }
        *(char *)((int)aiNode + 0xc) = cVar9;
        *(int *)((int)aiNode + 8) = local_2c;
        *(char *)((int)aiNode + 0x13) = cVar1;
        *(unsigned int *)((int)aiNode + 0xd) = local_10;
        *(int *)((int)aiNode + 0x18) = iVar5;
        *(unsigned short *)((int)aiNode + 0x11) = local_c;
        *(char *)((int)aiNode + 0x15) = cVar15;
        *(char *)((int)aiNode + 0x17) = local_32;
        *(int *)((int)aiNode + 0x1c) = iVar4;
        elem = nextNode;
    }
}

// FUNCTION: LEGOLAND 0x0042b2a0
unsigned int *FUN_0042b2a0(struct BalloonRideObj *arg1, unsigned short arg2) {
    struct BalloonRide *ride = arg1->ride;

    DAT_00616028 = (unsigned int)ride->layer;
    DAT_0061602c = ride->field_14;
    DAT_00616030 = ride->field_18;
    DAT_00616034 = arg2;
    *(unsigned int *)((char *)ride->layer + 0x10) |= 0x2000;
    return &DAT_00616028;
}

// FUNCTION: LEGOLAND 0x0042b2e0
void FUN_0042b2e0(struct BalloonRideObj *param_1, void *param_2, void *param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6) {
    struct BalloonRide *ride = param_1->ride;
    struct BalloonListElem *elem;
    struct BalloonListElem *list2;
    int blokes[6];
    char count;
    char cVar1;
    char cVar4;
    int local_4c;
    int local_44;
    __int64 local_40;
    __int64 local_38;
    struct LayerResult local_18;
    int *p;
    int i;
    int iVar6, iVar7;
    unsigned int sprcase;
    unsigned int uVar10;
    unsigned int uVar8;
    int bVar13;

    blokes[1] = 0;
    blokes[2] = 0;
    elem = ride->list;
    blokes[3] = 0;
    blokes[4] = 0;
    count = 0;
    blokes[0] = 0;
    blokes[5] = 0;
    AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
    local_44 = (int)FUN_0042a980(param_4);
    if (local_44 == 0) {
        return;
    }
    cVar1 = *(char *)(local_44 + 0x15);
    cVar4 = *(char *)(local_44 + 0x16);
    local_4c = *(unsigned char *)(local_44 + 0x13);
    local_38 = GetScreenCoordsForObject((unsigned char *)param_4, ride);
    GetLayer((struct LayerOwner *)ride->layer, &local_18, 1);
    if (elem != NULL) {
        short id = *param_4;
        do {
            if (id == (short)elem->id) {
                blokes[count] = (int)elem->bloke;
                count++;
            }
            elem = elem->next;
        } while (elem != NULL);
        if ((char)count != '\0') {
            list2 = ride->list;
            local_40 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616044, 0);
            AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
            if ('\0' < (char)count) {
                p = blokes;
                i = count;
                do {
                    if (*(char *)(*p + 0x60) == '\x06') {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p++;
                    i--;
                } while (i != 0);
                if ('\0' < (char)count) {
                    p = blokes;
                    i = count;
                    do {
                        if (*(char *)(*p + 0x60) == '\x05') {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p++;
                        i--;
                    } while (i != 0);
                }
            }
            iVar6 = (int)local_38;
            iVar7 = (int)(local_38 >> 0x20);
            PrintSprite(DAT_00616048, (int)local_40 + iVar6, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
            if ('\0' < (char)count) {
                p = blokes;
                i = count;
                do {
                    if (*(char *)(*p + 0x60) == '\x04') {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p++;
                    i--;
                } while (i != 0);
            }
            PrintSprite(DAT_0061604c, (int)local_40 + iVar6, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
            if ('\0' < (char)count) {
                p = blokes;
                i = count;
                do {
                    if (*(char *)(*p + 0x60) == '\0') {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p++;
                    i--;
                } while (i != 0);
                if ('\0' < (char)count) {
                    p = blokes;
                    i = count;
                    do {
                        if (*(char *)(*p + 0x60) == '\x01') {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p++;
                        i--;
                    } while (i != 0);
                    if ('\0' < (char)count) {
                        p = blokes;
                        i = count;
                        do {
                            if (*(char *)(*p + 0x60) == '\x02') {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p++;
                            i--;
                        } while (i != 0);
                        if ('\0' < (char)count) {
                            p = blokes;
                            i = count;
                            do {
                                if (*(char *)(*p + 0x60) == '\x03') {
                                    IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                }
                                p++;
                                i--;
                            } while (i != 0);
                        }
                    }
                }
            }
            PrintSprite(DAT_00616050, (int)local_40 + iVar6, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
            if ('\0' < (char)count) {
                p = blokes;
                i = count;
                do {
                    if (*(char *)(*p + 0x60) == '\x07') {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p++;
                    i--;
                } while (i != 0);
                if ('\0' < (char)count) {
                    p = blokes;
                    i = count;
                    do {
                        if (*(char *)(*p + 0x60) == '\x0e') {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p++;
                        i--;
                    } while (i != 0);
                    if ('\0' < (char)count) {
                        p = blokes;
                        i = count;
                        do {
                            if (*(char *)(*p + 0x60) == '\x0f') {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p++;
                            i--;
                        } while (i != 0);
                    }
                }
            }
            uVar10 = (unsigned int)cVar1;
            LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 1), uVar10);
            local_40 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616044, 1);
            AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
            PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616044, 1), (int)local_40 + iVar6, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
            uVar8 = uVar10 & 0x80000007;
            bVar13 = uVar8 == 0;
            if ((int)uVar8 < 0) {
                bVar13 = (uVar8 - 1 | 0xfffffff8) == 0xffffffff;
            }
            if (bVar13 || cVar1 == '\0') {
                LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 1), uVar10);
                local_40 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616044, 1);
                AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
                if ('\0' < (char)count) {
                    p = blokes;
                    i = count;
                    do {
                        if (*(char *)(*p + 0x60) == '\x08') {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p++;
                        i--;
                    } while (i != 0);
                    if ('\0' < (char)count) {
                        i = count;
                        p = blokes;
                        do {
                            if (*(char *)(*p + 0x60) == '\x09') {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p++;
                            i--;
                        } while (i != 0);
                        if ('\0' < (char)count) {
                            i = count;
                            p = blokes;
                            do {
                                if (*(char *)(*p + 0x60) == '\x0d') {
                                    IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                }
                                p++;
                                i--;
                            } while (i != 0);
                            if ('\0' < (char)count) {
                                p = blokes;
                                i = count;
                                do {
                                    if (*(char *)(*p + 0x60) == '\x0e') {
                                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                    }
                                    p++;
                                    i--;
                                } while (i != 0);
                            }
                        }
                    }
                }
                sprcase = FUN_0042aa60(cVar1, (char)local_4c);
                switch (sprcase) {
                case 0:
                case 3:
                    PrintSprite(DAT_00616054, (int)local_40 + (int)local_38, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
                    break;
                case 1:
                case 4:
                    PrintSprite(DAT_00616058, (int)local_40 + (int)local_38, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
                    break;
                case 2:
                case 5:
                    PrintSprite(DAT_0061605c, (int)local_40 + (int)local_38, (int)(local_40 >> 0x20) + iVar7, param_6, 0);
                    break;
                }
            }
            for (; list2 != NULL; list2 = list2->next) {
                int b;
                if (*param_4 == (short)list2->id && (b = (int)list2->bloke, (*(unsigned char *)(b + 0x62) & 0x80) != 0)) {
                    int unit;
                    int local_54, local_50;
                    local_54 = (int)local_18.field_4 + 0xc;
                    local_50 = (int)local_18.field_8 + -6;
                    local_4c = 0x12;
                    local_44 = 0;
                    AdjustOffsetForViewMode((struct AdjustStruct *)&local_4c);
                    local_44 = local_44 + -8;
                    unit = *(int *)(b + 4);
                    *(int *)(unit + 0x24) = *(short *)(b + 0x3c) - local_4c;
                    *(int *)(unit + 0x28) = *(short *)(b + 0x3e) - local_44;
                    AdjustBlokePosition((struct BlokePos *)(unit + 0x24));
                    AdjustOffsetForViewMode((struct AdjustStruct *)&local_54);
                    *(int *)(unit + 0x1c) = local_54 + (int)local_38 + (*(short *)(b + 0x3c) - local_4c);
                    *(int *)(unit + 0x20) = (*(short *)(b + 0x3e) - local_44) + local_50 + (int)(local_38 >> 0x20);
                    AdjustBlokePosition((struct BlokePos *)(unit + 0x1c));
                    IP_RenderBlokeIn3DNow(list2->bloke);
                }
            }
            goto draw_layer2;
        }
    }
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 1), (int)cVar1);
    local_40 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616044, 1);
    AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
    PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616044, 1), (int)local_40 + (int)local_38, (int)(local_40 >> 0x20) + (int)(local_38 >> 0x20), param_6, 0);
draw_layer2:
    cVar4 = cVar4 + '\x01';
    if ('0' < cVar4) {
        cVar4 = '\0';
    }
    *(char *)(local_44 + 0x16) = cVar4;
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616044, 2), (int)cVar4);
    local_40 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616044, 2);
    AdjustOffsetForViewMode((struct AdjustStruct *)&local_40);
    PrintSprite((struct Sprite *)GetSpriteForLayer((struct LayerContainer *)DAT_00616044, 2), (int)local_40 + (int)local_38, (int)(local_40 >> 0x20) + (int)(local_38 >> 0x20), param_6, 0);
}

// FUNCTION: LEGOLAND 0x0042b9d0
void FUN_0042b9d0(void) {
    KillSprite(DAT_00616048);
    KillSprite(DAT_0061604c);
    KillSprite(DAT_00616050);
    KillSprite(DAT_00616054);
    KillSprite(DAT_00616058);
    KillSprite(DAT_0061605c);
    KillSprite(DAT_0081cde8);
    FreeBinV(DAT_00616018);
    FUN_0042a9f0();
}

// FUNCTION: LEGOLAND 0x0042ba40
void FUN_0042ba40(void) {
    struct CursorState *temp;

    temp = DAT_0081cde4;
    EditMode.unk0 = 1;
    EditMode.unk8 = temp;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint(&((struct CursorState *)EditMode.unk8)->var_3c);
}

// FUNCTION: LEGOLAND 0x0042ba80
unsigned int FUN_0042ba80(void) {
    unsigned int marker;
    unsigned int terminator;
    struct BalloonNode *current;

    marker = 1;
    terminator = 0;
    current = (struct BalloonNode *)DAT_00616060;
    while (current != NULL) {
        if (SaveGameWrite(&marker, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(current, 0x20) == 0) {
            return 0;
        }
        current = current->next;
    }
    if (SaveGameWrite(&terminator, 4) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042baf0
unsigned int FUN_0042baf0(struct BalloonRideObj *param_1) {
    struct BalloonRide *ride = param_1->ride;
    struct BalloonListElem *elem;
    struct BalloonNode *node;
    struct BalloonNode *prev;
    int comp;
    unsigned int *h;

    if (SaveGameRead(&param_1, 4) == 0) {
        return 0;
    }
    prev = NULL;
    while (param_1 != NULL) {
        node = (struct BalloonNode *)malloc(sizeof(struct BalloonNode));
        if (SaveGameRead(node, 0x20) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_00616060 = node;
        }
        if (SaveGameRead(&param_1, 4) == 0) {
            return 0;
        }
        prev = node;
    }
    for (elem = ride->list; elem != NULL; elem = elem->next) {
        comp = *(int *)((char *)elem + 0x10);
        if (*(int *)(comp + 0x30) != 0) {
            *(unsigned int *)(comp + 0x2c) = (&DAT_0061603c)[*(int *)(comp + 0x30)];
        } else {
            *(unsigned int *)(comp + 0x2c) = 0;
            *(unsigned int *)(*(int *)((char *)elem + 0x10) + 0x30) = 0;
        }
        h = *(unsigned int **)((char *)elem->bloke + 0x54);
        if (h != NULL) {
            *h = ((unsigned int *)&DAT_00616018)[h[1]];
        }
    }
    return 1;
}
