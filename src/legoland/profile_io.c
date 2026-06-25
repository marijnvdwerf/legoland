#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include <windows.h>
#include <ddraw.h>

#include "clipping.h"
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "input.h"
#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "text.h"
#include "timer.h"
#include "title.h"

#pragma intrinsic(memset, memcpy, strcpy, strlen)

struct ProfileSprite {
    unsigned char pad_0[0xc];
    short field_c;
    short field_e;
};

struct RideState {
    unsigned char pad_0[0x18];
    unsigned char var_18;
};

struct ProfileObj {
    struct ProfileObj *next;
    unsigned int var_4;
    unsigned int var_8;
    unsigned int var_c;
    unsigned int var_10;
    unsigned int var_14;
    unsigned int var_18;
    unsigned short var_1c;
    unsigned char pad_1e[2];
    unsigned int var_20;
    unsigned int var_24;
    unsigned int var_28;
    unsigned int var_2c;
    unsigned int var_30;
    unsigned int var_34;
};

// FUNCTION: LEGOLAND 0x004912e0
LEGO_EXPORT void ResetTempProfile(void) {
    DAT_007cad60.field_28 = 0x4b;
    DAT_007cad60.field_2c = 0x4b;
    DAT_007cad60.field_30 = 0x4b;
    DAT_007cad60.field_34 = 0;
    DAT_007cad60.field_38 = 0;
    DAT_007cad60.field_3c = 0;
    DAT_007cad60.field_40 = 0;
    DAT_007cad60.name[0] = 0;
    DAT_007cad60.field_20 = 5;
    DAT_007cad60.name_len = 0;
    DAT_007cad60.field_42 = 0;
    memset(DAT_007cad60.field_43, 0, 200);
    *(int *)&DAT_007cad60.field_10b = 0;
    DAT_007cad60.field_10b = 1;
    FUN_0048a780(DAT_007cad60.field_43);
}

// FUNCTION: LEGOLAND 0x00491360
LEGO_EXPORT int Goto_ProfileDir(void) {
    struct _finddata_t find_data;
    long handle;
    int found;
    int next;

    found = 0;
    handle = _findfirst("profiles", &find_data);
    next = (int)handle;
    while (next != -1) {
        if ((find_data.attrib & 0x10) != 0) {
            found = 1;
        }
        next = _findnext(handle, &find_data);
    }
    _findclose(handle);
    if (!found) {
        return _chdir("profiles") == 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004913e0
LEGO_EXPORT int ReturnFrom_ProfileDir(void) { return 1; }

// FUNCTION: LEGOLAND 0x004913f0
LEGO_EXPORT char ScanForProfiles(void) {
    char path[120];
    char count;
    int index;
    int remaining;
    void *stream;

    count = 0;
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    index = 1;
    remaining = 7;
    do {
        // STRING: LEGOLAND 0x004bf718
        sprintf(path, "profiles\\Profile%d.txt", index);
        stream = fopen(path, "r");
        if (stream == 0) {
            // STRING: LEGOLAND 0x004bf6fc
            printf("\ncannot open output file");
        } else {
            count++;
            fclose(stream);
        }
        index++;
        remaining--;
    } while (remaining != 0);
    if (ReturnFrom_ProfileDir() == 0) {
        return -1;
    }
    return count;
}

// FUNCTION: LEGOLAND 0x00491470
LEGO_EXPORT char LoadProfilesFormDisk(void) {
    char path[120];
    struct ProfileData data;
    char slot;
    char result;
    int index;
    void *stream;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    slot = 8;
    index = 8;
    do {
        sprintf(path, "profiles\\Profile%d.txt", index);
        stream = fopen(path, "r");
        if (stream == 0) {
            printf("\ncannot open output file");
            AddNodeToProfileList(0, &data, slot);
        } else {
            fread(&data, sizeof(struct ProfileData), 1, stream);
            AddNodeToProfileList(1, &data, slot);
            fclose(stream);
        }
        slot--;
        index--;
    } while (slot != 0);
    result = (ReturnFrom_ProfileDir() != 0) - 1;
    return result;
}

// FUNCTION: LEGOLAND 0x00491540
unsigned int FUN_00491540(void) { return DAT_007cad60.name[0] != 0; }

// FUNCTION: LEGOLAND 0x00491550
LEGO_EXPORT char UpDateCurrentSaveSlotInfo(void) {
    struct ProfileData temp;
    char path[120];
    void *stream;

    memset(&temp, 0, sizeof(struct ProfileData));
    if (LoadDateIntoTempProfile(DAT_0080ffe3, DAT_0080ffe4 & 0xff) == 0) {
        return -1;
    }
    strcpy(temp.name, DAT_007cad60.name);
    temp.field_28 = DAT_0080ffa0.field_24;
    temp.field_2c = DAT_0080ffa0.field_28;
    temp.field_30 = DAT_0080ffa0.field_2c;
    temp.field_24 = DAT_0080ffe5;
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\%dsave%d.sh", DAT_0080ffe3, DAT_0080ffe4 & 0xff);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
    } else {
        fwrite(&temp, sizeof(struct ProfileData), 1, stream);
        fclose(stream);
    }
    return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
}

// FUNCTION: LEGOLAND 0x00491680
LEGO_EXPORT char UpDateCurrentProfile(void) {
    struct ProfileData temp;
    char path[120];
    void *stream;

    memset(&temp, 0, sizeof(struct ProfileData));
    strcpy(temp.name, (char *)&DAT_0080ffa0);
    temp.field_20 = DAT_0080ffa0.field_20;
    temp.field_28 = DAT_0080ffa0.field_24;
    temp.field_2c = DAT_0080ffa0.field_28;
    temp.field_30 = DAT_0080ffa0.field_2c;
    temp.field_34 = *(int *)&DAT_0080ffa0.field_34[0];
    temp.field_38 = *(int *)&DAT_0080ffa0.field_34[4];
    temp.field_3c = *(int *)&DAT_0080ffa0.field_34[8];
    temp.field_40 = *(short *)&DAT_0080ffa0.field_34[12];
    temp.field_42 = DAT_0080ffa0.field_34[14];
    memcpy(temp.field_43, DAT_0080ffe6, 200);
    *(int *)&temp.field_10b = *(int *)&DAT_0080ffa0.field_30[0];
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", DAT_0080ffe3);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
    } else {
        fwrite(&temp, sizeof(struct ProfileData), 1, stream);
        fclose(stream);
    }
    return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
}

// FUNCTION: LEGOLAND 0x004917c0
int FUN_004917c0(int slot) {
    struct ProfileData data;
    char path[120];
    struct ProfileNode *node;
    void *stream;

    node = FUN_004919a0((unsigned char)slot);
    if (node == NULL) {
        return 0;
    }
    memset(&data, 0, sizeof(struct ProfileData));
    strcpy(data.name, node->data.name);
    data.field_20 = node->data.field_20;
    data.field_28 = node->data.field_28;
    data.field_2c = node->data.field_2c;
    data.field_30 = node->data.field_30;
    memcpy(&data.field_34, &node->data.field_34, 15);
    memcpy(data.field_43, node->data.field_43, 200);
    *(int *)&data.field_10b = *(int *)&node->data.field_10b;
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", slot & 0xff);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
        return 0;
    }
    fwrite(&data, sizeof(struct ProfileData), 1, stream);
    fclose(stream);
    return ReturnFrom_ProfileDir() != 0;
}

// FUNCTION: LEGOLAND 0x00491910
LEGO_EXPORT char SaveProfileToDisk(void) {
    char path[120];
    void *stream;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", DAT_0080ffe3);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
        return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
    }
    fwrite(&DAT_007cad60, sizeof(struct ProfileData), 1, stream);
    fclose(stream);
    return ReturnFrom_ProfileDir() != 0 ? 1 : -1;
}

// FUNCTION: LEGOLAND 0x004919a0
struct ProfileNode *FUN_004919a0(unsigned char slot) {
    struct ProfileNode *current = (struct ProfileNode *)DAT_00798890;

    if (current == NULL) {
        return NULL;
    }

    while (1) {
        if (current->slot == slot) {
            return current;
        }
        current = current->next;
        if (current == NULL) {
            return NULL;
        }
    }
}

// FUNCTION: LEGOLAND 0x004919c0
LEGO_EXPORT void AddNodeToProfileList(int load, struct ProfileData *data, char slot) {
    struct ProfileNode *node = (struct ProfileNode *)malloc(sizeof(struct ProfileNode));
    memset(node, 0, sizeof(struct ProfileNode));
    if (load != 0) {
        strcpy(node->data.name, data->name);
        node->data.field_20 = data->field_20;
        node->data.field_28 = data->field_28;
        node->data.field_2c = data->field_2c;
        node->data.field_30 = data->field_30;
        node->slot = slot;
        node->has_header = 1;
        memcpy(&node->data.field_34, &data->field_34, 15);
        memcpy(node->data.field_43, data->field_43, 200);
        *(int *)&node->data.field_10b = *(int *)&data->field_10b;
        node->next = (struct ProfileNode *)DAT_00798890;
        DAT_00798890 = node;
        return;
    }
    node->slot = slot;
    node->next = (struct ProfileNode *)DAT_00798890;
    DAT_00798890 = node;
}

// FUNCTION: LEGOLAND 0x00491ab0
LEGO_EXPORT int RemoveProfile(unsigned char index) {
    char path[120];
    int slot;
    int remaining;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", index);
    remove(path);
    slot = 8;
    remaining = 8;
    do {
        // STRING: LEGOLAND 0x004bf730
        sprintf(path, "profiles\\%dsave%d.sav", index, slot);
        remove(path);
        sprintf(path, "profiles\\%dsave%d.sh", index, slot);
        remove(path);
        slot--;
        remaining--;
    } while (remaining != 0);
    return (ReturnFrom_ProfileDir() != 0) - 1;
}

// FUNCTION: LEGOLAND 0x00491b50
LEGO_EXPORT void DeleteProfileList(void) {
    struct ProfileNode *current = (struct ProfileNode *)DAT_00798890;

    while (current != NULL) {
        struct ProfileNode *next = current->next;
        free(current);
        current = next;
    }

    DAT_00798890 = NULL;
}

// FUNCTION: LEGOLAND 0x00491bd0
LEGO_EXPORT void EnterNewProfile(struct ProfileSprite *sprite) {
    char cursor_str[10];
    unsigned char count;
    char input;
    int left;
    int right;
    int bottom;
    int center_x;
    int top;
    char *blink;

    count = DAT_007cad60.name_len;
    // STRING: LEGOLAND 0x004bf2e4
    *(short *)cursor_str = *(short *)"|";
    input = GetInputChar();
    if (input != '\0') {
        if (input == -1 && count != 0) {
            count--;
            ((char *)&DAT_007cad60)[count] = 0;
        }
        if (count < 0x1f && DAT_00798894 < 0x7b && input > '\0') {
            if (input == ' ') {
                if (count != 0) {
                    ((char *)&DAT_007cad60)[count] = ' ';
                    count++;
                    ((char *)&DAT_007cad60)[count] = 0;
                }
            } else {
                ((char *)&DAT_007cad60)[count] = input;
                count++;
                ((char *)&DAT_007cad60)[count] = 0;
            }
        }
    }
    top = sprite->field_e + 7;
    left = sprite->field_c + 0x14;
    bottom = top + 0x13;
    right = left + 0xc0;
    if (count != 0) {
        RECT rc;
        rc.left = left;
        rc.top = top;
        rc.right = right;
        rc.bottom = bottom;
        center_x = FUN_00491e40(DAT_007cad60.name, 2, rc, 1);
    } else {
        center_x = (right + left) >> 1;
    }
    DAT_00798894 = (center_x - ((right + left) >> 1)) * 2;
    top = sprite->field_e + 5;
    blink = "|";
    if (GetBlink() == 0) {
        // STRING: LEGOLAND 0x004b8ad4
        blink = " ";
    }
    strcpy(cursor_str, blink);
    {
        RECT rc;
        rc.left = center_x;
        rc.top = top;
        rc.right = center_x + 100;
        rc.bottom = bottom;
        FUN_00490fa0(cursor_str, 2, rc, 1);
    }
    DAT_007cad60.name_len = count;
}

// FUNCTION: LEGOLAND 0x00491d60
LEGO_EXPORT void NewPrintCent(char *text, int font, RECT rc, int color_flag) {
    HRGN region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    HDC hdc;
    HGDIOBJ old_region;

    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    if ((char)color_flag == 1) {
        SetTextColor(hdc, 0xffffff);
    }
    old_region = SelectObject(hdc, region);
    color_flag = (int)SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &rc, 0x25);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

// FUNCTION: LEGOLAND 0x00491e40
int FUN_00491e40(char *text, int font, RECT rc, int color_flag) {
    RECT measured;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;

    measured.right = rc.left;
    measured.bottom = rc.top;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    if ((char)color_flag == 1) {
        SetTextColor(hdc, 0xffffff);
    }
    old_region = SelectObject(hdc, region);
    color_flag = (int)SelectFont(hdc, font);
    DrawTextA(hdc, text, strlen(text), &measured, 0x425);
    DrawTextA(hdc, text, strlen(text), &rc, 0x25);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
    return (rc.left + rc.right) / 2 + (measured.left - measured.right) / 2;
}

// FUNCTION: LEGOLAND 0x00491f90
unsigned char FUN_00491f90(struct RideState *state, unsigned char flags) {
    if ((flags & 0x2) != 0) {
        if (state->var_18 == 2) {
            if (DAT_007cad60.field_20 >= 2) {
                DAT_007cad60.field_20--;
                return 1;
            }
        } else {
            if (DAT_007cad60.field_20 <= 99) {
                DAT_007cad60.field_20++;
                return 1;
            }
        }
        return 1;
    } else {
        if ((flags & 0x1) != 0) {
            return 2;
        }
        if ((flags & 0x4) != 0) {
            return 2;
        }
        return 1;
    }
}

// FUNCTION: LEGOLAND 0x004920a0
char FUN_004920a0(unsigned int param1, unsigned char flags) {
    if ((flags & 0x2) != 0 && (void *)ScanForProfiles != 0) {
        RemoveIconGroup(0x15);
        CloseFontEndCheckBox();
        DAT_0080ff80.unk4 = 0xffffffff;
        DAT_007986e8 = 0;
        DAT_0080ffe3 = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004920e0
struct ProfileObj *FUN_004920e0(void) {
    struct ProfileObj *v = (struct ProfileObj *)malloc(0x38);
    v->next = 0;
    v->var_4 = 0;
    v->var_c = 0;
    v->var_28 = 0;
    v->var_2c = 0;
    v->var_30 = 0;
    v->var_34 = 0;
    v->var_8 = 0;
    v->var_1c = 0;
    v->var_24 = 0;
    v->var_20 = 0;
    return v;
}

// FUNCTION: LEGOLAND 0x00492110
void FUN_00492110(void) {
    struct ProfileObj *obj = FUN_004920e0();
    obj->next = (struct ProfileObj *)DAT_007988cc;
    DAT_007988cc = obj;
}

// FUNCTION: LEGOLAND 0x00492130
int FUN_00492130(void *hwnd) { STUB(); }
