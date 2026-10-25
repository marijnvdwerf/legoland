#include "popupinfo.h"
#include <windows.h>
#include <ddraw.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "icon.h"
#include "legoland.h"
#include "man3d.h"
#include "math.h"
#include "text.h"
#include "worker_mouse.h"

#pragma intrinsic(strlen, strcpy, strcat, memcpy)

struct Sprite;
struct Element;

struct InfoObjData {
    /* 0x00 */ unsigned char pad_0[0x4];
    /* 0x04 */ struct Element *field_4;
    /* 0x08 */ unsigned int field_8;
    /* 0x0c */ unsigned int field_c;
    /* 0x10 */ unsigned char pad_10[0x18 - 0x10];
    /* 0x18 */ int field_18;
    /* 0x1c */ struct InfoObjInner *field_1c;
};

struct InfoObjInner {
    /* 0x00 */ unsigned char pad_0[0x60];
    /* 0x60 */ unsigned char field_60;
};

#include "bloke.h"
#include "bloke_ai.h"
#include "build.h"
#include "controller.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "gfx.h"
#include "help.h"
#include "image_sprite.h"
#include "llidb.h"
#include "map_object.h"
#include "obj_instance.h"
#include "objclass.h"
#include "print_sprite.h"
#include "render.h"
#include "sound_music.h"
#include "stream.h"
#include "string.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

struct NewObjInfo {
    /* 0x00 */ unsigned char pad_0[0x26];
    /* 0x26 */ short field_26;
    /* 0x28 */ unsigned char pad_28[0x78 - 0x28];
    /* 0x78 */ char *field_78;
    /* 0x7c */ unsigned char pad_7c[0x80 - 0x7c];
    /* 0x80 */ char *field_80;
    /* 0x84 */ unsigned char pad_84[0xc4 - 0x84];
    /* 0xc4 */ char **name;
};

// FUNCTION: LEGOLAND 0x00470bb0
LEGO_EXPORT void InitPopUpInfo(void) {
    unsigned int *puVar3;
    int iVar2;

    puVar3 = (unsigned int *)&PopUpInfoHover;
    for (iVar2 = 0x40; iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar3 = 0;
        puVar3 = puVar3 + 1;
    }
    // STRING: LEGOLAND 0x004b89ac
    if (LLIDB_FindElement("POTTING SHED", &PottingShedHandle, 0) != 0) {
        exit(1);
    }
    // STRING: LEGOLAND 0x004b899c
    if (LLIDB_FindElement("MECHANICS HUT", &MechanicsHutHandle, 0) != 0) {
        exit(1);
    }
    if (LLIDB_FindElement(PathControlName, &PathControlHandle, 0) != 0) {
        exit(1);
    }
    // STRING: LEGOLAND 0x004b83d0
    if (LLIDB_FindElement("ENTRANCE 1", &Entrance1Handle, 0) != 0) {
        exit(1);
    }
    if (PopUpSpritesLoaded == NULL) {
        PopUpSpritesLoaded = (struct Sprite *)1;
        // STRING: LEGOLAND 0x004baca4
        PuBgMainSprite = LoadSprite("PU_BGMain.lls", 4);
        // STRING: LEGOLAND 0x004bac90
        PuBgCentreTopSprite = LoadSprite("PU_BGCentreTop.lls", 4);
        // STRING: LEGOLAND 0x004bac7c
        PuBgRightTopSprite = LoadSprite("PU_BGRightTop.lls", 4);
        // STRING: LEGOLAND 0x004bac68
        PuBgLeftMidSprite = LoadSprite("PU_BGLeftMid.lls", 4);
        // STRING: LEGOLAND 0x004bac54
        PuBgCentreMidSprite = LoadSprite("PU_BGCentreMid.lls", 4);
        // STRING: LEGOLAND 0x004bac40
        PuBgRightMidSprite = LoadSprite("PU_BGRightMid.lls", 4);
        // STRING: LEGOLAND 0x004bac2c
        PuBgLeftBtmSprite = LoadSprite("PU_BGLeftBtm.lls", 4);
        // STRING: LEGOLAND 0x004bac18
        PuBgCentreBtmSprite = LoadSprite("Pu_BGCentreBtm.lls", 4);
        // STRING: LEGOLAND 0x004bac04
        PuBgRightBtmSprite = LoadSprite("PU_BGRightBtm.lls", 4);
        // STRING: LEGOLAND 0x004babf4
        NewPopMockSprite = LoadSprite("NewPopMock.lls", 4);
        // STRING: LEGOLAND 0x004babe0
        ObjectRepairOkSprite = LoadSprite("ObjectRepairOK.lls", 4);
        // STRING: LEGOLAND 0x004babcc
        ObjectNoRepair1Sprite = LoadSprite("ObjectNoRepair1.lls", 4);
        // STRING: LEGOLAND 0x004babb4
        PuDeleteObjectOnSprite = LoadSprite("PU_DeleteObjectON.lls", 4);
        // STRING: LEGOLAND 0x004baba0
        PuDeleteObjectSprite = LoadSprite("PU_DeleteObject.lls", 4);
        // STRING: LEGOLAND 0x004bab8c
        PuClosePopUpOnSprite = LoadSprite("PU_ClosePopUpON.lls", 4);
        // STRING: LEGOLAND 0x004bab78
        PuClosePopUpSprite = LoadSprite("PU_ClosePopUp.lls", 4);
        // STRING: LEGOLAND 0x004bab60
        PuAddGardenerOnSprite = LoadSprite("PU_AddGardenerON.lls", 4);
        // STRING: LEGOLAND 0x004bab4c
        PuAddGardenerSprite = LoadSprite("PU_AddGardener.lls", 4);
        // STRING: LEGOLAND 0x004bab34
        PuAddMechanicsOnSprite = LoadSprite("PU_AddMechanicsON.lls", 4);
        // STRING: LEGOLAND 0x004bab20
        PuAddMechanicsSprite = LoadSprite("PU_AddMechanics.lls", 4);
        // STRING: LEGOLAND 0x004bab10
        NextIconSprite = LoadSprite("NextIcon.lls", 4);
        // STRING: LEGOLAND 0x004bab00
        NextIconOnSprite = LoadSprite("NextIconOn.lls", 4);
        // STRING: LEGOLAND 0x004baaf0
        PrevIconSprite = LoadSprite("PrevIcon.lls", 4);
        // STRING: LEGOLAND 0x004baae0
        PrevIconOnSprite = LoadSprite("PrevIconOn.lls", 4);
        // STRING: LEGOLAND 0x004baad4
        ISadSprite = LoadSprite("i_sad.lls", 0);
        // STRING: LEGOLAND 0x004baac8
        INormSprite = LoadSprite("i_norm.lls", 0);
        // STRING: LEGOLAND 0x004baabc
        IHappySprite = LoadSprite("i_happy.lls", 0);
        // STRING: LEGOLAND 0x004baaac
        IHungrySprite = LoadSprite("i_hungry.lls", 0);
        // STRING: LEGOLAND 0x004baa9c
        IPeckishSprite = LoadSprite("i_peckish.lls", 0);
        // STRING: LEGOLAND 0x004baa90
        IFullSprite = LoadSprite("i_full.lls", 0);
    }
    AddGardenerIcon = InsertIcon(0, 0, 0x2c3, PuAddGardenerSprite);
    AddGardenerIcon->string_id = 0x6e;
    AddGardenerIcon->string = GetString(0x6e);
    AddGardenerIcon->flags |= 0x2000;
    AddGardenerIcon->flags |= 0x4002;
    AddGardenerIcon->flags |= 0x400;
    AddGardenerIcon->event_handler = (void *)AddGardenerEventHandler;
    AddMechanicsIcon = InsertIcon(0, 0, 0x2c3, PuAddMechanicsSprite);
    AddMechanicsIcon->string_id = 0x6f;
    AddMechanicsIcon->string = GetString(0x6f);
    AddMechanicsIcon->flags |= 0x2000;
    AddMechanicsIcon->flags |= 0x4002;
    AddMechanicsIcon->flags |= 0x400;
    AddMechanicsIcon->event_handler = (void *)AddMechanicsEventHandler;
    DeleteObjectIcon = InsertIcon(0, 0, 0x2c3, PuDeleteObjectSprite);
    DeleteObjectIcon->string_id = 0x70;
    DeleteObjectIcon->string = GetString(0x70);
    DeleteObjectIcon->flags |= 0x2000;
    DeleteObjectIcon->flags |= 0x4002;
    DeleteObjectIcon->flags |= 0x400;
    DeleteObjectIcon->event_handler = (void *)DeleteObjectEventHandler;
    ClosePopUpIcon = InsertIcon(0, 0, 0x2c3, PuClosePopUpSprite);
    ClosePopUpIcon->string_id = 0x73;
    ClosePopUpIcon->string = GetString(0x73);
    ClosePopUpIcon->flags |= 0x2000;
    ClosePopUpIcon->flags |= 0x4002;
    ClosePopUpIcon->flags |= 0x400;
    ClosePopUpIcon->event_handler = (void *)ClosePopUpEventHandler;
    NextPopUpIcon = InsertIcon(0, 0, 0x2c3, NextIconSprite);
    NextPopUpIcon->string_id = 0x88e;
    NextPopUpIcon->string = GetString(0x88e);
    NextPopUpIcon->flags |= 0x2000;
    NextPopUpIcon->flags |= 0x4002;
    NextPopUpIcon->flags |= 0x400;
    NextPopUpIcon->event_handler = (void *)NextIconEventHandler;
    PrevPopUpIcon = InsertIcon(0, 0, 0x2c3, PrevIconSprite);
    PrevPopUpIcon->string_id = 0x88f;
    PrevPopUpIcon->string = GetString(0x88f);
    PrevPopUpIcon->flags |= 0x2000;
    PrevPopUpIcon->flags |= 0x4002;
    PrevPopUpIcon->flags |= 0x400;
    PrevPopUpIcon->event_handler = (void *)PrevIconEventHandler;
    // STRING: LEGOLAND 0x004baa7c
    PuCornerMaskIcon = LoadSpriteIcon("PU_CornerMask.lls", 4, 0, 0, 0x2c3);
    PuCornerMaskIcon->flags |= 0x400;
    PopUpDeleteIcon = InsertIcon(0, 0, 0x2c3, PuDeleteObjectSprite);
    PopUpDeleteIcon->string_id = 0xa1;
    PopUpDeleteIcon->string = GetString(0xa1);
    PopUpDeleteIcon->flags |= 0x2000;
    PopUpDeleteIcon->flags |= 0x4002;
    PopUpDeleteIcon->flags |= 0x400;
    PopUpDeleteIcon->event_handler = (void *)DeleteWorkOrderEventHandler;
    CreatePopUpInfoButtons(PopUpOkEventHandler, CBCloseEventHandler);
}

// FUNCTION: LEGOLAND 0x00471170
int UnloadPopUpSprites(void) {
    if (PopUpSpritesLoaded != NULL) {
        PopUpSpritesLoaded = NULL;

        if (PuBgMainSprite != NULL) {
            KillSprite(PuBgMainSprite);
            PuBgMainSprite = NULL;
        }
        if (PuBgCentreTopSprite != NULL) {
            KillSprite(PuBgCentreTopSprite);
            PuBgCentreTopSprite = NULL;
        }
        if (PuBgRightTopSprite != NULL) {
            KillSprite(PuBgRightTopSprite);
            PuBgRightTopSprite = NULL;
        }
        if (PuBgCentreMidSprite != NULL) {
            KillSprite(PuBgCentreMidSprite);
            PuBgCentreMidSprite = NULL;
        }
        if (PuBgLeftMidSprite != NULL) {
            KillSprite(PuBgLeftMidSprite);
            PuBgLeftMidSprite = NULL;
        }
        if (PuBgRightMidSprite != NULL) {
            KillSprite(PuBgRightMidSprite);
            PuBgRightMidSprite = NULL;
        }
        if (PuBgLeftBtmSprite != NULL) {
            KillSprite(PuBgLeftBtmSprite);
            PuBgLeftBtmSprite = NULL;
        }
        if (PuBgCentreBtmSprite != NULL) {
            KillSprite(PuBgCentreBtmSprite);
            PuBgCentreBtmSprite = NULL;
        }
        if (PuBgRightBtmSprite != NULL) {
            KillSprite(PuBgRightBtmSprite);
            PuBgRightBtmSprite = NULL;
        }
        if (ObjectRepairOkSprite != NULL) {
            KillSprite(ObjectRepairOkSprite);
            ObjectRepairOkSprite = NULL;
        }
        if (ObjectNoRepair1Sprite != NULL) {
            KillSprite(ObjectNoRepair1Sprite);
            ObjectNoRepair1Sprite = NULL;
        }
        if (PuDeleteObjectOnSprite != NULL) {
            KillSprite(PuDeleteObjectOnSprite);
            PuDeleteObjectOnSprite = NULL;
        }
        if (PuDeleteObjectSprite != NULL) {
            KillSprite(PuDeleteObjectSprite);
            PuDeleteObjectSprite = NULL;
        }
        if (PuClosePopUpOnSprite != NULL) {
            KillSprite(PuClosePopUpOnSprite);
            PuClosePopUpOnSprite = NULL;
        }
        if (PuClosePopUpSprite != NULL) {
            KillSprite(PuClosePopUpSprite);
            PuClosePopUpSprite = NULL;
        }
        if (NextIconSprite != NULL) {
            KillSprite(NextIconSprite);
            NextIconSprite = NULL;
        }
        if (NextIconOnSprite != NULL) {
            KillSprite(NextIconOnSprite);
            NextIconOnSprite = NULL;
        }
        if (PrevIconSprite != NULL) {
            KillSprite(PrevIconSprite);
            PrevIconSprite = NULL;
        }
        if (PrevIconOnSprite != NULL) {
            KillSprite(PrevIconOnSprite);
            PrevIconOnSprite = NULL;
        }
        if (PuAddGardenerOnSprite != NULL) {
            KillSprite(PuAddGardenerOnSprite);
            PuAddGardenerOnSprite = NULL;
        }
        if (PuAddGardenerSprite != NULL) {
            KillSprite(PuAddGardenerSprite);
            PuAddGardenerSprite = NULL;
        }
        if (PuAddMechanicsOnSprite != NULL) {
            KillSprite(PuAddMechanicsOnSprite);
            PuAddMechanicsOnSprite = NULL;
        }
        if (PuAddMechanicsSprite != NULL) {
            KillSprite(PuAddMechanicsSprite);
            PuAddMechanicsSprite = NULL;
        }
        if (ISadSprite != NULL) {
            KillSprite(ISadSprite);
            ISadSprite = NULL;
        }
        if (INormSprite != NULL) {
            KillSprite(INormSprite);
            INormSprite = NULL;
        }
        if (IHappySprite != NULL) {
            KillSprite(IHappySprite);
            IHappySprite = NULL;
        }
        if (IHungrySprite != NULL) {
            KillSprite(IHungrySprite);
            IHungrySprite = NULL;
        }
        if (IPeckishSprite != NULL) {
            KillSprite(IPeckishSprite);
            IPeckishSprite = NULL;
        }
        if (IFullSprite != NULL) {
            KillSprite(IFullSprite);
            IFullSprite = NULL;
        }
        KillPUOKAndCBSprites();
    }
}

// FUNCTION: LEGOLAND 0x00471450
LEGO_EXPORT int UnLoad_PopUpInfo(void) {
    RemoveIconGroup(0x2c3);
    return UnloadPopUpSprites();
}

// FUNCTION: LEGOLAND 0x00471470
void DisablePopUpIconHandlers(void) {
    DeleteObjectIcon->event_handler = NULL;
    AddGardenerIcon->event_handler = NULL;
    AddMechanicsIcon->event_handler = NULL;
    PopUpDeleteIcon->event_handler = NULL;
}

// FUNCTION: LEGOLAND 0x004714a0
void EnablePopUpIconHandlers(void) {
    DeleteObjectIcon->event_handler = (void *)DeleteObjectEventHandler;
    ClosePopUpIcon->event_handler = (void *)ClosePopUpEventHandler;
    AddGardenerIcon->event_handler = (void *)AddGardenerEventHandler;
    AddMechanicsIcon->event_handler = (void *)AddMechanicsEventHandler;
    PopUpDeleteIcon->event_handler = (void *)DeleteWorkOrderEventHandler;
}

// FUNCTION: LEGOLAND 0x004714e0
void RemoveAllNewObjects(void) {
    while (NewObjects.count != 0) {
        RemoveNewObject(NewObjects.objs[0]);
    }
}

// FUNCTION: LEGOLAND 0x00471510
LEGO_EXPORT void ResetInfoStruct(void) {
    PopUpInfoRide = 0;
    PopUpInfoBloke = 0;
    PopUpInfoTile = 0;
    PopUpInfoSubjectType = 0;
    PopUpInfoState = 0;
    PopUpDeleteConfirm = 0;
    PopUpInfoResizePending = 0;
    PopUpFollowingVisitor = 0;
    PopupInfoLineCount = 0;
    DisableInfoPopUPIcons();
    EnablePopUpIconHandlers();
}

// FUNCTION: LEGOLAND 0x00471550
LEGO_EXPORT void PopInfoSizeMayChange(void) {
    if (PopUpInfoState != 0) {
        PopUpInfoResizePending = 1;
    }
}

// FUNCTION: LEGOLAND 0x00471570
LEGO_EXPORT void StopFollowingBloke(void) {
    if (PopUpFollowingVisitor == 0) {
        return;
    }
    ResetInfoStruct();
}

// FUNCTION: LEGOLAND 0x00471580
LEGO_EXPORT void DisableInfoPopUPIcons(void) {
    DeleteObjectIcon->flags |= 0x400;
    ClosePopUpIcon->flags |= 0x400;
    PuCornerMaskIcon->flags |= 0x400;
    PopUpInfoOkIcon->flags |= 0x400;
    CBCloseIcon->flags |= 0x400;
    AddGardenerIcon->flags |= 0x400;
    AddMechanicsIcon->flags |= 0x400;
    PopUpDeleteIcon->flags |= 0x400;
    NextPopUpIcon->flags |= 0x400;
    PrevPopUpIcon->flags |= 0x400;
}

// FUNCTION: LEGOLAND 0x00471610
void SetPopUpIconSprites(void) {
    SetIconSprite((struct IconNode *)DeleteObjectIcon, PuDeleteObjectSprite);
    SetIconSprite((struct IconNode *)PrevPopUpIcon, PrevIconSprite);
    SetIconSprite((struct IconNode *)NextPopUpIcon, NextIconSprite);
    SetIconSprite((struct IconNode *)ClosePopUpIcon, PuClosePopUpSprite);
    SetIconSprite((struct IconNode *)AddGardenerIcon, PuAddGardenerSprite);
    SetIconSprite((struct IconNode *)AddMechanicsIcon, PuAddMechanicsSprite);
    SetIconSprite((struct IconNode *)PopUpDeleteIcon, PuDeleteObjectSprite);
}

// FUNCTION: LEGOLAND 0x004716a0
LEGO_EXPORT void InfoPrintCent(int len, char *text, int font, RECT rc, int flag) {
    HRGN region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    HDC hdc;
    HGDIOBJ old_region;
    HGDIOBJ old_font;

    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    SetTextColor(hdc, 0xffffff);
    old_region = SelectObject(hdc, region);
    old_font = (HGDIOBJ)SelectFont(hdc, font);
    if (flag != 0) {
        DrawTextA(hdc, text, strlen(text), &rc, 0x11);
    } else {
        DrawTextA(hdc, text, strlen(text), &rc, 0x10);
    }
    SelectObject(hdc, old_font);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    DeleteObject(region);
}

/* Measures word-wrapped centred text (DrawTextA DT_CALCRECT | DT_WORDBREAK | DT_CENTER, font param_6) at width
 * param_4 and returns how many steps of (height + param_3, width + param_5) the pop-up must grow until the text
 * height fits param_2. The memory DC is never deleted. */
// FUNCTION: LEGOLAND 0x004717a0
int CountPopUpTextGrowSteps(const char *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    HDC hdc;
    int iVar2;
    int iVar4;
    RECT local_10;

    local_10.left = 0;
    local_10.top = 0;
    local_10.right = 0;
    local_10.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    iVar4 = param_2;
    param_2 = 0;
    local_10.right = param_4;
    SelectFont(hdc, param_6);
    while (1) {
        iVar2 = DrawTextA(hdc, param_1, strlen(param_1), &local_10, 0x411);
        if (iVar2 <= iVar4) {
            return param_2;
        }
        iVar4 = iVar4 + param_3;
        local_10.right = local_10.right + param_5;
        param_2 = param_2 + 1;
        if (iVar2 <= iVar4) {
            return param_2;
        }
    }
}

/* Measures single-line centred text (DT_CALCRECT | DT_CENTER, font param_6) and returns the extra width beyond
 * param_4 in 32-pixel steps, rounded up (zero or negative when it fits). The memory DC is never deleted. */
// FUNCTION: LEGOLAND 0x00471840
int CountPopUpTextWidthSteps(const char *param_1, int param_2, int param_3, int param_4, int param_5, int param_6) {
    HDC hdc;
    RECT local_10;

    local_10.left = 0;
    local_10.top = 0;
    local_10.right = 0;
    local_10.bottom = 0;
    hdc = CreateCompatibleDC(NULL);
    local_10.right = param_4;
    SelectFont(hdc, param_6);
    DrawTextA(hdc, param_1, strlen(param_1), &local_10, 0x401);
    return (((local_10.right - local_10.left) - param_4) + 0x1f) >> 5;
}

// FUNCTION: LEGOLAND 0x004718c0
void ClampPopUpInfoPosition(int param_1) {
    int x;
    int y;
    int width;
    int height;

    x = PopUpInfoX;
    y = PopUpInfoY;
    width = param_1 * 0x20 + 0xc8;
    height = param_1 * 0x14 + 0x96;
    if (x > 0x27b - width) {
        x = x + (-5 - width);
        if (x < 0) {
            x = 5;
        }
    }
    if (x < 0x82 && BuildSidePanelState != 2) {
        x = 0x82;
    }
    if (y < 0x25) {
        y = 0x25;
    } else if (y > 0x16f - height) {
        y = 0x16f - height;
    }
    PopUpInfoX = x;
    PopUpInfoY = y;
}

// FUNCTION: LEGOLAND 0x00471950
LEGO_EXPORT void PopUpInfoSetUp(struct HoverInfo t, unsigned int param_4, unsigned int param_5) {
    int iVar2;
    int x;
    int y;
    unsigned int uVar3;
    unsigned char *iVar4;
    INT_PTR iVar5;

    uVar3 = t.data.value & 0xffff;
    x = uVar3 & 0xff;
    y = uVar3 >> 8;
    if ((x < 0) || (x >= lpConfig->width) || (y < 0) || (y >= lpConfig->height)) {
        iVar4 = NULL;
    } else {
        iVar4 = (unsigned char *)&GameMap[y][x];
    }
    ResetInfoStruct();
    PopUpInfoState = 1;
    PopUpInfoResizePending = 1;
    PopUpInfoX = param_4;
    PopUpInfoY = param_5;
    PopUpInfoHoverValue = (unsigned short)t.data.value;
    PopUpInfoHover.type = t.type;
    PopUpInfoHover.ptr = t.ptr;
    PopUpInfoHover.data.value = t.data.value;
    switch (t.type) {
    case 0x307:
        if ((short)t.ptr->action != 5) {
            PlayInstanceOfSample(GameFX[FX_GARDENER_LIFT].sample, 0, 1, 0);
            PickUpWorker(0x307, t.ptr);
        }
        break;
    case 0x103:
        if (t.ptr == NULL) {
            return;
        }
        if (((struct Element *)t.ptr != PathControlHandle) && ((struct Element *)t.ptr != Entrance1Handle)) {
            PopUpInfoRide = ((struct Element *)t.ptr)->ride;
            PopUpInfoTile = iVar4;
            if (PopUpInfoRide->element == PottingShedHandle) {
                PopUpInfoState = 0;
                PopUpInfoSubjectType = 10;
                param_4 = iVar4[4];
                (&param_4)[1] = iVar4[5];
                if (BuyGardener() == 0) {
                    return;
                }
                GenerateGardener((int *)&param_4, 1);
                return;
            }
            if (PopUpInfoRide->element == MechanicsHutHandle) {
                PopUpInfoState = 0;
                PopUpInfoSubjectType = 0x14;
                param_4 = iVar4[4];
                (&param_4)[1] = iVar4[5];
                if (BuyMechanic() == 0) {
                    return;
                }
                GenerateMechanic((int *)&param_4, 1);
                return;
            }
            PopUpInfoSubjectType = 0x103;
            return;
        }
        break;
    case 0x306:
        PopUpInfoSubjectType = t.type;
        PopUpInfoBloke = t.ptr;
        PopUpVisitorField1c = t.ptr->person->field_1c;
        PopUpVisitorField20 = t.ptr->person->field_20;
        return;
    case 0x308:
        if (t.ptr->action != 5) {
            if (((t.ptr->action == 0x13) && (t.ptr->param_action >= 0x6b)) ||
                ((t.ptr->action == 0x16) && (t.ptr->param_action >= 0x6b))) {
                iVar4 = (unsigned char *)t.ptr->order;
                iVar5 = ((WorkOrder *)iVar4)->pos.x;
                if ((iVar5 < 0) || (iVar5 >= (int)(unsigned int)lpConfig->width || (iVar2 = ((WorkOrder *)iVar4)->pos.y, iVar2 < 0)) ||
                    (iVar2 >= (int)(unsigned int)lpConfig->height)) {
                    iVar5 = 0;
                } else {
                    iVar5 = (INT_PTR)GameMap[iVar2] + iVar5 * sizeof(struct MapElement);
                }
                *(unsigned short *)(iVar5 + offsetof(struct MapElement, flags)) = *(unsigned short *)(iVar5 + offsetof(struct MapElement, flags)) & 0xbfff;
                FreeMechanicWorkOrder((WorkOrder *)iVar4);
            }
            PlayInstanceOfSample(GameFX[FX_MECHANIC_LIFT].sample, 0, 1, 0);
            PickUpWorker(0x308, t.ptr);
        }
        break;
    }
    ResetInfoStruct();
}

// FUNCTION: LEGOLAND 0x00471bf0
void ClearNewObjectsList(void) {
    if (PopUpInfoState != 2) {
        NewObjects.current = 0;
        NewObjects.count = 0;
    }
}

// FUNCTION: LEGOLAND 0x00471c10
void AddNewObjectIcon(struct NewObjInfo *param_1) {
    struct Sprite *sprite;
    char local_200[512];

    if ((int)NewObjects.count < 0x14) {
        // STRING: LEGOLAND 0x004bacd8
        sprintf(local_200, "NewObjIcons\\%s.bmp", *param_1->name);
        sprite = LoadSprite(local_200, 0);
        NewObjects.sprites[NewObjects.count] = sprite;
        if (NewObjects.sprites[NewObjects.count] != NULL) {
            NewObjects.objs[NewObjects.count] = param_1;
            NewObjects.count = NewObjects.count + 1;
            return;
        }
        // STRING: LEGOLAND 0x004bacb4
        DBPrintf("Failied to open New Obj graphic %s\n", local_200);
    }
}

// FUNCTION: LEGOLAND 0x00471ca0
void RemoveNewObject(void *param) {
    int j;
    int k;

    for (j = 0; j < (int)NewObjects.count; j++) {
        if (param == NewObjects.objs[j]) {
            if (NewObjects.sprites[j] != NULL) {
                KillSprite(NewObjects.sprites[j]);
                NewObjects.sprites[j] = NULL;
            }
            for (k = j; k + 1 < (int)NewObjects.count; k++) {
                NewObjects.sprites[k] = NewObjects.sprites[k + 1];
                NewObjects.objs[k] = NewObjects.objs[k + 1];
            }
            NewObjects.count--;
            if (NewObjects.current >= (int)NewObjects.count) {
                NewObjects.current = NewObjects.count - 1;
            }
            if (NewObjects.count == 0 && PopUpInfoState == 2) {
                PopUpInfoState = 0;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x00471d40
void ShowNewObjectsInfo(void) {
    if (NewObjects.count != 0) {
        ResetInfoStruct();
        PopUpInfoState = 2;
    }
}

// FUNCTION: LEGOLAND 0x00471d60
void SetPopUpOkCloseIconSprites(void) {
    SetIconSprite((struct IconNode *)PopUpInfoOkIcon, PUOKSprite);
    SetIconSprite((struct IconNode *)CBCloseIcon, CBCloseSprite);
}

/* Draws the variable-width popup footer, positions the OK and Close icons, draws its label, and refreshes their
 * idle sprites when the mouse leaves the button area. */
// FUNCTION: LEGOLAND 0x00471d90
void DrawInfoPopupFooter(void) {
    int iVar1;
    short sVar2;
    char *str;
    int uVar4;
    int iVar5;
    int iVar6;
    RECT rc;
    struct PrintCtx ctx = {1};
    char local_14[20];

    iVar6 = PopUpInfoX;
    uVar4 = *(unsigned int *)&PopupInfoLineCount & 0xff;
    iVar5 = uVar4 * 0x14;
    iVar1 = PopUpInfoY + 0x48 + iVar5;
    PrintSprite(CBBGLeftSprite, PopUpInfoX, iVar1, 0, (int *)&ctx);
    iVar6 = iVar6 + 0x7a;
    for (; uVar4 > 0; uVar4 = uVar4 - 1) {
        PrintSprite(CBBGCentreSprite, iVar6, iVar1, 0, (int *)&ctx);
        iVar6 = iVar6 + 0x20;
    }
    PrintSprite(CBBGRightSprite, iVar6, iVar1, 0, (int *)&ctx);
    PopUpInfoOkIcon->flags = PopUpInfoOkIcon->flags & 0xfffffbff;
    iVar6 = iVar6 + 0x4e;
    PopUpInfoOkIcon->x = (short)iVar6 - 0x4b;
    sVar2 = (short)iVar1 + 3;
    PopUpInfoOkIcon->y = sVar2;
    CBCloseIcon->flags = CBCloseIcon->flags & 0xfffffbff;
    CBCloseIcon->x = (short)iVar6 - 0x27;
    CBCloseIcon->y = sVar2;
    str = GetString(0xa2);
    sprintf(local_14, (char *)PercentSFormat, str);
    rc.left = PopUpInfoX + 0xc;
    rc.top = iVar1 + 6;
    rc.right = rc.left + 0x7a + iVar5;
    rc.bottom = rc.top + 0x1b;
    PrintTextBox(local_14, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 1, 5, 0xff0000, 0xffffff);
    rc.left = PopUpInfoOkIcon->x;
    rc.top = iVar1;
    rc.right = CBCloseIcon->x + 0x24;
    rc.bottom = iVar1 + 0x1b;
    if ((rc.right < (int)MousePos.x) || ((int)MousePos.x < rc.left)) {
        SetPopUpOkCloseIconSprites();
    }
    if ((rc.bottom < (int)MousePos.y) || ((int)MousePos.y < rc.top)) {
        SetPopUpOkCloseIconSprites();
    }
    DisablePopUpIconHandlers();
}

// FUNCTION: LEGOLAND 0x00471f10
void DrawPopUpInfoBackground(void) {
    int x;
    int y;
    int count;
    int innerX;
    int cx;
    int cy;
    int row;
    int col;
    struct PrintCtx ctx = {1};

    count = *(unsigned int *)&PopupInfoLineCount & 0xff;
    x = PopUpInfoX;
    y = PopUpInfoY;
    PrintSprite(PuBgMainSprite, x, y, 0, (int *)&ctx);
    innerX = x + 0xbc;
    cx = innerX;
    for (col = count; col > 0; col--) {
        PrintSprite(PuBgCentreTopSprite, cx, y, 0, (int *)&ctx);
        cx += 0x20;
    }
    PrintSprite(PuBgRightTopSprite, cx, y, 0, (int *)&ctx);
    if (count != 0) {
        cy = y + 99;
        for (row = count; row > 0; row--) {
            PrintSprite(PuBgLeftMidSprite, x, cy, 0, (int *)&ctx);
            cx = innerX;
            for (col = count; col > 0; col--) {
                PrintSprite(PuBgCentreMidSprite, cx, cy, 0, (int *)&ctx);
                cx += 0x20;
            }
            PrintSprite(PuBgRightMidSprite, cx, cy, 0, (int *)&ctx);
            cy += 0x14;
        }
    }
    cy = y + 99 + count * 0x14;
    PrintSprite(PuBgRightBtmSprite, x, cy, 0, (int *)&ctx);
    cx = innerX;
    for (col = count; col > 0; col--) {
        PrintSprite(PuBgCentreBtmSprite, cx, cy, 0, (int *)&ctx);
        cx += 0x20;
    }
    PrintSprite(PuBgLeftBtmSprite, cx, cy, 0, (int *)&ctx);
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x00472090
void Empty_00472090(void) {
}

// Inlined in the original: the position is computed before the hide flag is cleared.
static __inline void ShowPopUpIconAt(struct IconNode **icon, int x, int y) {
    (*icon)->flags &= 0xfffffbff;
    (*icon)->x = x;
    (*icon)->y = y;
}

// FUNCTION: LEGOLAND 0x004720a0
void DrawNewObjectPopup(void) {
    struct NewObjInfo *obj;
    struct PrintCtx ctx = {1};
    char text[128];
    int close_x;
    int close_y;
    RECT rc;

    PrintSprite(NewPopMockSprite, 0xbe, 0x28, 0, (int *)&ctx);

    ShowPopUpIconAt(&PrevPopUpIcon, 0xc1, 0x46);
    ShowPopUpIconAt(&NextPopUpIcon, NewPopMockSprite->width - ClosePopUpIcon->width + 0xbb, 0x46);
    close_x = NewPopMockSprite->width - ClosePopUpIcon->width + 0xbb;
    close_y = (short)NewPopMockSprite->height - ClosePopUpIcon->height + 0x25;
    ShowPopUpIconAt(&ClosePopUpIcon, close_x, close_y);

    PushRenderingStatusAndUnlockVideoSurface();
    if (NewObjects.count == 1) {
        // STRING: LEGOLAND 0x004bad04
        sprintf(text, "You have a new object");
    } else {
        // STRING: LEGOLAND 0x004bacec
        sprintf(text, "You have %d new objects", NewObjects.count);
    }
    rc.left = 0xc1;
    rc.top = 0x30;
    rc.right = NewPopMockSprite->width + 0xbb;
    rc.bottom = 0x44;
    PrintTextBox(text, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 2, 5, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)NewObjects.objs[NewObjects.current];
    rc.left = 0xc1;
    rc.top = 0x4b;
    rc.right = NewPopMockSprite->width + 0xbb;
    rc.bottom = 0x5f;
    PrintTextBox(obj->field_78, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 2, 5, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)NewObjects.objs[NewObjects.current];
    rc.left = 0x13e;
    rc.top = 0x68;
    rc.right = 0x23a;
    rc.bottom = 0xdf;
    PrintTextBox(obj->field_80, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 2, 0x10, 0xff0000, 0xffffff);
    obj = (struct NewObjInfo *)NewObjects.objs[NewObjects.current];
    sprintf(text, "%d", obj->field_26);
    rc.left = 0xf0;
    rc.top = 0xd1;
    rc.right = 0x133;
    rc.bottom = 0xe3;
    PrintTextBox(text, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 2, 1, 0xff0000, 0xffffff);
    PopRenderingStatus();
    PrintSprite(NewObjects.sprites[NewObjects.current], 0xc4, 100, 0, 0);

    /* restore the normal sprite of each button the mouse is not over */
    if ((unsigned int)(MousePos.x - ClosePopUpIcon->x) >= (unsigned int)ClosePopUpIcon->width ||
        (unsigned int)(MousePos.y - ClosePopUpIcon->y) >= (unsigned int)ClosePopUpIcon->height) {
        SetIconSprite(ClosePopUpIcon, PuClosePopUpSprite);
    }
    if ((unsigned int)(MousePos.x - NextPopUpIcon->x) >= (unsigned int)NextPopUpIcon->width ||
        (unsigned int)(MousePos.y - NextPopUpIcon->y) >= (unsigned int)NextPopUpIcon->height) {
        SetIconSprite(NextPopUpIcon, NextIconSprite);
    }
    if ((unsigned int)(MousePos.x - PrevPopUpIcon->x) >= (unsigned int)PrevPopUpIcon->width ||
        (unsigned int)(MousePos.y - PrevPopUpIcon->y) >= (unsigned int)PrevPopUpIcon->height) {
        SetIconSprite(PrevPopUpIcon, PrevIconSprite);
    }

    if (NewObjects.current == 0) {
        PrevPopUpIcon->flags |= 0x400;
    } else {
        PrevPopUpIcon->flags &= ~0x400;
    }
    if (NewObjects.current == (int)(NewObjects.count - 1)) {
        NextPopUpIcon->flags |= 0x400;
    } else {
        NextPopUpIcon->flags &= ~0x400;
    }
}

/* Temporarily configures the query class and cursor for the popup object, runs its cursor setup and buildability
 * test, then restores the previous query state. */
// FUNCTION: LEGOLAND 0x004723f0
int IsPopupObjectBuildable(void) {
    int result;
    unsigned int local_8[2];
    struct Cursor local_cursor;
    void *saved_class;
    struct Ride *cls;
    unsigned int v;

    saved_class = QueryClass;
    v = PopUpInfoHover.data.value & 0xffff;
    memcpy(&local_cursor, &QueryCursor, sizeof(struct Cursor));
    local_8[0] = v & 0xff;
    local_8[1] = v >> 8;
    cls = ((struct Element *)PopUpInfoHover.ptr)->ride;
    QueryCursor.tile_y = v >> 8;
    QueryClass = cls;
    QueryCursor.tile_x = v & 0xff;
    cls->cb_94(cls->element, local_8);
    BuildCursorPtr(&QueryCursor, 0, 0);
    result = IsCursorBuildable(&QueryCursor);
    memcpy(&QueryCursor, &local_cursor, sizeof(struct Cursor));
    QueryClass = saved_class;
    return result;
}

/* Draws the info popup: the title/info text for the selected object or bloke, the mood
   sprites, the build/repair progress bar and the icons (repair, delete, add gardener/mechanic). */
// FUNCTION: LEGOLAND 0x004724a0
LEGO_EXPORT void DrawPopUpInfo(void) {
    char buf_b[256];
    char buf_a[256];
    char tmp[512];
    struct Ride *ride;
    struct Bloke *bloke;
    int repairable = 0;
    int can_delete = 0;
    int add_gardener = 0;
    int add_mechanic = 0;
    int show_close = 0;
    int size;
    int x;
    int y;
    struct Point tl;
    struct Point br;
    int width;
    int bar_x;
    int bar_y;
    int bar_w;
    int mid;
    int row_y; /* y of the current label/icon row: the original keeps one variable for both */
    int mood;
    int hunger;
    int power;
    int name_lines;
    int info_lines;
    int entry_index;
    int icon_x;
    int icon_y;
    int icon_bottom;
    int last_x;
    int left_bound;
    float frac;
    struct BuildObj *entry;
    TileId target;
    struct InfoObjData *info;

    buf_a[0] = 0;
    memset(buf_a + 1, 0, 255);
    buf_b[0] = 0;
    memset(buf_b + 1, 0, 255);
    ride = (struct Ride *)PopUpInfoRide;
    bloke = (struct Bloke *)PopUpInfoBloke;
    GetNearestColour(0xda, 0xc6, 0x96);
    PopUpShowsBuildProgress = 0;
    if (PopUpInfoState == 2) {
        DrawNewObjectPopup();
        return;
    }
    if (0 != EditMode.unk0) {
        ResetInfoStruct();
        return;
    }
    if (PopUpInfoState == 0) {
        return;
    }
    if (MapRightClickFlags & 2) {
        ResetInfoStruct();
        return;
    }
    switch (PopUpInfoSubjectType) {
    case 0x103:
        sprintf(buf_a, (char *)PercentSFormat, ride->name);
        // STRING: LEGOLAND 0x004bad44
        sprintf(buf_b, "%s %d\n%s %d", GetString(0x76), GetObjRepairCost(ride, PopUpInfoTile[0x11]), GetString(0x77), GetObjSalvageValue(ride, PopUpInfoTile[0x11]));
        if (MapStats.power_enabled != 0) {
            power = FindObjectsPower(ride);
            if (power < 0) {
                // STRING: LEGOLAND 0x004bad3c
                sprintf(tmp, "\n%s %d", GetString(0x78), -power);
                if (PopUpInfoTile[0xd] & 1) {
                    // STRING: LEGOLAND 0x004bad38
                    strcat(tmp, "\n");
                    strcat(tmp, GetString(0x7a));
                }
            } else if (power != 0) {
                if (PopUpInfoTile[0x11] >= ride->durability >> 2) {
                    sprintf(tmp, "\n%s %d", GetString(0x79), power);
                } else {
                    // STRING: LEGOLAND 0x004bad34
                    sprintf(tmp, "\n%s", GetString(0x7b));
                }
            }
            if (power != 0) {
                strcat(buf_b, tmp);
            }
        }
        if (ride->durability != 0) {
            repairable = 1;
        }
        if (!(PopUpInfoTile[0xc] & 0x40)) {
            can_delete = 1;
        }
        break;
    case 0x14:
        sprintf(buf_a, (char *)PercentSFormat, ride->name);
        // STRING: LEGOLAND 0x004bad2c
        sprintf(buf_b, "%s : %d", GetString(0x93), GetMechanicCount());
        // STRING: LEGOLAND 0x004bad1c
        sprintf(tmp, "\n%s %d\n%s %d", GetString(0x76), GetObjRepairCost(ride, PopUpInfoTile[0x11]), GetString(0x77), GetObjSalvageValue(ride, PopUpInfoTile[0x11]));
        strcat(buf_b, tmp);
        add_mechanic = 1;
        if (!(PopUpInfoTile[0xc] & 0x40)) {
            can_delete = 1;
        }
        break;
    case 0xa:
        sprintf(buf_a, (char *)PercentSFormat, ride->name);
        sprintf(buf_b, "%s : %d", GetString(0x91), GetGardenerCount());
        sprintf(tmp, "\n%s %d\n%s %d", GetString(0x76), GetObjRepairCost(ride, PopUpInfoTile[0x11]), GetString(0x77), GetObjSalvageValue(ride, PopUpInfoTile[0x11]));
        strcat(buf_b, tmp);
        add_gardener = 1;
        if (!(PopUpInfoTile[0xc] & 0x40)) {
            can_delete = 1;
        }
        break;
    case 0x104:
        sprintf(buf_a, (char *)PercentSFormat, ride->name);
        sprintf(buf_b, (char *)PercentSFormat, GetString(0xa0));
        PopUpShowsBuildProgress = 1;
        break;
    case 0x10b:
        info = PopUpInfoData;
        if (info->field_18 != 0 && info->field_1c->field_60 >= 0x6b) {
            PopUpInfoHover.type = 0x104;
            PopUpInfoSetUp(PopUpInfoHover, PopUpInfoX, PopUpInfoY);
            return;
        }
        sprintf(buf_a, (char *)PercentSFormat, *(char **)info->field_4);
        sprintf(buf_b, (char *)PercentSFormat, GetString(0xd2));
        show_close = 1;
        break;
    case 0x10c:
        info = PopUpInfoData;
        if (info->field_18 != 0 && info->field_1c->field_60 >= 0x6b) {
            PopUpInfoHover.type = 0x104;
            PopUpInfoSetUp(PopUpInfoHover, PopUpInfoX, PopUpInfoY);
            return;
        }
        sprintf(buf_a, (char *)PercentSFormat, *(char **)info->field_4);
        sprintf(buf_b, (char *)PercentSFormat, GetString(0xd3));
        show_close = 1;
        break;
    case 0x306:
        if (bloke->action == 3 && bloke->param_action >= 0xc) {
            ResetInfoStruct();
            return;
        }
        sprintf(buf_a, GetVisitorName(PopUpInfoBloke));
        sprintf(buf_b, EmptyString);
        PopUpFollowingVisitor = 1;
        break;
    }
    if (0 != PopUpInfoResizePending) {
        if (PopUpInfoSubjectType == 0x306) {
            PopupInfoLineCount = 2;
        } else {
            name_lines = CountPopUpTextWidthSteps(buf_a, 0x40, 0x14, 0xb0, 0x20, 1);
            info_lines = CountPopUpTextGrowSteps(buf_b, 0x40, 0x14, 0xb0, 0x20, 2);
            PopupInfoLineCount = info_lines;
            if (info_lines <= name_lines) {
                PopupInfoLineCount = name_lines;
            }
        }
        PopUpInfoResizePending = 0;
    }
    ClampPopUpInfoPosition(*(unsigned int *)&PopupInfoLineCount & 0xff);
    size = *(unsigned int *)&PopupInfoLineCount & 0xff;
    DrawPopUpInfoBackground();
    PushRenderingStatusAndUnlockVideoSurface();
    x = PopUpInfoX;
    y = PopUpInfoY;
    if (buf_a) {
        tl.x = x + 0xc;
        tl.y = y + 6;
        br.x = size * 32 + x + 0xbc;
        br.y = y + 0x19;
        PrintTextBox(buf_a, tl.x, tl.y, br.x - tl.x, br.y - tl.y, 1, 1, 0xff0000, 0xffffff);
    }
    if (buf_b) {
        tl.x = x + 0xc;
        tl.y = y + 0x23;
        br.x = size * 32 + x + 0xbc;
        br.y = y + size * 20 + 0x63;
        PrintTextBox(buf_b, tl.x, tl.y, br.x - tl.x, br.y - tl.y, 2, 0x10, 0xff0000, 0xffffff);
    }
    PopRenderingStatus();
    if (PopUpInfoSubjectType == 0x306) {
        mood = GetBlokeMood(bloke);
        hunger = GetBlokeHungerLevel(bloke);
        tl.x = x + 0xc;
        tl.y = y + 0x23;
        br.x = size * 32 + x + 0xb0;
        br.y = y + size * 20 + 0x63;
        width = br.x - tl.x;
        mid = (br.y + tl.y) / 2;
        row_y = mid + 0x22;
        PrintTextBox(GetString(0x8e), tl.x, row_y, width / 2, 0x14, 2, 0x11, 0xff0000, 0xffffff);
        PrintTextBox(GetString(0x8f), (br.x + tl.x) / 2, row_y, width / 2, 0x14, 2, 0x11, 0xff0000, 0xffffff);
        if (mood == 3) {
            row_y = mid - 0x20;
            PrintSprite(ISadSprite, width / 4 + tl.x - 0x20, row_y, 0, 0);
        } else if (mood == 2) {
            row_y = mid - 0x20;
            PrintSprite(IHappySprite, width / 4 + tl.x - 0x20, row_y, 0, 0);
        } else {
            row_y = mid - 0x20;
            PrintSprite(INormSprite, width / 4 + tl.x - 0x20, row_y, 0, 0);
        }
        if (hunger == 0) {
            PrintSprite(IFullSprite, br.x - width / 4 - 0x20, row_y, 0, 0);
        } else if (hunger == 1) {
            PrintSprite(IPeckishSprite, br.x - width / 4 - 0x20, row_y, 0, 0);
        } else {
            PrintSprite(IHungrySprite, br.x - width / 4 - 0x20, row_y, 0, 0);
        }
    }
    if (repairable != 0 || PopUpShowsBuildProgress != 0) {
        if (repairable != 0) {
            frac = (float)PopUpInfoTile[0x11] / ride->durability;
        } else {
            target = PopUpInfoHover.data.tile;
            entry_index = 0;
            for (entry = BuildObjArray;; entry++, entry_index++) {
                if ((INT_PTR)&entry->coords >= (INT_PTR)&ButtonRepeatDelay) {
                    return;
                }
                if (entry->coords.id == target.id) {
                    break;
                }
            }
            if (entry_index >= 0x100) {
                return;
            }
            frac = (float)BuildObjArray[entry_index].elapsed / GetBuildTime(PopUpInfoRide);
            if (frac == 1.0f) {
                PopUpInfoHover.type = 0x103;
                PopUpInfoSetUp(PopUpInfoHover, PopUpInfoX, PopUpInfoY);
                return;
            }
        }
        bar_x = x + 6;
        bar_y = y + size * 20 + 0x6f;
        bar_w = size * 32 + 0xbc;
        RenderBlock(bar_x, bar_y, bar_w, 6, 0);
        if (frac < 0.25 && repairable != 0) {
            RenderBlock(bar_x, bar_y, (int)(bar_w * frac), 6, GetNearestColour(0xff, 0, 0));
        } else {
            RenderBlock(bar_x, bar_y, (int)(bar_w * frac), 6, GetNearestColour(0, 0xff, 0));
        }
    }
    icon_y = y + (size * 5 + 0x1e) * 4;
    icon_x = size * 32 + x + 0xc8;
    ClosePopUpIcon->flags = ClosePopUpIcon->flags & 0xfffffbff;
    ClosePopUpIcon->x = icon_x - 0x27;
    ClosePopUpIcon->y = icon_y;
    last_x = ClosePopUpIcon->x;
    if (show_close != 0) {
        PopUpDeleteIcon->flags = PopUpDeleteIcon->flags & 0xfffffbff;
        PopUpDeleteIcon->x = icon_x - 0x4e;
        PopUpDeleteIcon->y = icon_y;
        last_x = PopUpDeleteIcon->x;
    }
    if (can_delete != 0) {
        if (IsPopupObjectBuildable() != 0) {
            DeleteObjectIcon->flags = 0xfffffbff & DeleteObjectIcon->flags;
            DeleteObjectIcon->x = icon_x - 0x4e;
            DeleteObjectIcon->y = icon_y;
            last_x = DeleteObjectIcon->x;
        }
    }
    if (add_gardener != 0) {
        AddGardenerIcon->flags = 0xfffffbff & AddGardenerIcon->flags;
        AddGardenerIcon->y = icon_y;
        if (can_delete != 0) {
            AddGardenerIcon->x = icon_x - 0x75;
        } else {
            AddGardenerIcon->x = icon_x - 0x4e;
        }
        last_x = AddGardenerIcon->x;
    }
    if (add_mechanic != 0) {
        AddMechanicsIcon->flags = AddMechanicsIcon->flags & 0xfffffbff;
        AddMechanicsIcon->y = icon_y;
        if (can_delete != 0) {
            AddMechanicsIcon->x = icon_x - 0x75;
        } else {
            AddMechanicsIcon->x = icon_x - 0x4e;
        }
        last_x = AddMechanicsIcon->x;
    }
    icon_bottom = icon_y + 0x1b;
    PuCornerMaskIcon->x = last_x;
    PuCornerMaskIcon->y = icon_y;
    PuCornerMaskIcon->flags = PuCornerMaskIcon->flags & 0xfffffbff;
    left_bound = PopUpDeleteConfirm != 0 ? ClosePopUpIcon->x : PuCornerMaskIcon->x;
    if (ClosePopUpIcon->x + 0x24 < MousePos.x || MousePos.x < left_bound) {
        SetPopUpIconSprites();
    }
    if (icon_bottom < MousePos.y || MousePos.y < icon_y) {
        SetPopUpIconSprites();
    }
    if (PopUpDeleteConfirm != 0) {
        DrawInfoPopupFooter();
    }
    Empty_00472090();
}

// FUNCTION: LEGOLAND 0x004730f0
unsigned char ClosePopUpEventHandler(void *param1, unsigned char param2, unsigned int param3, unsigned int param4) {
    SetPopUpIconSprites();

    if (param1) {
        SetIconSprite(param1, PuClosePopUpOnSprite);
    }

    if (param2 & 0x2) {
        RemoveAllNewObjects();
        ResetInfoStruct();
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00473130
unsigned int TryClosePopUp(void) {
    if (PopUpInfoState != 0) {
        ClosePopUpEventHandler(NULL, 0x2, 0, 0);
        return 1;
    } else {
        return 0;
    }
}

// FUNCTION: LEGOLAND 0x00473160
unsigned int HandlePopUpOkKey(void) {
    unsigned int state = PopUpInfoState;

    if (state == 1) {
        ResetInfoStruct();
        return 1;
    }

    if (state == 2) {
        NextIconEventHandler(NULL, state, 0, 0);
        return 1;
    }

    return 0;
}

// FUNCTION: LEGOLAND 0x004731a0
unsigned char DeleteObjectEventHandler(void *param_1, unsigned char param_2) {
    SetPopUpIconSprites();
    SetIconSprite(param_1, PuDeleteObjectOnSprite);
    if ((param_2 & 2) != 0) {
        DisablePopUpIconHandlers();
        PopUpDeleteConfirm = 1;
        return 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004731e0
unsigned char PopUpOkEventHandler(void *param_1, unsigned char flags) {
    unsigned int local_8[2];
    struct Cursor local_cursor;
    void *saved_class;
    struct Ride *cls;
    unsigned int v;

    SetPopUpOkCloseIconSprites();
    SetIconSprite(param_1, PUOKOnSprite);
    if ((flags & 2) != 0) {
        v = PopUpInfoHover.data.value & 0xffff;
        saved_class = QueryClass;
        memcpy(&local_cursor, &QueryCursor, sizeof(struct Cursor));
        local_8[0] = v & 0xff;
        local_8[1] = v >> 8;
        cls = ((struct Element *)PopUpInfoHover.ptr)->ride;
        QueryCursor.tile_y = local_8[1];
        QueryObj.pos.y = (unsigned char)local_8[1];
        QueryClass = cls;
        QueryCursor.tile_x = local_8[0];
        QueryObj.pos.x = (unsigned char)local_8[0];
        cls->cb_94(cls->element, local_8);
        BuildCursorPtr(&QueryCursor, 0, 0);
        if ((int)IsCursorBuildable(&QueryCursor) != 0) {
            ClearPathUnderFootprint(QueryClass, local_8);
            cls = QueryClass;
            RemObjFromMap(cls, cls->element, QueryObj, &QueryCursor);
        }
        memcpy(&QueryCursor, &local_cursor, sizeof(struct Cursor));
        QueryClass = saved_class;
        ResetInfoStruct();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00473310
unsigned char CBCloseEventHandler(void *param1, unsigned char param2) {
    SetPopUpOkCloseIconSprites();
    SetIconSprite(param1, CBCloseOnSprite);

    if (param2 & 0x2) {
        PopUpInfoOkIcon->flags |= 0x400;
        CBCloseIcon->flags |= 0x400;
        EnablePopUpIconHandlers();
        PopUpDeleteConfirm = 0;
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x00473360
unsigned char NextIconEventHandler(void *arg1, unsigned char flags, unsigned int arg3, unsigned int arg4) {
    SetPopUpOkCloseIconSprites();
    if (arg1) {
        SetIconSprite(arg1, NextIconOnSprite);
    }
    if (flags & 2) {
        if (NewObjects.current < (int)(NewObjects.count - 1)) {
            NewObjects.current++;
        } else {
            ResetInfoStruct();
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004733b0
unsigned char PrevIconEventHandler(void *arg0, unsigned char flags) {
    SetPopUpOkCloseIconSprites();
    SetIconSprite(arg0, PrevIconOnSprite);

    if (flags & 0x2) {
        if (NewObjects.current > 0) {
            NewObjects.current--;
        }
    }

    return 1;
}

// FUNCTION: LEGOLAND 0x004733f0
unsigned char AddGardenerEventHandler(void *param_1, unsigned char param_2) {
    unsigned int local_8[2];

    SetPopUpIconSprites();
    if (GardenerCount < 0xf) {
        SetIconSprite(param_1, PuAddGardenerOnSprite);
        if ((param_2 & 2) != 0) {
            local_8[0] = PopUpInfoTile[4];
            local_8[1] = PopUpInfoTile[5];
            if (BuyGardener() != 0) {
                GenerateGardener(local_8, 1);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00473460
unsigned char AddMechanicsEventHandler(void *param_1, unsigned char param_2) {
    unsigned int local_8[2];

    SetPopUpIconSprites();
    if (MechanicCount < 0xf) {
        SetIconSprite(param_1, PuAddMechanicsOnSprite);
        if ((param_2 & 2) != 0) {
            local_8[0] = PopUpInfoTile[4];
            local_8[1] = PopUpInfoTile[5];
            if (BuyMechanic() != 0) {
                GenerateMechanic(local_8, 1);
            }
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004734d0
unsigned char DeleteWorkOrderEventHandler(void *param_1, unsigned char param_2) {
    SetPopUpIconSprites();
    SetIconSprite(param_1, PuDeleteObjectOnSprite);
    if ((param_2 & 2) != 0) {
        if (PopUpInfoSubjectType == 0x10b) {
            SetObjRectFlags(PopUpInfoData->field_4, (struct Point *)&PopUpInfoData->field_8, 0);
            RemoveGardenersWorkOrderAt(PopUpInfoData->field_8, PopUpInfoData->field_c);
            ResetInfoStruct();
            return 1;
        }
        SetObjRectFlags(PopUpInfoData->field_4, (struct Point *)&PopUpInfoData->field_8, 0);
        RemoveMechanicsWorkOrderAt(PopUpInfoData->field_8, PopUpInfoData->field_c);
        ResetInfoStruct();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004735b0
void ClearAdvicePriority(void) {
    CurrentAdvicePriority = 0;
}

// FUNCTION: LEGOLAND 0x004735e0
unsigned int TryPlayAdviceSpeech(unsigned int param) {
    struct InfoTimedEntry *entry;
    unsigned long now;

    entry = &AdviceSpeechEntries[param];
    if ((int)CurrentAdvicePriority < (int)param) {
        now = GetTicks();
        if ((int)(entry->interval + entry->last_time) < (int)now) {
            if (PlaySpeech(entry->sample) != 0) {
                CurrentAdvicePriority = param;
                entry->last_time = now;
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00473640
unsigned int PlayCantBuildAdvice(unsigned int param_1) {
    return TryPlayAdviceSpeech(AdviceSpeechIds[param_1]);
}

// FUNCTION: LEGOLAND 0x00473660
void UpdateAdvicePriority(void) {
    if (CurrentAdvicePriority == 0) {
        return;
    }
    if (SpeechIsPlaying() != 0) {
        return;
    }
    ClearAdvicePriority();
}

// FUNCTION: LEGOLAND 0x00473680
void ShowPopUpMessage(int param_1, int param_2, char *param_3, void *param_4, void *param_5) {
    strcpy(PopUpMessageText, param_3);
    PopUpMessageX = param_1;
    PopUpMessageY = param_2;
    CreatePopUpInfoButtons(param_4, param_5);
    PopUpMessageShown = 1;
}

// FUNCTION: LEGOLAND 0x004736e0
void HidePopUpMessage(void) {
    KillPUOKAndCBSprites();
    PopUpMessageShown = 0;
}

// FUNCTION: LEGOLAND 0x004736f0
void DrawPopUpMessage(void) {
    int x;
    int y;
    int i;
    RECT rc;
    RECT hit;

    if (PopUpMessageShown != 0) {
        struct PrintCtx ctx = {1};

        y = PopUpMessageY;
        x = PopUpMessageX;
        /* the bar: left end, PopUpMessageCentrePieces centre pieces, right end */
        PrintSprite(CBBGLeftSprite, x, y, 0, (int *)&ctx);
        x += 0x7a;
        for (i = 0; i < PopUpMessageCentrePieces; i++) {
            PrintSprite(CBBGCentreSprite, x, y, 0, (int *)&ctx);
            x += 0x20;
        }
        PrintSprite(CBBGRightSprite, x, y, 0, (int *)&ctx);
        x += 0x4e;
        PopUpInfoOkIcon->flags &= ~0x400;
        PopUpInfoOkIcon->x = (short)(x - 0x4b);
        PopUpInfoOkIcon->y = (short)PopUpMessageY + 3;
        CBCloseIcon->flags &= ~0x400;
        CBCloseIcon->x = (short)(x - 0x27);
        CBCloseIcon->y = (short)PopUpMessageY + 3;
        rc.left = PopUpMessageX + 0xc;
        rc.top = PopUpMessageY + 6;
        rc.right = rc.left + PopUpMessageCentrePieces * 0x14 + 0x7a;
        rc.bottom = rc.top + 0x1b;
        PrintTextBox(PopUpMessageText, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 1, 5, 0xff0000, 0xffffff);
        /* hit box of the bar: outside it, reset the OK/close icons */
        hit.left = PopUpInfoOkIcon->x;
        hit.right = CBCloseIcon->x + 0x24;
        hit.top = y;
        hit.bottom = y + 0x1b;
        if (hit.right < (int)MousePos.x || (int)MousePos.x < hit.left) {
            SetPopUpOkCloseIconSprites();
        }
        if (hit.bottom < (int)MousePos.y || (int)MousePos.y < hit.top) {
            SetPopUpOkCloseIconSprites();
        }
    }
}
