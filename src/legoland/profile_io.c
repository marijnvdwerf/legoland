#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "legoland.h"

#include <windows.h>
#include <ddraw.h>
#include <dsound.h>

#include "clipping.h"
#include "draw.h"
#include "globals.h"
#include "icon.h"
#include "input.h"
#include "profile.h"
#include "profile_io.h"
#include "savegame_ui.h"
#include "sound_sfx.h"
#include "text.h"
#include "timer.h"
#include "title.h"

#pragma intrinsic(memset, memcpy, strcpy, strlen)

struct RideState {
    unsigned char pad_0[0x18];
    unsigned char var_18;
};

// FUNCTION: LEGOLAND 0x004912e0
LEGO_EXPORT void ResetTempProfile(void) {
    TempProfile.speech_volume = 0x4b;
    TempProfile.music_volume = 0x4b;
    TempProfile.fx_volume = 0x4b;
    TempProfile.name[0] = 0;
    TempProfile.age = 5;
    TempProfile.name_len = 0;
    memset(&TempProfile.field_34, 0, 15);
    memset(TempProfile.field_43, 0, 200);
    *(int *)&TempProfile.field_10b = 0;
    TempProfile.field_10b = 1;
    Empty_0048a780(TempProfile.field_43);
}

// FUNCTION: LEGOLAND 0x00491360
LEGO_EXPORT int Goto_ProfileDir(void) {
    struct _finddata_t find_data;
    long handle;
    int found;
    int next;

    found = 0;
    // STRING: LEGOLAND 0x004b9174
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
        // STRING: LEGOLAND 0x004b9174
        return _mkdir("profiles") == 0;
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
    result = ReturnFrom_ProfileDir() ? 0 : -1;
    return result;
}

// FUNCTION: LEGOLAND 0x00491540
unsigned int TempProfileHasName(void) { return TempProfile.name[0] != 0; }

// FUNCTION: LEGOLAND 0x00491550
LEGO_EXPORT char UpDateCurrentSaveSlotInfo(void) {
    char result;
    struct ProfileData temp;
    char path[120];
    void *stream;

    memset(&temp, 0, sizeof(struct ProfileData));
    if (LoadDateIntoTempProfile(CurrentProfile.profile_slot, CurrentProfile.save_slot & 0xff) == 0) {
        return -1;
    }
    strcpy(temp.name, TempProfile.name);
    temp.speech_volume = CurrentProfile.speech_volume;
    temp.music_volume = CurrentProfile.music_volume;
    temp.fx_volume = CurrentProfile.fx_volume;
    temp.field_24 = CurrentProfile.field_45;
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\%dsave%d.sh", CurrentProfile.profile_slot, CurrentProfile.save_slot & 0xff);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
    } else {
        fwrite(&temp, sizeof(struct ProfileData), 1, stream);
        fclose(stream);
    }
    result = ReturnFrom_ProfileDir() ? 1 : -1;
    return result;
}

// FUNCTION: LEGOLAND 0x00491680
LEGO_EXPORT char UpDateCurrentProfile(void) {
    char result;
    struct ProfileData temp;
    char path[120];
    void *stream;

    memset(&temp, 0, sizeof(struct ProfileData));
    strcpy(temp.name, (char *)&CurrentProfile);
    temp.age = CurrentProfile.age;
    temp.speech_volume = CurrentProfile.speech_volume;
    temp.music_volume = CurrentProfile.music_volume;
    temp.fx_volume = CurrentProfile.fx_volume;
    temp.field_34 = *(int *)&CurrentProfile.flags[4];
    temp.field_38 = *(int *)&CurrentProfile.flags[8];
    temp.field_3c = *(int *)&CurrentProfile.flags[12];
    temp.field_40 = *(short *)&CurrentProfile.flags[16];
    temp.field_42 = CurrentProfile.flags[18];
    memcpy(temp.field_43, CurrentProfile.field_46, 200);
    *(int *)&temp.field_10b = *(int *)&CurrentProfile.flags[0];
    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", CurrentProfile.profile_slot);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
    } else {
        fwrite(&temp, sizeof(struct ProfileData), 1, stream);
        fclose(stream);
    }
    result = ReturnFrom_ProfileDir() ? 1 : -1;
    return result;
}

// FUNCTION: LEGOLAND 0x004917c0
int WriteProfileSlotToFile(int slot) {
    struct ProfileData data;
    char path[120];
    struct ProfileNode *node;
    void *stream;

    node = FindProfileNodeBySlot((unsigned char)slot);
    if (node == NULL) {
        return 0;
    }
    memset(&data, 0, sizeof(struct ProfileData));
    strcpy(data.name, node->data.name);
    data.age = node->data.age;
    data.speech_volume = node->data.speech_volume;
    data.music_volume = node->data.music_volume;
    data.fx_volume = node->data.fx_volume;
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
    char result;
    char path[120];
    void *stream;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", CurrentProfile.profile_slot);
    stream = fopen(path, "w+");
    if (stream == 0) {
        printf("\ncannot open output file");
    } else {
        fwrite(&TempProfile, sizeof(struct ProfileData), 1, stream);
        fclose(stream);
    }
    result = ReturnFrom_ProfileDir() ? 1 : -1;
    return result;
}

// FUNCTION: LEGOLAND 0x004919a0
struct ProfileNode *FindProfileNodeBySlot(unsigned char slot) {
    struct ProfileNode *current = (struct ProfileNode *)ProfileListHead;

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
        node->data.age = data->age;
        node->data.speech_volume = data->speech_volume;
        node->data.music_volume = data->music_volume;
        node->data.fx_volume = data->fx_volume;
        node->slot = slot;
        node->has_header = 1;
        memcpy(&node->data.field_34, &data->field_34, 15);
        memcpy(node->data.field_43, data->field_43, 200);
        *(int *)&node->data.field_10b = *(int *)&data->field_10b;
        node->next = (struct ProfileNode *)ProfileListHead;
        ProfileListHead = node;
        return;
    }
    node->slot = slot;
    node->next = (struct ProfileNode *)ProfileListHead;
    ProfileListHead = node;
}

// FUNCTION: LEGOLAND 0x00491ab0
LEGO_EXPORT int RemoveProfile(unsigned char index) {
    char path[120];
    int slot;
    int i;

    if (Goto_ProfileDir() == 0) {
        return -1;
    }
    sprintf(path, "profiles\\Profile%d.txt", index);
    remove(path);
    slot = 8;
    for (i = 0; i < 8; i++) {
        // STRING: LEGOLAND 0x004bf730
        sprintf(path, "profiles\\%dsave%d.sav", index, slot);
        remove(path);
        sprintf(path, "profiles\\%dsave%d.sh", index, slot);
        remove(path);
        slot--;
    }
    return (ReturnFrom_ProfileDir() != 0) - 1;
}

// FUNCTION: LEGOLAND 0x00491b50
LEGO_EXPORT void DeleteProfileList(void) {
    struct ProfileNode *current = (struct ProfileNode *)ProfileListHead;

    while (current != NULL) {
        struct ProfileNode *next = current->next;
        free(current);
        current = next;
    }

    ProfileListHead = NULL;
}

// FUNCTION: LEGOLAND 0x00491bd0
LEGO_EXPORT void EnterNewProfile(struct IconNode *sprite) {
    char cursor_str[2];
    unsigned char count;
    char input;
    RECT rc;
    int center_x;
    char *blink;

    count = TempProfile.name_len;
    // STRING: LEGOLAND 0x004bf2e4
    *(short *)cursor_str = *(short *)"|";
    input = GetInputChar();
    if (input != '\0') {
        if (input == -1 && count != 0) {
            count--;
            TempProfile.name[count] = 0;
        }
        if (count < 0x1f && ProfileNameTextWidth < 0x7b && input > '\0') {
            if (input == ' ') {
                if (count != 0) {
                    TempProfile.name[count] = ' ';
                    count++;
                    TempProfile.name[count] = 0;
                }
            } else {
                TempProfile.name[count] = input;
                count++;
                TempProfile.name[count] = 0;
            }
        }
    }
    rc.top = sprite->y + 7;
    rc.left = sprite->x + 0x14;
    rc.bottom = rc.top + 0x13;
    rc.right = rc.left + 0xc0;
    if (count != 0) {
        center_x = DrawCenteredTextAndGetRightEdge(TempProfile.name, 2, rc, 1);
    } else {
        center_x = (rc.right + rc.left) >> 1;
    }
    ProfileNameTextWidth = (center_x - ((rc.right + rc.left) >> 1)) * 2;
    rc.top = sprite->y + 5;
    blink = "|";
    if (GetBlink() == 0) {
        // STRING: LEGOLAND 0x004b8ad4
        blink = " ";
    }
    strcpy(cursor_str, blink);
    rc.left = center_x;
    rc.right = center_x + 100;
    DrawClippedText(cursor_str, 2, rc, 1);
    TempProfile.name_len = count;
}

// FUNCTION: LEGOLAND 0x00491d60
LEGO_EXPORT void NewPrintCent(char *text, int font, RECT rc, INT_PTR color_flag) {
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
    color_flag = (INT_PTR)SelectFont(hdc, font); /* the old font */
    DrawTextA(hdc, text, strlen(text), &rc, 0x25);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
}

/* Measures and draws centered text in the supplied rectangle with sprite clipping and optional white text, then
 * returns the horizontal coordinate of its measured right edge. */
// FUNCTION: LEGOLAND 0x00491e40
int DrawCenteredTextAndGetRightEdge(char *text, int font, RECT rc, INT_PTR color_flag) {
    RECT measured;
    HRGN region;
    HDC hdc;
    HGDIOBJ old_region;

    measured = rc;
    region = CreateRectRgn(SPRITE_ClipRect.left, SPRITE_ClipRect.top, SPRITE_ClipRect.right, SPRITE_ClipRect.bottom);
    PushRenderingStatusAndUnlockVideoSurface();
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->GetDC((LPDIRECTDRAWSURFACE)renderEngine, &hdc);
    SetBkMode(hdc, 1);
    if ((char)color_flag == 1) {
        SetTextColor(hdc, 0xffffff);
    }
    old_region = SelectObject(hdc, region);
    color_flag = (INT_PTR)SelectFont(hdc, font); /* the old font */
    DrawTextA(hdc, text, strlen(text), &measured, 0x425);
    DrawTextA(hdc, text, strlen(text), &rc, 0x25);
    SelectObject(hdc, (HGDIOBJ)color_flag);
    SelectObject(hdc, old_region);
    ((LPDIRECTDRAWSURFACE)renderEngine)->lpVtbl->ReleaseDC((LPDIRECTDRAWSURFACE)renderEngine, hdc);
    PopRenderingStatus();
    DeleteObject(region);
    return (rc.left + rc.right) / 2 + (measured.right - measured.left) / 2;
}

/* Age arrow handler for the profile being created: a click (flags & 2) decrements TempProfile.age (down arrow, icon
 * field 0x18 == 2, minimum 1) or increments it (maximum 100). Returns 2 for press/hold flags 1 and 4. No
 * registration of it was found. */
// FUNCTION: LEGOLAND 0x00491f90
unsigned char ProfileAgeArrowClicked(struct RideState *state, unsigned char flags) {
    if ((flags & 0x2) != 0) {
        if (state->var_18 == 2) {
            if (TempProfile.age >= 2) {
                TempProfile.age--;
                return 1;
            }
        } else {
            if (TempProfile.age <= 99) {
                TempProfile.age++;
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

/* Closes the profile popup and confirmation box, invalidates the front-end screen, and clears the selected profile
 * slot on activation. */
// FUNCTION: LEGOLAND 0x004920a0
char CancelProfileSelectionClicked(unsigned int param1, unsigned char flags) {
    if ((flags & 0x2) != 0 && (void *)ScanForProfiles != 0) {
        RemoveIconGroup(0x15);
        CloseFontEndCheckBox();
        FrontEndScreenMode.unk4 = 0xffffffff;
        NewProfilePopUpShown = 0;
        CurrentProfile.profile_slot = 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004920e0
struct Sample *AllocSample(void) {
    struct Sample *v = (struct Sample *)malloc(sizeof(struct Sample));
    v->next = 0;
    v->refcount = 0;
    v->source_type = 0;
    v->active = 0;
    v->buffer = 0;
    v->field_30 = 0;
    v->field_34 = 0;
    v->fade = 0;
    v->flags = 0;
    v->callback = 0;
    v->timeout = 0;
    return v;
}

// FUNCTION: LEGOLAND 0x00492110
void *NewSample(void) {
    struct Sample *obj = AllocSample();
    obj->next = (struct Sample *)SampleListHead;
    SampleListHead = obj;
    return obj;
}

// FUNCTION: LEGOLAND 0x00492130
int InitDirectSound(void *hwnd) {
    int ok;

    ok = 0;
    if (DirectSoundCreate(NULL, (LPDIRECTSOUND *)&DSound, NULL) == 0) {
        if (((LPDIRECTSOUND)DSound)->lpVtbl->SetCooperativeLevel((LPDIRECTSOUND)DSound, (HWND)hwnd, 1) ==
            0) {
            memset(DSoundCaps, 0, sizeof(DSoundCaps));
            DSoundCaps[0] = sizeof(DSoundCaps);
            if (((LPDIRECTSOUND)DSound)->lpVtbl->GetCaps((LPDIRECTSOUND)DSound, (LPDSCAPS)DSoundCaps) ==
                0) {
                ok = 1;
            }
        }
        if (!ok) {
            ((LPDIRECTSOUND)DSound)->lpVtbl->Release((LPDIRECTSOUND)DSound);
        }
    }
    if (ok) {
        SoundAvailable = 1;
        return 1;
    }
    DSound = NULL;
    SoundAvailable = 0;
    return 0;
}
