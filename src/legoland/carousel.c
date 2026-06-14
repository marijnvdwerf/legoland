#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"
#include "bloke.h"
#include "carousel.h"
#include "gamemap.h"
#include "image_sprite.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"
#include "sound_music.h"
#include "tilemap.h"

// FUNCTION: LEGOLAND 0x0042bbc0
void FUN_0042bbc0(unsigned short *param_1) {
    struct CarouselNode *node = (struct CarouselNode *)malloc(sizeof(struct CarouselNode));
    if (node != NULL) {
        unsigned int *fill = (unsigned int *)node;
        int i;
        for (i = 0xb; i != 0; i--) {
            *fill = 0;
            fill++;
        }
        node->id = *param_1;
        node->next = DAT_006160c4;
        DAT_006160c4 = node;
        FUN_0042c210(node);
    }
}

// FUNCTION: LEGOLAND 0x0042bc00
void FUN_0042bc00(struct CarouselNode *node) {
    struct CarouselNode *cur;
    struct CarouselNode *prev;

    if (DAT_006160c4 == node) {
        DAT_006160c4 = node->next;
        free(node);
        return;
    }
    cur = DAT_006160c4->next;
    prev = DAT_006160c4;
    while (cur != node) {
        prev = prev->next;
        if (prev == NULL) {
            goto done;
        }
        cur = prev->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    }
done:
    free(node);
}

// FUNCTION: LEGOLAND 0x0042bc40
void FUN_0042bc40(void) {
    while (DAT_006160c4 != NULL) {
        FUN_0042bc00(DAT_006160c4);
    }
}

// FUNCTION: LEGOLAND 0x0042bc60
struct CarouselNode *FUN_0042bc60(unsigned short *param_1) {
    struct CarouselNode *node;

    if (DAT_006160c4 != NULL) {
        node = DAT_006160c4;
        if (*param_1 == DAT_006160c4->id) {
            return DAT_006160c4;
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

// FUNCTION: LEGOLAND 0x0042bc90
void FUN_0042bc90(struct CarouselNode *node) {
    struct SampleParams params;

    params.field_0 = 2;
    node->field_7 = node->field_6;
    node->flags = node->flags & 0xffffbfff | 1;
    node->field_6 = 0;
    node->field_14 = 0;
    params.field_8 = *(unsigned char *)((char *)node + 4);
    params.field_c = *(unsigned char *)((char *)node + 5);
    node->field_8 = 1;
    PlayInstanceOfSample(*(void **)(CAROUSSEL_SFX + 8), 1, 1, &params);
}

// FUNCTION: LEGOLAND 0x0042c210
void FUN_0042c210(struct CarouselNode *node) {
    unsigned int r;
    struct SampleParams params;

    node->field_14 = 0;
    node->field_8 = 0;
    r = rand();
    r = r & 0x80000001;
    if ((int)r < 0) {
        r = (r - 1 | 0xfffffffe) + 1;
    }
    node->field_18 = 0;
    node->field_10 = (char)r + '\x03';
    node->field_6 = 0;
    params.field_8 = *(unsigned char *)((char *)node + 4);
    params.field_c = *(unsigned char *)((char *)node + 5);
    node->flags = node->flags & 0xffffbffe;
    params.field_0 = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042c280
void FUN_0042c280(struct CarouselRideObj *param_1) {
    struct LayerResult layer;

    DAT_006160bc = param_1->ride;
    // STRING: LEGOLAND 0x004b64d8
    Load_FXList((const unsigned char *)"Carousel.wav", 2);
    DAT_006160bc->flags |= 0x420;
    DAT_00616068 = DAT_006160bc->layer;
    *(unsigned int *)((char *)DAT_00616068 + 0x10) |= 0x2000;
    GetLayer((struct LayerOwner *)DAT_006160bc->layer, &layer, 0);
    DAT_00616078 = (int)layer.field_4 + -0x58;
    DAT_0061607c = (int)layer.field_8 + -0xcd;
    // STRING: LEGOLAND 0x004b65a4
    DAT_006160b8 = LoadSprite("z_Carousel.lls", 1);
    DAT_006160c0 = DAT_006160b8;
    // STRING: LEGOLAND 0x004b658c
    DAT_00616080 = LoadBinV("Zbuffers\\CarouselOn.bnv");
    DAT_00616090 = DAT_00616080;
    // STRING: LEGOLAND 0x004b6574
    DAT_0061608c = LoadBinV("Zbuffers\\Carousel.bnv");
    DAT_00616094 = DAT_0061608c;
    // STRING: LEGOLAND 0x004b6558
    DAT_00616084 = LoadBinV("Zbuffers\\CarouselOff.bnv");
    DAT_00616098 = DAT_00616084;
    // STRING: LEGOLAND 0x004b653c
    DAT_0061606c = LoadSprite("Carousel Entrance Matte.lls", 1);
    // STRING: LEGOLAND 0x004b651c
    DAT_00616070 = LoadSprite("Carousel Entrance Matte2.lls", 1);
    HideLayer(DAT_00616068, 2);
    StopLayerPlaying((unsigned int)DAT_00616068, 2);
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616068, 2), 0);
    HideLayer(DAT_00616068, 0);
    StopLayerPlaying((unsigned int)DAT_00616068, 0);
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_00616068, 0), 0);
    HideLayer(DAT_00616068, 1);
}

// FUNCTION: LEGOLAND 0x0042c3f0
void FUN_0042c3f0(struct CarouselRideObj *input) {
    DAT_006160bc = input->ride;
    KillSprite(DAT_0061606c);
    KillSprite(DAT_00616070);
    KillSprite(DAT_006160c0);
    FUN_0042bc40();
    Kill_FXList(CAROUSSEL_SFX, 2);
    FreeBinV(DAT_00616090);
    FreeBinV(DAT_00616094);
    FreeBinV(DAT_00616098);
}

// FUNCTION: LEGOLAND 0x0042c460
void FUN_0042c460(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_006160bc;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x0042c4a0
void FUN_0042c4a0(struct CarouselRideObj *param_1, unsigned int param_2, unsigned int param_3) {
    struct CarouselNode *node;
    struct SampleParams params;

    node = FUN_0042bc60((unsigned short *)&param_2);
    if (node != NULL) {
        FUN_0042bc00(node);
    }
    StandardRemoveObject((unsigned int)param_1, param_2, param_3);
    RemoveAllBlokesFromRide((unsigned int)param_1->ride, param_2);
    params.field_8 = param_2 & 0xff;
    params.field_c = *((unsigned char *)&param_2 + 1);
    params.field_0 = 2;
    UnSourceAndFadeAllSamplesFromSource(&params, 0xffffff38);
}

// FUNCTION: LEGOLAND 0x0042c520
void FUN_0042c520(unsigned int param_1, unsigned char *param_2) {
    unsigned char *src = param_2;
    unsigned char b0 = param_2[0];
    unsigned char b4 = param_2[4];

    *(unsigned char *)&param_2 = b0;
    *((unsigned char *)&param_2 + 1) = b4;
    AddBasicObject(param_1, (unsigned int)src);
    FUN_0042bbc0((unsigned short *)&param_2);
}

// FUNCTION: LEGOLAND 0x0042c550
unsigned int *FUN_0042c550(struct CarouselRideObj *param1, unsigned short param2) {
    struct CarouselRide *ride = param1->ride;

    DAT_006160a0 = (unsigned int)ride->layer;
    DAT_006160a4 = *(unsigned int *)((char *)ride + 0x14);
    DAT_006160a8 = *(unsigned int *)((char *)ride + 0x18);
    DAT_006160ac = param2;
    *(unsigned int *)((char *)ride->layer + 0x10) |= 0x2000;
    return &DAT_006160a0;
}

// FUNCTION: LEGOLAND 0x0042c590
int FUN_0042c590(void) {
    struct CarouselNode *current = DAT_006160c4;
    unsigned int flag = 1;
    unsigned int terminator = 0;

    while (current != NULL) {
        if (!SaveGameWrite(&flag, 4)) {
            return 0;
        }
        if (!SaveGameWrite(current, 0x2c)) {
            return 0;
        }
        current = current->next;
    }

    if (SaveGameWrite(&terminator, 4)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0042c600
int FUN_0042c600(struct CarouselRideObj *param_1) {
    struct CarouselRide *ride = param_1->ride;
    struct CarouselNode *node;
    struct CarouselNode *prev;
    struct CarouselListElem *elem;
    unsigned int marker;

    prev = NULL;
    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != 0) {
        node = (struct CarouselNode *)malloc(sizeof(struct CarouselNode));
        if (SaveGameRead(node, 0x2c) == 0) {
            return 0;
        }
        node->next = NULL;
        if (prev != NULL) {
            prev->next = node;
        } else {
            DAT_006160c4 = node;
        }
        prev = node;
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
    }
    for (elem = ride->list; elem != NULL; elem = elem->next) {
        unsigned int *comp = *(unsigned int **)((char *)elem + 0x10);
        if (comp[0xc] != 0) {
            comp[0xb] = ((unsigned int *)&DAT_006160bc)[comp[0xc]];
        } else {
            comp[0xb] = 0;
            (*(unsigned int **)((char *)elem + 0x10))[0xc] = 0;
        }
        {
            unsigned int *h = *(unsigned int **)((char *)elem->bloke + 0x54);
            if (h != NULL) {
                *h = ((unsigned int *)&DAT_00616090)[h[1]];
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0042c6d0
void FUN_0042c6d0(struct CarouselNode *node) {
    struct CarouselListElem *elem = DAT_006160bc->list;
    unsigned int flags = node->flags;

    if ((flags & 1) == 0) {
        if ((flags & 0x4000) == 0) {
            if (node->field_6 != 0) {
                if (node->field_1c == 0) {
                    node->flags = flags | 0x4000;
                    Ride_SetFlagToNotLetAnyoneOn((unsigned char *)&node->id);
                } else {
                    node->field_1c = node->field_1c - 1;
                }
            }
        } else if ((char)node->field_6 == (char)node->field_18) {
            node->flags = flags & 0xffffbfff;
            FUN_0042bc90(node);
            return;
        }
    } else {
        int v = node->field_14 + 1;
        node->field_14 = v;
        if (node->field_10 == 0) {
            v = GetAllBlokesOffRide((struct Ride *)DAT_006160bc, node->id);
            if (v == 0) {
                return;
            }
            FUN_0042c210(node);
            return;
        }
        if (1 < v) {
            char cVar4;
            node->field_14 = 0;
            cVar4 = node->field_8 + '\x01';
            node->field_8 = cVar4;
            if ('?' < cVar4) {
                node->field_8 = 0;
                node->field_10 = node->field_10 - 1;
            }
        }
    }
    for (; elem != NULL; elem = elem->next) {
        if (node->id == elem->id && *(char *)((char *)elem->bloke + 0x35) == '\x01') {
            // STRING: LEGOLAND 0x004b4704
            sprintf(DAT_004b64d4, "%02d", *(unsigned char *)((char *)elem->bloke + 0x36));
            // STRING: LEGOLAND 0x004b64cc
            SetBlokePositionFromBNV(DAT_0061608c, elem->bloke, "BlokeBox??", (int)(char)node->field_8, 0xc9c57dea, 0xc9c585e8, 0);
        }
    }
    *(short *)**(int **)((char *)DAT_006160b8 + 8) = (short)(char)node->field_8;
}

// FUNCTION: LEGOLAND 0x0042c800
void FUN_0042c800(void) {
    struct CarouselNode *current = DAT_006160c4;

    while (current != NULL) {
        FUN_0042c6d0(current);
        current = current->next;
    }
}

// FUNCTION: LEGOLAND 0x0042c820
void FUN_0042c820(struct CarouselRideObj *param_1) {
    struct CarouselRide *ride = param_1->ride;
    struct CarouselListElem *elem = ride->list;
    struct CarouselListElem *next;
    int bloke;
    int blokepos;
    unsigned char *pos;
    int iVar12, iVar13;
    char cVar7;
    int local_30, local_2c;
    int local_18, local_14;
    int local_c, local_8;
    __int64 coords;

    while (elem != NULL) {
        next = elem->next;
        blokepos = (int)elem->bloke;
        pos = (unsigned char *)&elem->id;
        bloke = (int)FUN_0042bc60((unsigned short *)pos);
        if (bloke == 0) {
            break;
        }
        iVar12 = *(int *)((char *)ride + 0xc) + (unsigned int)*pos;
        iVar13 = (unsigned int)pos[1] + *(int *)((char *)ride + 0x10);
        if (*(short *)(blokepos + 0xe) == 0) {
            switch (*(unsigned char *)(blokepos + 0x60)) {
            case 0:
                iVar13 = iVar13 * 0x100 + 0x80;
                *(char *)(bloke + 0x18) = *(char *)(bloke + 0x18) + '\x01';
                iVar12 = (iVar12 + -3) * 0x100;
                *(unsigned int *)(blokepos + 0x1c) = 0xb4;
                *(unsigned char *)(blokepos + 0x62) |= 8;
                *(int *)(blokepos + 0x24) = iVar12;
                *(int *)(blokepos + 0x28) = iVar13;
                cVar7 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar12, iVar13, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar7 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar7 + 0x10) >> 5) + 3);
                *(unsigned int *)(blokepos + 0x58) = 0;
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 1:
                coords = GetScreenCoordsForObject(pos, ride);
                {
                    int iVar10 = *(int *)(blokepos + 0x6c);
                    int iVar10b = *(int *)(blokepos + 0x68);
                    short sVar8, sVar9;
                    GetTileDimensions(&local_30, &local_2c);
                    iVar13 = (iVar10b + iVar10) * local_2c;
                    iVar12 = (iVar10b - iVar10) * local_30;
                    sVar8 = Get_XScroll();
                    sVar9 = Get_YScroll();
                    local_18 = (((((unsigned int)lpConfig->field_20 - (int)sVar8) + (iVar12 >> 9)) - DAT_00616078 / 2) - (int)coords) * 2;
                    local_14 = ((((iVar13 >> 9) + ((unsigned int)lpConfig->field_22 - (int)sVar9)) - DAT_0061607c / 2) - (int)(coords >> 0x20)) * 2;
                }
                *(struct Sprite **)(*(int *)(blokepos + 4) + 0x2c) = DAT_006160c0;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 1;
                *(float *)(*(int *)(blokepos + 4) + 0x3c) = (float)GetUnitDepth(0xc9c57dea, 0xc9c585e8);
                *(unsigned char *)(blokepos + 0x35) = 0;
                // STRING: LEGOLAND 0x004b4704
                sprintf(DAT_004b64d4, "%02d", FUN_0042cd20(elem, (struct CarouselNode *)bloke, DAT_006160bc->capacity));
                // STRING: LEGOLAND 0x004b64cc
                *(unsigned int *)(blokepos + 0x54) = (unsigned int)NewBNVPath(DAT_00616090, 0, "BlokeBox??", 0xc9c57dea, 0xc9c585e8, &local_18);
                UpdateBlokeFromBNVPath(blokepos, *(unsigned int *)(blokepos + 0x54));
                *(unsigned char *)(blokepos + 0x62) |= 0x80;
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 2:
                if (UpdateBlokeFromBNVPath(blokepos, *(unsigned int *)(blokepos + 0x54)) == 0) {
                    *(unsigned char *)(blokepos + 0x35) = 1;
                    *(unsigned char *)(blokepos + 0x60) = 5;
                    free(*(void **)(blokepos + 0x54));
                    *(unsigned int *)(blokepos + 0x54) = 0;
                }
                BlokeSetFrame(blokepos, *(unsigned char *)(blokepos + 0x74));
                break;
            case 5:
                *(unsigned char *)(blokepos + 0x62) |= 0x80;
                BlokeSetFrame(blokepos, 0);
                *(unsigned char *)(blokepos + 0x35) = 1;
                *(struct Sprite **)(*(int *)(blokepos + 4) + 0x2c) = DAT_006160c0;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 1;
                *(float *)(*(int *)(blokepos + 4) + 0x3c) = (float)GetUnitDepth(0xc9c57dea, 0xc9c585e8);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                cVar7 = *(char *)(bloke + 6) + '\x01';
                *(char *)(bloke + 6) = cVar7;
                if ((short)cVar7 == *(short *)((char *)DAT_006160bc + 0x2e)) {
                    FUN_0042bc90((struct CarouselNode *)bloke);
                }
                break;
            case 7:
                local_c = (int)*(short *)(blokepos + 0x3c) << 1;
                local_8 = (int)*(short *)(blokepos + 0x3e) << 1;
                BlokeWalkAnim((struct Bloke *)blokepos);
                BlokeSetFrame(blokepos, 0);
                *(unsigned char *)(blokepos + 0x62) |= 0x80;
                *(struct Sprite **)(*(int *)(blokepos + 4) + 0x2c) = DAT_006160c0;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 1;
                *(float *)(*(int *)(blokepos + 4) + 0x3c) = (float)GetUnitDepth(0xc9c57dea, 0xc9c585e8);
                *(unsigned char *)(blokepos + 0x35) = 2;
                // STRING: LEGOLAND 0x004b4704
                sprintf(DAT_004b64d4, "%02d", *(unsigned char *)(blokepos + 0x36));
                // STRING: LEGOLAND 0x004b64cc
                *(unsigned int *)(blokepos + 0x54) = (unsigned int)NewBNVPath(DAT_00616098, 2, "BlokeBox??", 0xc9c57dea, 0xc9c585e8, &local_c);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 8:
                if (UpdateBlokeFromBNVPath(blokepos, *(unsigned int *)(blokepos + 0x54)) == 0) {
                    *(unsigned char *)(blokepos + 0x35) = 2;
                    *(unsigned char *)(blokepos + 0x60) = 0xd;
                    free(*(void **)(blokepos + 0x54));
                    *(unsigned int *)(blokepos + 0x54) = 0;
                    *(unsigned char *)(blokepos + 0x72) = 3;
                }
                BlokeSetFrame(blokepos, *(unsigned char *)(blokepos + 0x74));
                break;
            case 0xd:
                *(unsigned char *)(*(unsigned char *)(blokepos + 0x36) + 0x1f + bloke) = 0;
                *(unsigned short *)(blokepos + 0x62) &= 0xff7f;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x2c) = 0;
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x30) = 0;
                UnAdjustBlokePosition(*(int *)(blokepos + 4) + 0x1c);
                ScreenToMapRef(*(int *)(blokepos + 4) + 0x1c, blokepos + 0x68, 0);
                *(unsigned int *)(*(int *)(blokepos + 4) + 0x34) = 0;
                iVar13 = iVar13 * 0x100 + 0x80;
                *(int *)(blokepos + 0x6c) = *(int *)(blokepos + 0x6c) << 8;
                *(int *)(blokepos + 0x68) = *(int *)(blokepos + 0x68) << 8;
                iVar12 = iVar12 * 0x100 + 0x80;
                *(int *)(blokepos + 0x24) = iVar12;
                *(int *)(blokepos + 0x28) = iVar13;
                cVar7 = CalcMoveLine(*(unsigned int *)(blokepos + 0x68), *(unsigned int *)(blokepos + 0x6c), iVar12, iVar13, blokepos + 0x98);
                *(short *)(blokepos + 0xe) = 7;
                *(unsigned char *)(blokepos + 0x73) = cVar7 + 0x10;
                NewDirForAction(blokepos, ((unsigned char)(cVar7 + 0x10) >> 5) + 3);
                *(char *)(blokepos + 0x60) = *(char *)(blokepos + 0x60) + '\x01';
                break;
            case 0xe:
                RemoveBlokeFromRide((void *)((char *)ride), elem);
                *(unsigned short *)(blokepos + 0x62) &= 0xfff7;
                cVar7 = *(char *)(bloke + 7) + -1;
                *(char *)(bloke + 7) = cVar7;
                if (cVar7 == '\0') {
                    *(unsigned char *)(bloke + 6) = 0;
                    Ride_ClearFlagToNotLetAnyoneOn((unsigned char *)(bloke + 4));
                }
                break;
            }
        }
        elem = next;
    }
    FUN_0042c800();
}

// FUNCTION: LEGOLAND 0x0042bcf0
void FUN_0042bcf0(struct CarouselRideObj *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6) {
    struct CarouselRide *ride = param_1->ride;
    struct CarouselListElem *elem;
    int bloke;
    int iVar6, iVar10;
    char cVar5;
    unsigned int uVar7;
    struct LayerResult layerres;
    int local_68;
    __int64 local_5c, local_54;
    unsigned int local_4c;
    int local_48;
    short local_44;
    int local_28[11];

    iVar6 = (int)ride;
    elem = ride->list;
    {
        int *fill = local_28;
        int n;
        local_28[0] = 0;
        for (n = 9; fill = fill + 1, n != 0; n--) {
            *fill = 0;
        }
    }
    local_44 = *param_4;
    cVar5 = '\0';
    local_4c = 0x103;
    local_48 = (int)param_1;
    bloke = (int)FUN_0042bc60(param_4);
    if (bloke != 0) {
        __int64 sc = GetScreenCoordsForObject((unsigned char *)param_4, ride);
        iVar10 = (int)(sc >> 0x20);
        iVar6 = (int)sc;
        GetLayer((struct LayerOwner *)ride->layer, &layerres, 0);
        if (elem != NULL) {
            short sVar1 = *param_4;
            do {
                if (sVar1 == (short)elem->id) {
                    int idx = (int)cVar5;
                    cVar5 = cVar5 + '\x01';
                    local_28[idx] = (int)elem->bloke;
                }
                elem = elem->next;
            } while (elem != NULL);
            if (cVar5 != '\0') {
                uVar7 = GetLLSForLayer((unsigned int)DAT_00616068, 0);
                LLSSetFrame((struct LLS *)uVar7, (int)*(char *)(bloke + 8));
                local_5c = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 0);
                AdjustOffsetForViewMode((struct AdjustStruct *)&local_5c);
                uVar7 = GetSpriteForLayer((struct LayerContainer *)DAT_00616068, 0);
                PrintSprite((struct Sprite *)uVar7, (int)local_5c + iVar6, (int)(local_5c >> 0x20) + iVar10, param_6, (int *)&local_4c);
                local_54 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 1);
                AdjustOffsetForViewMode((struct AdjustStruct *)&local_54);
                PrintSprite(DAT_00616070, (int)local_54 + iVar6, (int)(local_54 >> 0x20) + iVar10, param_6, (int *)&local_4c);
                uVar7 = GetLLSForLayer((unsigned int)DAT_00616068, 2);
                LLSSetFrame((struct LLS *)uVar7, (int)*(char *)(bloke + 8));
                local_5c = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 2);
                AdjustOffsetForViewMode((struct AdjustStruct *)&local_5c);
                uVar7 = GetSpriteForLayer((struct LayerContainer *)DAT_00616068, 2);
                PrintSprite((struct Sprite *)uVar7, (int)local_5c + iVar6, (int)(local_5c >> 0x20) + iVar10, param_6, (int *)&local_4c);
                if ('\0' < cVar5) {
                    int *p;
                    local_68 = (int)cVar5;
                    p = local_28;
                    do {
                        if (*(char *)(*p + 0x60) == '\0') {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p = p + 1;
                        local_68 = local_68 + -1;
                    } while (local_68 != 0);
                    if ('\0' < cVar5) {
                        local_68 = (int)cVar5;
                        p = local_28;
                        do {
                            if (*(char *)(*p + 0x60) == '\x01') {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p = p + 1;
                            local_68 = local_68 + -1;
                        } while (local_68 != 0);
                        if ('\0' < cVar5) {
                            local_68 = (int)cVar5;
                            p = local_28;
                            do {
                                if (*(char *)(*p + 0x60) == '\r') {
                                    IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                }
                                p = p + 1;
                                local_68 = local_68 + -1;
                            } while (local_68 != 0);
                            if ('\0' < cVar5) {
                                int param1c = (int)cVar5;
                                p = local_28;
                                do {
                                    if (*(char *)(*p + 0x60) == '\x0e') {
                                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                    }
                                    p = p + 1;
                                    param1c = param1c + -1;
                                } while (param1c != 0);
                            }
                        }
                    }
                }
                *(short *)**(int **)((char *)DAT_006160b8 + 8) = (short)*(char *)(bloke + 8);
                {
                    int local_64 = DAT_00616078;
                    int local_60 = DAT_0061607c;
                    struct CarouselListElem *e;
                    int saved_64 = local_64, saved_60 = local_60;
                    for (e = ride->list; local_60 = DAT_0061607c, local_64 = DAT_00616078,
                         DAT_00616078 = local_64, DAT_0061607c = local_60, e != NULL; e = e->next) {
                        int b = (int)e->bloke;
                        if (*param_4 == (short)e->id && (*(unsigned char *)(b + 0x62) & 0x80) != 0) {
                            int unit = *(int *)(b + 4);
                            *(int *)(unit + 0x24) = (int)*(short *)(b + 0x3c);
                            *(int *)(unit + 0x28) = (int)*(short *)(b + 0x3e);
                            AdjustBlokePosition((struct BlokePos *)(unit + 0x24));
                            AdjustOffsetForViewMode((struct AdjustStruct *)&local_64);
                            *(int *)(unit + 0x1c) = *(short *)(b + 0x3c) + local_64 + iVar6;
                            *(int *)(unit + 0x20) = *(short *)(b + 0x3e) + local_60 + iVar10;
                            AdjustBlokePosition((struct BlokePos *)(unit + 0x1c));
                            IP_RenderBlokeIn3DNow(e->bloke);
                            saved_64 = local_64;
                            saved_60 = local_60;
                        }
                        local_60 = saved_60;
                        local_64 = saved_64;
                    }
                    (void)saved_64;
                    (void)saved_60;
                }
                local_54 = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 1);
                AdjustOffsetForViewMode((struct AdjustStruct *)&local_54);
                PrintSprite(DAT_0061606c, (int)local_54 + iVar6, (int)(local_54 >> 0x20) + iVar10, param_6, 0);
                return;
            }
        }
        uVar7 = GetLLSForLayer((unsigned int)DAT_00616068, 0);
        LLSSetFrame((struct LLS *)uVar7, (int)*(char *)(bloke + 8));
        local_5c = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 0);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_5c);
        uVar7 = GetSpriteForLayer((struct LayerContainer *)DAT_00616068, 0);
        PrintSprite((struct Sprite *)uVar7, (int)local_5c + iVar6, (int)(local_5c >> 0x20) + iVar10, param_6, (int *)&local_4c);
        local_5c = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 1);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_5c);
        uVar7 = GetSpriteForLayer((struct LayerContainer *)DAT_00616068, 1);
        PrintSprite((struct Sprite *)uVar7, (int)local_5c + iVar6, (int)(local_5c >> 0x20) + iVar10, param_6, (int *)&local_4c);
        uVar7 = GetLLSForLayer((unsigned int)DAT_00616068, 2);
        LLSSetFrame((struct LLS *)uVar7, (int)*(char *)(bloke + 8));
        local_5c = GetRenderOffsetForLayer((struct LayerOffsetHolder *)DAT_00616068, 2);
        AdjustOffsetForViewMode((struct AdjustStruct *)&local_5c);
        uVar7 = GetSpriteForLayer((struct LayerContainer *)DAT_00616068, 2);
        PrintSprite((struct Sprite *)uVar7, (int)local_5c + iVar6, (int)(local_5c >> 0x20) + iVar10, param_6, (int *)&local_4c);
    }
}

// FUNCTION: LEGOLAND 0x0042cd20
int FUN_0042cd20(struct CarouselListElem *elem, struct CarouselNode *node, signed char divisor) {
    int count = divisor;
    int eax = rand();
    int index = eax % count;
    unsigned char *slots = (unsigned char *)node;

    while (slots[0x20 + index] != 0) {
        index++;
        if (index >= count) {
            index = 0;
        }
    }

    slots[0x20 + index] = 1;
    *(char *)((char *)elem->bloke + 0x36) = (char)(index + 1);
    return index + 1;
}
