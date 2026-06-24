#include "options.h"
#include <string.h>
#include "controller.h"
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "interface.h"
#include "legoland.h"
#include "math.h"
#include "popupinfo.h"
#include "print_sprite.h"
#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "saveload.h"
#include "sound_music.h"
#include "sound_sfx.h"
#include "string.h"

#include "help.h"
#include "image_sprite.h"
#include "stream.h"

// FUNCTION: LEGOLAND 0x0048eb40
void FUN_0048eb40(void) {
    DAT_006687bc = DAT_00798740;
    DAT_006687c0 = DAT_0079873c;
}

// FUNCTION: LEGOLAND 0x0048eb60
LEGO_EXPORT void InitOptionScreen(void) {
    struct IconNode *icon;
    unsigned int flag_mask = 0x6002;
    unsigned int x;

    FUN_00499380();
    DAT_007cb314 = (char)DAT_0080ffc4;
    DAT_007cb31c = (char)DAT_0080ffc8;
    DAT_007cb315 = (char)DAT_0080ffcc;

    // STRING: LEGOLAND 0x004bf49c
    SPRITE_TitleScreenBk = LoadSprite("OptionScreenBK.lls", 0);

    // STRING: LEGOLAND 0x004bf484
    icon = LoadSpriteIcon("Accept_On_Options.lls", 4, 0x13, 0x14f, 7);
    icon->string_id = 0x3c;
    icon->string = GetString(0x3c);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048efd0;
    DAT_006687bc = 0;

    // STRING: LEGOLAND 0x004bf470
    icon = LoadSpriteIcon("Exit_on_Options.lls", 4, 0x1e0, 6, 7);
    icon->string_id = 0x3e;
    icon->string = GetString(0x3e);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f0a0;
    DAT_006687c0 = (unsigned int)FUN_0048f0a0;

    // STRING: LEGOLAND 0x004bf45c
    icon = LoadSpriteIcon("Load_on_Options.lls", 4, 0x168, 0x13a, 7);
    icon->string_id = 0x3f2;
    icon->string = GetString(0x3f2);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f550;

    // STRING: LEGOLAND 0x004bf448
    icon = LoadSpriteIcon("Save_on_Options.lls", 4, 0x1df, 0xbd, 7);
    icon->string_id = 0x3f;
    icon->string = GetString(0x3f);
    icon->event_handler = (void *)FUN_0048f050;
    icon->flags |= flag_mask;

    FUN_0048eb20();

    // STRING: LEGOLAND 0x004bf43c
    icon = LoadSpriteIcon("Down1.lls", 4, 0x19c, 0x2e, 7);
    icon->string_id = 0x46;
    icon->string = GetString(0x46);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f760;
    icon->field_18s = 1;

    // STRING: LEGOLAND 0x004bf434
    icon = LoadSpriteIcon("Up1.lls", 4, 0x26, 0x2e, 7);
    icon->string_id = 0x45;
    icon->string = GetString(0x45);
    icon->event_handler = (void *)FUN_0048f5f0;
    icon->flags |= flag_mask;
    icon->field_18s = 2;

    x = FUN_0048eaf0(DAT_0080ffc4);
    // STRING: LEGOLAND 0x004bf420
    icon = LoadSpriteIcon("VolMarkerSpeech.lls", 4, x, 0x33, 7);
    icon->string_id = 0x47;
    icon->string = GetString(0x47);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f8d0;
    icon->field_18s = 3;
    DAT_00798748 = icon;

    // STRING: LEGOLAND 0x004bf414
    icon = LoadSpriteIcon("Down2.lls", 4, 0x19c, 0x72, 7);
    icon->string_id = 0x49;
    icon->string = GetString(0x49);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f760;
    icon->field_18s = 4;

    // STRING: LEGOLAND 0x004bf40c
    icon = LoadSpriteIcon("Up2.lls", 4, 0x26, 0x72, 7);
    icon->string_id = 0x48;
    icon->string = GetString(0x48);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f5f0;
    icon->field_18s = 5;

    x = FUN_0048eaf0(DAT_0080ffc8);
    // STRING: LEGOLAND 0x004bf3f8
    icon = LoadSpriteIcon("VolMarkerMusic.lls", 4, x, 0x77, 7);
    icon->string_id = 0x4a;
    icon->string = GetString(0x4a);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f8d0;
    icon->field_18s = 6;
    DAT_00798750 = icon;

    // STRING: LEGOLAND 0x004bf3ec
    icon = LoadSpriteIcon("Down3.lls", 4, 0x19c, 0xb6, 7);
    icon->string_id = 0x4c;
    icon->string = GetString(0x4c);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f760;
    icon->field_18s = 7;

    // STRING: LEGOLAND 0x004bf3e4
    icon = LoadSpriteIcon("Up3.lls", 4, 0x26, 0xb6, 7);
    icon->string_id = 0x4b;
    icon->string = GetString(0x4b);
    icon->event_handler = (void *)FUN_0048f5f0;
    icon->flags |= flag_mask;
    icon->field_18s = 8;

    x = FUN_0048eaf0(DAT_0080ffcc);
    // STRING: LEGOLAND 0x004bf3d4
    icon = LoadSpriteIcon("VolMarkerFX.lls", 4, x, 0xbb, 7);
    icon->string_id = 0x4d;
    icon->string = GetString(0x4d);
    icon->flags |= flag_mask;
    icon->event_handler = (void *)FUN_0048f8d0;
    icon->field_18s = 9;
    DAT_0079874c = icon;

    DAT_007cb320 = 0;
    DAT_00798754 = 0;
}

// FUNCTION: LEGOLAND 0x0048ef10
unsigned char FUN_0048ef10(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        UpDateCurrentProfile();
        DAT_0080ff80.unk8 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ef40
unsigned char FUN_0048ef40(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ffc4 = DAT_007cb314;
        DAT_0080ffc8 = DAT_007cb31c;
        DAT_0080ffcc = DAT_007cb315;
        DAT_0080ff80.unk8 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048ef90
unsigned char FUN_0048ef90(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        CloseFontEndCheckBox();
        EditMode.unk4 = 0;
        RemoveIconGroup(7);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048efd0
unsigned char FUN_0048efd0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        FUN_00498920();
        DAT_006687b0 = 4;
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_0080ffe4 != 0) {
            UpDateCurrentSaveSlotInfo();
        }
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
        KillTitleScreenSprites();
        EditMode.unk4 = 3;
        FUN_00474880();
        UpDateCurrentProfile();
        FUN_004993c0();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f050
unsigned char FUN_0048f050(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb328 = 0;
        DAT_007cb324 = 0;
        DAT_0080ff80.unk8 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f0a0
unsigned char FUN_0048f0a0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        InitExitCheckBox(0xfa, 0x28);
        DAT_007cb320 = 1;
        DAT_004bef9c = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f0f0
LEGO_EXPORT void InitExitCheckBox(unsigned int param_1, unsigned int param_2) {
    short y_offset;
    struct IconNode *icon;

    FUN_0048eb20();
    DAT_00798678 = LoadSprite("PU_OK.lls", 4);
    DAT_00798674 = LoadSprite("PU_OKON.lls", 4);
    DAT_0079867c = LoadSprite("PU_ClosePopUp.lls", 4);
    DAT_00798680 = LoadSprite("PU_ClosePopUpON.lls", 4);
    DAT_00798684 = LoadSprite("PU_ClosePopUp.lls", 4);
    DAT_00798688 = LoadSprite("PU_ClosePopUpON.lls", 4);

    for (;;) {
        if (DAT_0080ff80.unk8 == 5) {
            DAT_006687bc = (unsigned int)FUN_0048f440;
            if (FUN_0047f820() > 0xea60) {
                DAT_00798754 = 1;
                // STRING: LEGOLAND 0x004bf4c4
                icon = LoadSpriteIcon("PU_BigPopupBK.lls", 4, param_1, param_2, 0xe);
                y_offset = 0xdc;
                break;
            }
            DAT_00798754 = 0;
        } else {
            DAT_006687bc = (unsigned int)FUN_0048ef90;
            DAT_00798754 = 0;
        }
        // STRING: LEGOLAND 0x004bf4b0
        icon = LoadSpriteIcon("PU_InfoPopupBK.lls", 4, param_1, param_2, 0xe);
        y_offset = 0x78;
        break;
    }
    DAT_007986d8 = InsertIcon(icon->x + 0x7d, icon->y + y_offset, 0xe, DAT_00798678);
    DAT_007986d8->string_id = 0x4e;
    DAT_007986d8->string = GetString(0x4e);
    DAT_007986d8->flags |= 0x2000;
    DAT_007986d8->flags |= 0x4002;
    DAT_007986d8->event_handler = (void *)DAT_006687bc;

    DAT_007986dc = InsertIcon(DAT_007986d8->x + 0x24, DAT_007986d8->y, 0xe, DAT_0079867c);
    DAT_007986dc->string_id = 4;
    DAT_007986dc->string = GetString(4);
    DAT_007986dc->flags |= 0x2000;
    DAT_007986dc->flags |= 0x4002;
    DAT_007986dc->event_handler = (void *)FUN_0048f4b0;
    DAT_007cb318 = 0;
    DAT_007cb310 = 0;
    DAT_006687c0 = (unsigned int)FUN_0048f4b0;
}

// FUNCTION: LEGOLAND 0x0048f2d0
LEGO_EXPORT void PrintExitCheckBox(void) {
    RECT rc;

    if (DAT_007cb320 != 0) {
        PushRenderingStatusAndUnlockVideoSurface();

        rc.left = 0x106;
        rc.top = 0x2e;
        rc.right = 0x1b6;
        rc.bottom = 0x47;
        if (DAT_007cb318 != 0) {
            InfoPrintCent(strlen(GetString(0x88)), GetString(0x88), 1, rc, 1);
        } else {
            InfoPrintCent(strlen(GetString(0x89)), GetString(0x89), 2, rc, 1);
        }

        rc.left = 0x106;
        rc.top = 0x4b;
        rc.right = 0x1b6;
        rc.bottom = 0xae;
        if (DAT_007cb318 != 0) {
            InfoPrintCent(strlen(GetString(0x8a)), GetString(0x8a), 1, rc, 1);
        } else if (DAT_00798754 != 0) {
            rc.bottom = 0x112;
            InfoPrintCent(strlen(GetString(0x884)), GetString(0x884), 1, rc, 1);
        } else {
            InfoPrintCent(strlen(GetString(0x8b)), GetString(0x8b), 1, rc, 1);
        }

        PopRenderingStatus();
        UpdateProfileCheckBoxIcons();
    }
}

// FUNCTION: LEGOLAND 0x0048f440
unsigned char FUN_0048f440(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        DAT_00832ba0 = 3;
        DAT_00668e38 = 0;
        RemoveIconGroup(7);
        FUN_0046fb40(0xd2);
        KillTitleScreenSprites();
        EditMode.unk4 = 3;
        FUN_0048eb40();
        FUN_00474880();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f4b0
unsigned char FUN_0048f4b0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(DAT_004b929c, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        FUN_0048eb40();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f4f0
unsigned char FUN_0048f4f0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb320 = 0;
        CloseFontEndCheckBox();
        DAT_007cb310 = 1;
        DAT_007cb328 = 0;
        DAT_007cb324 = 0;
        DAT_0080ff80.unk8 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f550
unsigned char FUN_0048f550(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_007cb328 = 1;
        DAT_007cb324 = 0;
        DAT_0080ff80.unk8 = 4;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5a0
unsigned char FUN_0048f5a0(unsigned int param_1, unsigned int param_2) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        StoreNewSaveGameToDisk();
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5d0
unsigned char FUN_0048f5d0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f5f0
unsigned char FUN_0048f5f0(struct IconNode *param_1, unsigned int param_2) {
    unsigned char pos;
    struct Sample *sample;
    unsigned int result;

    if (DAT_004bef9c != 0) {
        if ((param_2 & 1) != 0) {
            PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        }
        if ((param_2 & 4) != 0) {
            if (param_1->field_18s == 8) {
                sample = PlayInstanceOfSample(PTR_004b92c0, 1, 1, 0);
            } else {
                sample = 0;
            }
            while ((DAT_00813ac4 & 4) != 0) {
                if (GetIconAtPos(&DAT_00813a44, &pos) != param_1) {
                    break;
                }
                switch ((int)(unsigned short)param_1->field_18s) {
                case 2:
                    if (DAT_0080ffc4 != 0) {
                        DAT_0080ffc4--;
                        result = FUN_0048eaf0(DAT_0080ffc4);
                        DAT_00798748->x = (short)result;
                        if (!DAT_006687b4 && !FUN_00498cf0()) {
                            FUN_0046d230(-2);
                        }
                        FUN_0046d230(param_1->string_id);
                    }
                    break;
                case 5:
                    if (DAT_0080ffc8 != 0) {
                        DAT_0080ffc8--;
                        result = FUN_0048eaf0(DAT_0080ffc8);
                        DAT_00798750->x = (short)result;
                    }
                    break;
                case 8:
                    if (DAT_0080ffcc != 0) {
                        DAT_0080ffcc--;
                        result = FUN_0048eaf0(DAT_0080ffcc);
                        DAT_0079874c->x = (short)result;
                    }
                    break;
                }
                FUN_0046d110();
                RenderScreen();
                FUN_00498b40();
            }
            if (sample != 0) {
                KillPlayableSample(sample);
            }
            return 2;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f760
unsigned char FUN_0048f760(struct IconNode *param_1, unsigned int param_2) {
    unsigned char pos;
    struct Sample *sample;
    unsigned int result;

    if (DAT_004bef9c != 0) {
        if ((param_2 & 1) != 0) {
            PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        }
        if ((param_2 & 4) != 0) {
            if (param_1->field_18s == 7) {
                sample = PlayInstanceOfSample(PTR_004b92c0, 1, 1, 0);
            } else {
                sample = 0;
            }
            while ((DAT_00813ac4 & 4) != 0) {
                if (GetIconAtPos(&DAT_00813a44, &pos) != param_1) {
                    break;
                }
                switch ((int)(unsigned short)param_1->field_18s) {
                case 1:
                    if (DAT_0080ffc4 < 0x64) {
                        DAT_0080ffc4++;
                        result = FUN_0048eaf0(DAT_0080ffc4);
                        DAT_00798748->x = (short)result;
                        if (!DAT_006687b4 && !FUN_00498cf0()) {
                            FUN_0046d230(-2);
                        }
                        FUN_0046d230(param_1->string_id);
                    }
                    break;
                case 4:
                    if (DAT_0080ffc8 < 0x64) {
                        DAT_0080ffc8++;
                        result = FUN_0048eaf0(DAT_0080ffc8);
                        DAT_00798750->x = (short)result;
                    }
                    break;
                case 7:
                    if (DAT_0080ffcc < 0x64) {
                        DAT_0080ffcc++;
                        result = FUN_0048eaf0(DAT_0080ffcc);
                        DAT_0079874c->x = (short)result;
                    }
                    break;
                }
                FUN_0046d110();
                RenderScreen();
                FUN_00498b40();
            }
            if (sample != 0) {
                KillPlayableSample(sample);
            }
            return 2;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f8d0
unsigned char FUN_0048f8d0(struct IconNode *param_1, unsigned int param_2) {
    struct Sample *sample;
    int initial_offset;
    int x;
    unsigned int result;

    if (DAT_004bef9c != 0 && (param_2 & 4) != 0) {
        if (param_1->field_18s == 9) {
            sample = PlayInstanceOfSample(PTR_004b92c0, 1, 1, 0);
        } else {
            sample = 0;
        }
        initial_offset = DAT_00813a44.x - param_1->x;
        while ((DAT_00813ac4 & 4) != 0) {
            short min = 0x7c;
            x = DAT_00813a44.x - initial_offset;
            param_1->x = (short)x;
            if (param_1->x > 0x16d) {
                param_1->x = 0x16d;
            }
            if (param_1->x < min) {
                param_1->x = min;
            }
            result = FUN_0048eac0(param_1->x);
            switch ((unsigned int)(unsigned short)param_1->field_18s) {
            case 3:
                DAT_0080ffc4 = result;
                if (!DAT_006687b4 && !FUN_00498cf0()) {
                    FUN_0046d230(-2);
                }
                FUN_0046d230(param_1->string_id);
                break;
            case 6:
                DAT_0080ffc8 = result;
                break;
            case 9:
                DAT_0080ffcc = result;
                break;
            }
            FUN_0046d110();
            RenderScreen();
            FUN_00498b40();
        }
        if (sample != 0) {
            KillPlayableSample(sample);
        }
        return 2;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048f9f0
void FUN_0048f9f0(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3) {
    *param_1 = DAT_00668e38;
    *param_3 = EditMode;
    *param_2 = DAT_0080ff80;
}

// FUNCTION: LEGOLAND 0x0048fa40
unsigned int FUN_0048fa40(unsigned int *param_1, struct ScreenMode *param_2, struct EditState *param_3) {
    unsigned int ret;
    ret = DAT_0080ff80.unk8;
    DAT_00668e38 = *param_1;
    EditMode = *param_3;
    DAT_0080ff80 = *param_2;
    DAT_0080ff80.unk4 = ret;
    return ret;
}

// FUNCTION: LEGOLAND 0x0048faa0
LEGO_EXPORT void KillTitleScreenSprites(void) {
    if (SPRITE_TitleScreenBk != 0) {
        KillSprite(SPRITE_TitleScreenBk);
        SPRITE_TitleScreenBk = 0;
    }
    if (DAT_007986b8 != 0) {
        KillSprite(DAT_007986b8);
        DAT_007986b8 = 0;
    }
    DAT_007986d8 = 0;
    DAT_007986dc = 0;
    DAT_007cb360 = 0;
    DAT_007986e0 = 0;
}

// FUNCTION: LEGOLAND 0x0048faf0
LEGO_EXPORT void RenderScreen(void) {
    int locals[3] = {1};
    UpdateSoundVols();
    ReadGameButtons();
    PushRenderingStatusAndLockVideoSurface();
    PrintSprite(SPRITE_TitleScreenBk, 0, 0, 0, locals);
    if (DAT_00668e38 != 0) {
        RenderIcons2(7, 0xe, 0);
        UpdateFocussedIconPtr();
        ProcessFrontEndHelp();
        PopRenderingStatus();
        RenderingComplete();
    } else {
        RenderIcons();
        ProcessFrontEndHelp();
        PopRenderingStatus();
        RenderingComplete();
    }
}

// FUNCTION: LEGOLAND 0x0048fb80
unsigned char FUN_0048fb80(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    if (DAT_004bef9c != 0 && (param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        if (DAT_00668e38 != 0) {
            DAT_00668e38 = 0;
        }
        DAT_0080ff80.unk8 = 1;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048fbd0
unsigned char FUN_0048fbd0(unsigned int param_1, unsigned int param_2) {
    if ((param_2 & 2) != 0) {
        PlayInstanceOfSample(PTR_004b92c0, 0, 1, 0);
        DAT_0080ff80.unk8 = 5;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x0048fc00
void FUN_0048fc00(void) {
    if (DAT_0080ff80.unk0 != 0) {
        if (FUN_0046d280(DAT_0080ff80.unk0) != 0) {
            DAT_0080ff80.unk0 = 0;
        }
    }
}

// FUNCTION: LEGOLAND 0x0048fc30
int FUN_0048fc30(void) {
    return DAT_0080ffd9 != 0;
}
