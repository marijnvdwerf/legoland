#include "render3d.h"
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloke.h"
#include "globals.h"
#include "legoland.h"
#include "llidb.h"
#include "man3d.h"
#include "math.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render.h"
#include "resource.h"
#include "tilemap.h"

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x00441800
LEGO_EXPORT void Render_SetViewport(struct tagRECT *viewport) {
    ViewportLeft = viewport->left;
    ViewportRight = viewport->right;
    ViewportTop = viewport->top;
    ViewportBottom = viewport->bottom;
}

// FUNCTION: LEGOLAND 0x00441830
struct RideNode *FindRiderAt(void *param_1, short *param_2) {
    struct RideNode *node = (struct RideNode *)RiderSearchCursor;
    while (node != NULL) {
        if (memcmp(&node->tile.id, param_2, sizeof(node->tile.id)) == 0) {
            RiderSearchCursor = node;
            return node;
        }
        node = node->next;
    }
    RiderSearchCursor = NULL;
    return NULL;
}

// FUNCTION: LEGOLAND 0x00441870
struct RideNode *FindFirstRiderAt(struct Ride *param_1, short *param_2) {
    RiderSearchCursor = param_1->riders;
    return FindRiderAt(param_1, param_2);
}

// FUNCTION: LEGOLAND 0x00441890
struct RideNode *FindNextRiderAt(void *param_1, short *param_2) {
    struct RideNode *node = (struct RideNode *)RiderSearchCursor;
    if (node != NULL) {
        RiderSearchCursor = node->next;
        return FindRiderAt(param_1, param_2);
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x004418c0
struct RideNode *FindRiderAtIndex(int param_1, struct Ride *param_2, short *param_3) {
    struct RideNode *node = FindFirstRiderAt(param_2, param_3);
    int i = 0;
    if (node != NULL) {
        for (; i < param_2->seats; i++) {
            if (i == param_1) {
                return node;
            }
            node = FindNextRiderAt(param_2, param_3);
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00441910
void RidePointToScreen(struct Position *param_1, float *param_2, int *param_3) {
    float v[3];
    v[0] = param_2[0];
    v[1] = param_2[1];
    v[0] -= RidePointOriginX;
    v[1] -= RidePointOriginY;
    param_3[0] = (int)v[0] / 2;
    param_3[1] = (int)v[1] / 2;
    param_3[0] += param_1->x;
    param_3[1] += param_1->y;
}

// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00441980
__declspec(naked) void Put3DBlokeOnRide(struct Position *param_1, int param_2, int param_3, struct Person *param_4, int param_5, int param_6) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x10
        mov eax, dword ptr [ebp + 0x10]
        test eax, eax
        jge L44198f
        xor eax, eax
L44198f:
        mov ecx, dword ptr [ebp + 8]
        mov edx, dword ptr [ecx]
        cmp eax, edx
        jl L44199b
        lea eax, [edx - 1]
L44199b:
        mov edx, dword ptr [ecx + 0x24]
        push ebx
        push esi
        mov esi, dword ptr [ebp + 0xc]
        lea eax, [eax + eax*2]
        push edi
        mov edi, dword ptr [edx + esi*4]
        lea edx, [ebp - 8]
        shl eax, 4
        add edi, eax
        push edx
        push edi
        push ecx
        call RidePointToScreen
        mov eax, dword ptr [ebp + 0x18]
        mov ecx, dword ptr [ebp - 8]
        mov edx, dword ptr [ebp + 0x1c]
        add ecx, eax
        mov eax, dword ptr [ebp - 4]
        add esp, 0xc
        mov dword ptr [ebp - 0x10], ecx
        mov ecx, dword ptr [ebp + 0x14]
        add eax, edx
        mov dword ptr [ebp + 0x1c], 0x47800000
        mov dword ptr [ebp - 0xc], eax
        mov eax, offset RideMatrixRowOrder
        lea edx, [ecx + 0x58]
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp + 0xc], edx
L4419eb:
        mov esi, dword ptr [eax]
        xor ecx, ecx
        lea ebx, [esi + esi*2 + 3]
L4419f3:
        mov eax, dword ptr [ecx + RideMatrixColOrder]
        add eax, ebx
        fld dword ptr [edi + eax*4]
        fstp dword ptr [ebp + 0x10]
        fld dword ptr [ebp + 0x10]
        fmul dword ptr [ebp + 0x1c]
        fistp dword ptr [ebp + 0x10]
        mov eax, dword ptr [ecx + RideMatrixColSign]
        add ecx, 4
        imul eax, dword ptr [esi*4 + RideMatrixRowSign]
        imul eax, dword ptr [ebp + 0x10]
        mov dword ptr [edx], eax
        add edx, 0xc
        cmp ecx, 0xc
        jl L4419f3
        mov eax, dword ptr [ebp + 8]
        mov edx, dword ptr [ebp + 0xc]
        add eax, 4
        add edx, 4
        cmp eax, offset RideMatrixColOrder
        mov dword ptr [ebp + 8], eax
        mov dword ptr [ebp + 0xc], edx
        jl L4419eb
        mov ecx, dword ptr [ebp - 0xc]
        mov edx, dword ptr [ebp - 0x10]
        mov eax, dword ptr [ebp + 0x14]
        push ecx
        push edx
        push eax
        call SetPersonPosition
        add esp, 0xc
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x00441a60
LEGO_EXPORT void Put3DBlokesOnRide(struct Ride *param_1, unsigned char *param_2, int param_3, struct Position *param_4) {
    struct Point coords;
    int i;

    coords = GetScreenCoordsForObject((TileId *)param_2, param_1);
    i = 0;
    if (param_4->count > 0) {
        do {
            struct RideNode *node = (struct RideNode *)FindRiderAtIndex(i, param_1, (short *)param_2);
            if (node != NULL && (node->rider->flags & 0x80) != 0) {
                Put3DBlokeOnRide(param_4, i, param_3, node->person, coords.x, coords.y);
            }
            i = i + 1;
        } while (i < param_4->count);
    }
}

// FUNCTION: LEGOLAND 0x00441ad0
LEGO_EXPORT void Put3DBlokesOnRide2(struct Ride *ride, void *obj) {
    struct RideNode *node = (struct RideNode *)FindFirstRiderAt(ride, (short *)obj);
    if (node != NULL) {
        while ((node->rider->flags & 0x80) != 0) {
            node = (struct RideNode *)FindNextRiderAt(ride, (short *)obj);
            if (node == NULL) {
                return;
            }
        }
        while (node != NULL) {
            if (node->person != 0) {
                UpdatePersonFromBloke((struct Person *)node->person, (struct Bloke *)node->rider);
            }
            node = (struct RideNode *)FindNextRiderAt(ride, (short *)obj);
            if (node == NULL) {
                return;
            }
            while ((node->rider->flags & 0x80) != 0) {
                node = (struct RideNode *)FindNextRiderAt(ride, (short *)obj);
                if (node == NULL) {
                    return;
                }
            }
        }
    }
}

struct RenderNode {
    struct RenderNode *next;
    unsigned char pad_4[4];
    void *data_ptr;
    unsigned short id;
};

struct RenderBloke {
    unsigned short id;
};

struct RenderData {
    unsigned char pad_0[0x62];
    unsigned char flags;
};

struct RenderContext {
    unsigned char pad_0[0xcc];
    struct RenderNode *bloke_list;
};

// FUNCTION: LEGOLAND 0x00441b60
LEGO_EXPORT void RenderBlokesNotInSeats(struct Ride *a1, TileId *a2) {
    struct Ride *context = a1;
    TileId *bloke = a2;
    struct RideNode *node = context->riders;

    while (node != NULL) {
        if (bloke->id == node->tile.id) {
            struct Bloke *data = node->rider;
            if ((*(unsigned char *)&data->flags & 0x80) == 0) {
                IP_RenderBlokeIn3DNow(data);
            }
        }
        node = node->next;
    }
}

#pragma intrinsic(memset)

// FUNCTION: LEGOLAND 0x00441ba0
LEGO_EXPORT struct RinData *LoadRin(const char *path, const char *dir) {
    struct RinData *rin;
    struct ResFile *file;
    char name[264];
    char sprite_name[264];
    char c;
    int i;
    int j;

    rin = (struct RinData *)malloc(sizeof(struct RinData));
    file = RES_OpenFile(path);
    if (file != NULL) {
        memset(rin, 0, sizeof(struct RinData));
        RES_ReadFile(file, &rin->sprite_count, 4);
        rin->sprites = (struct Sprite **)malloc(rin->sprite_count << 2);
        i = 0;
        if (rin->sprite_count > 0) {
            do {
                char *p = name;
                do {
                    RES_ReadFile(file, &c, 1);
                    *p = c;
                    p++;
                } while (c != '\0');
                // STRING: LEGOLAND 0x004b7cf4
                sprintf(sprite_name, "%s\\%s.lls", dir, name);
                rin->sprites[i] = LoadSprite(sprite_name, 1);
                i++;
            } while (i < rin->sprite_count);
        }
        RES_ReadFile(file, &rin->data_count, 4);
        rin->datas = (void **)malloc(rin->data_count << 2);
        if (rin->datas != NULL && (j = 0, rin->data_count > 0)) {
            do {
                rin->datas[j] = malloc(rin->sprite_count << 2);
                RES_ReadFile(file, rin->datas[j], rin->sprite_count << 2);
                j++;
            } while (j < rin->data_count);
        }
        RES_CloseFile(file);
    }
    return rin;
}

#pragma function(memset)

// FUNCTION: LEGOLAND 0x00441cf0
LEGO_EXPORT void UnLoadRin(struct RinData *rin) {
    int i;
    int count;

    count = rin->sprite_count;
    for (i = 0; i < count; i++) {
        KillSprite(rin->sprites[i]);
        count = rin->sprite_count;
    }
    free(rin->sprites);

    count = rin->data_count;
    for (i = 0; i < count; i++) {
        free(rin->datas[i]);
        count = rin->data_count;
    }
    free(rin->datas);

    free(rin);
}

// FUNCTION: LEGOLAND 0x00441d60
LEGO_EXPORT void RenderUsingRin(struct RinData *param_1, int param_2, struct Ride *param_3, unsigned char *param_4) {
    struct Point coords;
    int idx;
    void *base;
    int i;
    struct Point offset;

    int y;
    int remap;
    coords = GetScreenCoordsForObject((TileId *)param_4, param_3);
    idx = param_2;
    if (param_2 >= param_1->data_count) {
        idx = param_2 % param_1->data_count;
    }
    base = param_1->datas[idx];
    for (i = param_1->sprite_count - 1; i >= 0; i--) {
        int sprite_id = ((int *)base)[i];
        struct RideNode *node;
        int *frame;
        if (param_1->data_table == NULL) {
            node = (struct RideNode *)FindRiderAtIndex(sprite_id, param_3, (short *)param_4);
        } else {
            remap = param_1->remap_table[sprite_id];
            node = (struct RideNode *)FindRiderAtIndex(param_1->data_table[remap], param_3, (short *)param_4);
        }
        if (node != NULL && (node->rider->flags & 0x80) != 0) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->rider);
        }
        frame = (int *)param_1->sprites[sprite_id];
        if (frame != NULL) {
            LLSSetFrame(GetLLSForSprite((struct Sprite *)frame), param_2);
        }
        offset.x = param_1->x;
        y = param_1->y;
        offset.y = y;
        AdjustOffsetForViewMode(&offset);
        frame = (int *)param_1->sprites[sprite_id];
        if (frame != NULL) {
            PrintSprite((struct Sprite *)frame, coords.x + offset.x, coords.y + offset.y, 0, 0);
        }
    }
}

// FUNCTION: LEGOLAND 0x00441e80
LEGO_EXPORT struct LLS *GetLLSForSprite(struct Sprite *sprite) {
    if (sprite != NULL) {
        return *sprite->lls;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00441ea0
LEGO_EXPORT struct LLS *GetLLSForLayer(struct Sprite *sprite, unsigned int index) {
    struct Sprite *layer = sprite->group->subs[index];
    if (layer == NULL) {
        return NULL;
    }
    return *layer->lls;
}

// FUNCTION: LEGOLAND 0x00441ec0
LEGO_EXPORT struct Sprite *GetSpriteForLayer(struct Sprite *sprite, unsigned int index) {
    return sprite->group->subs[index];
}

// FUNCTION: LEGOLAND 0x00441ee0
LEGO_EXPORT struct Point GetRenderOffsetForLayer(struct Sprite *sprite, int index) {
    struct SpriteGroup *group = sprite->group;
    struct Point r;
    r.x = group->xoffs[index];
    r.y = group->yoffs[index];
    return r;
}

// FUNCTION: LEGOLAND 0x00441f00
LEGO_EXPORT void StopLayerPlaying(struct Sprite *sprite, unsigned int index) {
    LLSStop((struct LLS *)GetLLSForLayer(sprite, index));
}

#pragma intrinsic(memset)

// FUNCTION: LEGOLAND 0x00441f20
LEGO_EXPORT unsigned short *LoadPalette(const char *path) {
    unsigned short *palette;
    unsigned short *out;
    struct ResFile *file;
    int i;
    unsigned char g;
    unsigned char b;
    unsigned char header[8];

    palette = (unsigned short *)malloc(0x200);
    if (palette != NULL) {
        memset(palette, 0, 0x200);
        file = RES_OpenFile(path);
        if (file != NULL) {
            RES_ReadFile(file, header, 8);
            out = palette;
            for (i = 0x100; i != 0; i--) {
                RES_ReadFile(file, (void *)&path, 1); /* the red byte goes into the dead parameter */
                RES_ReadFile(file, &g, 1);
                RES_ReadFile(file, &b, 1);
                if (DisplayPixelFormat == 2) {
                    *out = (unsigned short)(((((UINT_PTR)path & 0xf8) << 5 | (g & 0xfc)) << 3) | (b >> 3));
                } else {
                    *out = (unsigned short)((((((unsigned char)(UINT_PTR)path & 0xf8) << 5) | (g & 0xf8)) << 2) | (b >> 3));
                }
                out++;
            }
            RES_CloseFile(file);
        }
    }
    return palette;
}

#pragma function(memset)

struct CellEntry {
    short field_0;
    unsigned char x;
    unsigned char y;
    unsigned char field_4;
    unsigned char field_5;
};

/* Remaps qualifying faces from the source texture-cell rectangle into the destination cell, normalizing UVs by the
 * destination texture size. */
// FUNCTION: LEGOLAND 0x00442040
void RemapTexCoordsToCell(struct LocFile *param_1, int param_2, int param_3, struct PersonFace *faces, int count) {
    struct CellEntry *entry1 = (struct CellEntry *)((char *)param_1->cells + param_2 * 6);
    struct CellEntry *entry2 = (struct CellEntry *)((char *)param_1->cells + param_3 * 6);
    int idx1 = entry1->field_0 + param_1->texture_base;
    int idx2 = entry2->field_0 + param_1->texture_base;
    int lo_x = entry1->x;
    int hi_x = entry1->field_4 + entry1->x + 1;
    int lo_y = entry1->y;
    int hi_y = entry1->field_5 + entry1->y + 1;
    struct PersonFace *face;
    int i;

    if (count > 0) {
        i = count;
        face = faces;
        do {
            if ((face->flags & 0x2000) == 0 && face->cell == idx1) {
                float u0 = face->uv[0];
                float v0 = face->uv[1];
                float u1 = face->uv[2];
                float v1 = face->uv[3];
                float u2 = face->uv[4];
                float v2 = face->uv[5];
                int w, h;
                float sv2;
                if (u0 < FLOAT_004ab390) u0 = 0.0f;
                if (u0 > 1.0) u0 = 1.0f;
                if (v0 < FLOAT_004ab390) v0 = 0.0f;
                if (v0 > 1.0) v0 = 1.0f;
                if (u1 < FLOAT_004ab390) u1 = 0.0f;
                if (u1 > 1.0) u1 = 1.0f;
                if (v1 < FLOAT_004ab390) v1 = 0.0f;
                if (v1 > 1.0) v1 = 1.0f;
                if (u2 < FLOAT_004ab390) u2 = 0.0f;
                if (u2 > 1.0) u2 = 1.0f;
                if (v2 < FLOAT_004ab390) v2 = FLOAT_004ab390;
                if (v2 > 1.0) v2 = 1.0f;
                w = TextureSizes[idx1 * 2];
                h = TextureSizes[idx1 * 2 + 1];
                u0 = w * u0;
                u1 = w * u1;
                u2 = w * u2;
                v0 = h * v0;
                v1 = h * v1;
                sv2 = h * v2;
                if (u0 >= lo_x && u0 <= hi_x && u1 >= lo_x && u1 <= hi_x && u2 >= lo_x && u2 <= hi_x &&
                    v0 >= lo_y && v0 <= hi_y && v1 >= lo_y && v1 <= hi_y && sv2 >= lo_y && sv2 <= hi_y) {
                    /* texture size of the destination cell */
                    w = TextureSizes[idx2 * 2];
                    h = TextureSizes[idx2 * 2 + 1];
                    /* map from the source cell's rectangle to the destination cell's, normalized */
                    u0 = ((u0 - entry1->x) / entry1->field_4 * entry2->field_4 + entry2->x) / w;
                    v0 = ((v0 - entry1->y) / entry1->field_5 * entry2->field_5 + entry2->y) / h;
                    u1 = ((u1 - entry1->x) / entry1->field_4 * entry2->field_4 + entry2->x) / w;
                    v1 = ((v1 - entry1->y) / entry1->field_5 * entry2->field_5 + entry2->y) / h;
                    u2 = ((u2 - entry1->x) / entry1->field_4 * entry2->field_4 + entry2->x) / w;
                    sv2 = ((sv2 - entry1->y) / entry1->field_5 * entry2->field_5 + entry2->y) / h;
                    face->cell = entry2->field_0 + param_1->texture_base;
                    face->uv[0] = u0;
                    face->uv[1] = v0;
                    face->uv[2] = u1;
                    face->uv[3] = v1;
                    face->uv[4] = u2;
                    face->uv[5] = sv2;
                }
            }
            face++;
            i--;
        } while (i != 0);
    }
}

struct ColourRef {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
};

/* Replaces the key colour param_1 on colour-ramp faces (flag 0x2000) with param_4 in the first half of the face
 * list and param_2 in the second half, rebuilding each face's ramp. param_3 is unused. */
// FUNCTION: LEGOLAND 0x004424e0
void RecolourPersonFaces(struct ColourRef *param_1, struct ColourRef *param_2, struct ColourRef *param_3, struct ColourRef *param_4, struct PersonFace *param_5, int param_6) {
    int i = 0;
    int count;
    struct PersonFace *entry;
    int rgb;

    if (param_6 > 0) {
        entry = param_5;
        count = param_6;
        do {
            if ((entry->flags & 0x2000) != 0 && entry->b2 == param_1->b2 &&
                entry->b1 == param_1->b1 && entry->b0 == param_1->b0) {
                if (i >= count >> 1) {
                    entry->b2 = param_2->b2;
                    entry->b1 = param_2->b1;
                    entry->b0 = param_2->b0;
#ifdef LEGOLAND_PORT
                    /* [port] the colour's three bytes: the original reads four, past the 3-byte struct (the
                     * fourth goes unused); clang may make such a read anything, and the gardeners came out
                     * magenta on WebAssembly */
                    rgb = param_2->b0 | param_2->b1 << 8 | param_2->b2 << 16;
#else
                    rgb = *(int *)param_2;
#endif
                } else {
                    entry->b2 = param_4->b2;
                    entry->b1 = param_4->b1;
                    entry->b0 = param_4->b0;
#ifdef LEGOLAND_PORT
                    /* [port] the colour's three bytes: the original reads four, past the 3-byte struct (the
                     * fourth goes unused); clang may make such a read anything, and the gardeners came out
                     * magenta on WebAssembly */
                    rgb = param_4->b0 | param_4->b1 << 8 | param_4->b2 << 16;
#else
                    rgb = *(int *)param_4;
#endif
                }
                entry->ramp = GetOrCreateColorRamp(0x40, &rgb);
                count = param_6;
            }
            entry++;
            i = i + 1;
        } while (i < count);
    }
}

/* Returns a malloc'd copy of the mesh faces for this person. Visitors (character 1) get random alternatives for the
 * two swappable textures from the man or woman list (flag is person->random; the picks are kept in field_80 and
 * field_7c). The grey key colour 0x565656 becomes the person's two BlokeColours: random for visitors, fixed for
 * characters 2 and 3. */
// FUNCTION: LEGOLAND 0x00442580
void *CreatePersonFaceList(struct Person *person, struct LocFile *context, struct PersonFace *src, unsigned int count, unsigned int flag) {
    int *arrA;
    int idxI;
    int valC;
    int *arrD;
    int idxJ;
    int valE;
    struct ColourRef z0;
    struct ColourRef z1;
    struct ColourRef z2;
    struct ColourRef z3;
    int a;
    int b;
    void *mem;
    int size;
    int modI;
    int modJ;

    if (person->character == 1) {
        if (flag == 0) {
            arrA = AltManTexturesA;
            arrD = AltManTexturesB;
            valC = AltManBaseTextureA;
            modI = AltManTextureCountA;
            modJ = AltManTextureCountB[0];
            valE = AltManBaseTextureB;
            z0.b0 = z0.b1 = z0.b2 = 0x56;
            z3.b0 = z3.b1 = z3.b2 = 0;
        } else {
            arrA = AltWomanTexturesA;
            arrD = AltWomanTexturesB;
            modI = AltWomanTextureCountA;
            modJ = AltWomanTextureCountB;
            valC = AltWomanBaseTextureA;
            valE = AltWomanBaseTextureB;
            z0.b0 = z0.b1 = z0.b2 = 0x56;
            z3.b0 = z3.b1 = z3.b2 = 1;
        }
        if (person->field_80 == -1) {
            idxI = rand() % modI;
            person->field_80 = idxI;
        } else {
            idxI = person->field_80;
        }
        if (person->field_7c == -1) {
            idxJ = rand() % modJ;
            person->field_7c = idxJ;
        } else {
            idxJ = person->field_7c;
        }
        b = person->field_8c;
        if (b == -1) {
            do {
                b = rand() & 7;
            } while (b == 7);
            person->field_8c = b;
        }
        a = person->field_90;
        if (a == -1) {
            a = rand() & 7;
            person->field_90 = a;
        }
    } else {
        for (;;) {
            if (person->character == 3) {
                a = 4;
                b = 1;
            } else if (person->character == 2) {
                a = 0;
                b = 3;
            } else {
                break;
            }
            z0.b0 = z0.b1 = z0.b2 = 0x56;
            z3.b0 = z3.b1 = z3.b2 = 0;
            break;
        }
    }
    z1.b2 = BlokeColours[a * 3];
    z1.b1 = BlokeColours[a * 3 + 1];
    z1.b0 = BlokeColours[a * 3 + 2];
    z2.b2 = BlokeColours[b * 3];
    z2.b1 = BlokeColours[b * 3 + 1];
    z2.b0 = BlokeColours[b * 3 + 2];
    size = count * sizeof(struct PersonFace);
    mem = malloc(size);
    if (mem != 0) {
        memcpy(mem, src, size);
        if ((int)person->character < 2) {
            RemapTexCoordsToCell(context, valC, arrA[idxI], (struct PersonFace *)mem, count);
            RemapTexCoordsToCell(context, valE, arrD[idxJ], (struct PersonFace *)mem, count);
        }
        RecolourPersonFaces(&z0, &z1, &z3, &z2, mem, count);
    }
    return mem;
}

// FUNCTION: LEGOLAND 0x004427e0
char *RES_ReadLine(struct ResFile *param_1, char *param_2, int param_3) {
    struct ResFile *file = param_1;
    char *c = (char *)&param_1;
    int count = 0;
    int read;
    char *dst = param_2;

    do {
        read = RES_ReadFile(file, &param_1, 1);
        if (read != 0) {
            if (*c == '\r') goto skip_lf;
            if (*c == '\n') goto terminate;
            *dst = *c;
            dst++;
            count++;
        }
        if (*c == '\r') goto skip_lf;
    } while (*c != '\n' && count < param_3 && read != 0);
    if (*c == '\r') {
    skip_lf:
        RES_ReadFile(file, &param_1, 1);
    }
terminate:
    *dst = '\0';
    if (read == 0 && count == 0) {
        return NULL;
    }
    return param_2;
}

// FUNCTION: LEGOLAND 0x00442860
int FindIndexInStringList(char *param_1, char *param_2) {
    int index = 0;

    while (_stricmp(param_1, param_2) != 0) {
        param_1 = param_1 + strlen(param_1) + 1;
        index = index + 1;
        if (strlen(param_1) == 0) {
            return -1;
        }
    }
    return index;
}

// FUNCTION: LEGOLAND 0x004428c0
unsigned char *GetNthStringInList(unsigned char *str, int count) {
    unsigned char *result = str;
    if (count > 0) {
        do {
            result = result + strlen((char *)result) + 1;
            count = count - 1;
        } while (count != 0);
    }
    return result;
}

/* A 3D data list (Load3DDataFile, e.g. visitor\altman.txt) is sections of "Name\0" + int count + strings
 * ending with an empty one. param_2 picks the section (0 faces, 1 chests), param_3 the string in it.
 * volatile: the original reloads param_3 from the stack at each use. values is only set when section 0 has
 * strings; the original keeps it in param_1's stack slot, and the game's files always have them. */
// FUNCTION: LEGOLAND 0x004428f0
unsigned char *Get3DDataListString(char *param_1, int param_2, volatile int param_3) {
    char *values;
    char *names;
    char *p;
    int flag;

    if (param_1 != NULL) {
        p = param_1 + strlen(param_1) + 1;
        flag = *(int *)p;
        p = p + 4;
        names = p;
        if (flag != 0) {
            do {
                p = p + strlen(p) + 1;
            } while (strlen(p) != 0);
            p++;
            values = p + strlen(p) + 1 + 4;
        }
    } else {
        names = (char *)param_3;
    }
    switch (param_2) {
    case 1:
        return GetNthStringInList((unsigned char *)values, param_3);
    }
    return GetNthStringInList((unsigned char *)names, param_3);
}

// GLOBAL: LEGOLAND 0x004b7d00
const char ModelLineFormat[] = "%s %i %i %i %i %i";

/* Reads an alternative texture list (altman.txt as set 0, altwoman.txt as set 1): two base texture names and the
 * alternatives that may replace them, resolved to indices through the model texture list (Path3DFormat).
 * CreatePersonFaceList picks from these tables. */
// FUNCTION: LEGOLAND 0x00442980
void LoadPersonAltTextureList(const char *param_1, const char *param_2, const char *param_3, int param_4, unsigned int param_5) {
    int idx = 0;
    /* As in the original (0x442a13), count1 and count2 are not set when Load3DDataFile fails, and count2 is not set
     * when count1 is 0: the game reads whatever is in their stack slots there. */
    unsigned int count1;
    int count2;
    char *list_mid;
    char *list1;
    char *name;
    char *list2;
    int *out_b;
    char *data;
    int *out_a;
    int *arr2;
    int *arr1;
    int v1, v2, v3, v4, v5;
    char word[128];
    char line[512];
    char path[256];
    struct ResFile *file;
    char *p;
    char *q;
    char *dst;
    char c;
    int r;

    data = (char *)Load3DDataFile(param_3, param_1);
    if (NULL != data) {
        name = data;
        p = data + strlen(data) + 1;
        count1 = *(int *)p;
        p += 4;
        list1 = p;
        if (0 != count1) {
            q = p;
            do {
                q = q + strlen(q) + 1;
            } while (strlen(q) != 0);
            q = q + 1;
            list_mid = q;
            q = q + strlen(q) + 1;
            count2 = *(int *)q;
            list2 = q + 4;
        }
    }
    sprintf(path, Path3DFormat, param_3, param_2);
    if (param_4 == 0) {
        AltManTextureCountA = count1;
        AltManTexturesA = (int *)malloc(count1 * 4);
        AltManTextureCountB[0] = count2;
        AltManTexturesB = (int *)malloc(count2 * 4);
        arr1 = AltManTexturesA;
        arr2 = AltManTexturesB;
        out_b = &AltManBaseTextureB;
        out_a = &AltManBaseTextureA;
    } else {
        AltWomanTextureCountA = count1;
        AltWomanTexturesA = (int *)malloc(count1 * 4);
        AltWomanTextureCountB = count2;
        AltWomanTexturesB = (int *)malloc(count2 * 4);
        arr1 = AltWomanTexturesA;
        arr2 = AltWomanTexturesB;
        out_b = &AltWomanBaseTextureB;
        out_a = &AltWomanBaseTextureA;
    }
    file = RES_OpenFile(path);
    if (file != NULL) {
        RES_ReadLine(file, line, 512);
        RES_ReadLine(file, line, 512);
        if (RES_ReadLine(file, line, 512) != NULL) {
            do {
                /* lowercase the model name up to the first '.' */
                dst = path;
                p = line;
                do {
                    c = (char)tolower(*p++);
                    if (c == '.') c = 0;
                    *dst++ = c;
                } while (c != 0);
                sscanf(p, ModelLineFormat, word, &v5, &v4, &v3, &v2, &v1);
                if (_strcmpi(name, path) == 0) {
                    *out_a = idx;
                } else if (_strcmpi(list_mid, path) == 0) {
                    *out_b = idx;
                }
                if (count1 != 0) {
                    r = FindIndexInStringList(list1, path);
                    if (r != -1) arr1[r] = idx;
                }
                if (count2 != 0) {
                    r = FindIndexInStringList(list2, path);
                    if (r != -1) arr2[r] = idx;
                }
                idx++;
            } while (RES_ReadLine(file, line, 512) != NULL);
        }
    }
    RES_CloseFile(file);
    free(data);
}

// FUNCTION: LEGOLAND 0x00442c70
unsigned int FreeRender3DBuffers(void) {
    if (AltManTexturesA) {
        free(AltManTexturesA);
    }
    if (AltManTexturesB) {
        free(AltManTexturesB);
    }
    if (AltWomanTexturesA) {
        free(AltWomanTexturesA);
    }
    if (AltWomanTexturesB) {
        free(AltWomanTexturesB);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x00442cc0
LEGO_EXPORT struct Point GetScreenCoordsForObject(TileId *tile, struct Ride *ride) {
    int bounds[4];
    struct Point ref;
    int dx;
    int dy;
    struct Point r;

    ref.x = tile->pos.x;
    ref.y = tile->pos.y;
    GetTileBounds(&ref, bounds);
    dx = ride->field_14;
    dy = ride->field_18;
    if (dx < 0) {
        dx = -(-dx >> 1);
    } else {
        dx = dx >> 1;
    }
    if (dy < 0) {
        dy = -(-dy >> 1);
    } else {
        dy = dy >> 1;
    }
    r.x = bounds[0] + dx;
    r.y = bounds[1] + dy;
    return r;
}

// FUNCTION: LEGOLAND 0x00442d30
LEGO_EXPORT void AdjustOffsetForViewMode(struct Point *param_1) {
    int temp0;
    int temp4;

    temp0 = param_1->x;
    if (temp0 < 0) {
        temp0 = -((-temp0) >> 1);
    } else {
        temp0 = temp0 >> 1;
    }
    param_1->x = temp0;

    temp4 = param_1->y;
    if (temp4 < 0) {
        temp4 = -((-temp4) >> 1);
    } else {
        temp4 = temp4 >> 1;
    }
    param_1->y = temp4;
}

// FUNCTION: LEGOLAND 0x00442d60
LEGO_EXPORT void AdjustBlokePosition(struct Point *pos) {
    pos->x -= 0x4b;
    pos->y -= 0x4d;
}

// FUNCTION: LEGOLAND 0x00442d80
LEGO_EXPORT void UnAdjustBlokePosition(struct Point *pos) {
    pos->x += 0x4b;
    pos->y += 0x4d;
}

struct Vec3 {
    float x;
    float y;
    float z;
};

// FUNCTION: LEGOLAND 0x00442da0
void CrossProduct(struct Vec3 *a, struct Vec3 *b, struct Vec3 *out) {
    out->x = b->z * a->y - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

// FUNCTION: LEGOLAND 0x00442de0
float DotProduct(struct Vec3 *param_1, struct Vec3 *param_2) {
    return param_1->z * param_2->z + param_1->y * param_2->y + param_1->x * param_2->x;
}

// FUNCTION: LEGOLAND 0x00442e00
LEGO_EXPORT int TMNegParity(float *param_1) {
    struct Vec3 a;
    struct Vec3 b;
    struct Vec3 cross;

    a.x = param_1[0];
    a.y = param_1[1];
    a.z = param_1[2];
    b.x = param_1[3];
    b.y = param_1[4];
    b.z = param_1[5];
    CrossProduct(&a, &b, &cross);
    a.x = param_1[6];
    a.y = param_1[7];
    a.z = param_1[8];
    if (DotProduct(&cross, &a) < DoubleZero) {
        return 1;
    }
    return 0;
}

/* the render item pools (2048 and 20 nodes) */
// GLOBAL: LEGOLAND 0x00630108
struct RenderItemNode RenderItemPool[0x800];

// GLOBAL: LEGOLAND 0x00638218
struct RenderItemNode RenderItem2Pool[20];

// FUNCTION: LEGOLAND 0x00442e90
LEGO_EXPORT void RenderItems_New(void) {
    RenderItemPoolNext[0] = RenderItemPool;
    RenderItemCount = 0;
}

// FUNCTION: LEGOLAND 0x00442eb0
LEGO_EXPORT void RenderItem_Link(struct RenderItemNode **head, struct RenderItemNode *node, int key) {
    struct RenderItemNode *cur;

    node->next = NULL;
    node->prev = NULL;
    cur = *head;
    if (cur == NULL) {
        *head = node;
        return;
    }
    for (;;) {
        if (key <= cur->key) {
            node->next = cur;
            node->prev = cur->prev;
            if (cur->prev != NULL) {
                cur->prev->next = node;
            }
            cur->prev = node;
            if (cur == *head) {
                *head = node;
            }
            return;
        }
        if (cur->next == NULL) {
            cur->next = node;
            node->prev = cur;
            return;
        }
        cur = cur->next;
    }
}

// FUNCTION: LEGOLAND 0x00442f20
LEGO_EXPORT void AddBlokeToRenderList(struct RenderItemNode **head, struct RideNode *src, int key) {
    struct RenderItemNode *item = AllocRenderItem();
    item->item = src->rider;
    item->key = key;
    RenderItem_Link(head, (struct RenderItemNode *)item, key);
}

// FUNCTION: LEGOLAND 0x00442f50
struct RenderItemNode *AllocRenderItem(void) {
    struct RenderItemNode *result = RenderItemPoolNext[0];
    RenderItemPoolNext[0] = RenderItemPoolNext[0] + 1;
    RenderItemCount = RenderItemCount + 1;
    return result;
}

// FUNCTION: LEGOLAND 0x00442f70
LEGO_EXPORT void RenderBlokeList(struct RenderItemNode **list) {
    struct RenderItemNode *node = *list;
    while (node) {
        if (node->item) {
            IP_RenderBlokeIn3DNow((struct Bloke *)node->item);
        }
        node = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00442fa0
LEGO_EXPORT void Ride_SetFlagToNotLetAnyoneOn(void *param_1) {
    struct MapElement *cell;
    struct ObjInstance *instance;
    unsigned char *coords = param_1;
    int x = coords[0];
    int y = coords[1];

    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        cell = &GameMap[y][x];
    } else {
        cell = 0;
    }
    instance = (struct ObjInstance *)GetInstanceOfClass(cell->field_0->ride, (const TileId *)param_1);
    if (instance != 0) {
        *(unsigned char *)&instance->flags |= 2;
    }
}

// FUNCTION: LEGOLAND 0x00443000
LEGO_EXPORT void Ride_ClearFlagToNotLetAnyoneOn(void *param_1) {
    struct MapElement *cell;
    struct ObjInstance *instance;
    unsigned char *coords = param_1;
    int x = coords[0];
    int y = coords[1];

    if (x >= 0 && x < (int)lpConfig->width && y >= 0 && y < (int)lpConfig->height) {
        cell = &GameMap[y][x];
    } else {
        cell = 0;
    }
    instance = (struct ObjInstance *)GetInstanceOfClass(cell->field_0->ride, (const TileId *)param_1);
    if (instance != 0) {
        instance->flags &= 0xfffd;
    }
}

// FUNCTION: LEGOLAND 0x00443060
LEGO_EXPORT void RenderItems2_New(void) {
    RenderItem2PoolNext = RenderItem2Pool;
    RenderItem2Count = 0;
}

// FUNCTION: LEGOLAND 0x00443080
LEGO_EXPORT void RenderItem2_Link(struct RenderItemNode **head, struct RenderItemNode *node, int key) {
    struct RenderItemNode *cur;

    node->next = NULL;
    node->prev = NULL;
    cur = *head;
    if (cur == NULL) {
        *head = node;
        return;
    }
    for (;;) {
        if (key <= cur->key) {
            node->next = cur;
            node->prev = cur->prev;
            if (cur->prev != NULL) {
                cur->prev->next = node;
            }
            cur->prev = node;
            if (cur == *head) {
                *head = node;
            }
            return;
        }
        if (cur->next == NULL) {
            cur->next = node;
            node->prev = cur;
            return;
        }
        cur = cur->next;
    }
}

// FUNCTION: LEGOLAND 0x004430f0
LEGO_EXPORT void RenderItem2_AddItem(struct RenderItemNode **head, void *param_2, int key) {
    struct RenderItemNode *item = AllocRenderItem2();
    item->item = param_2;
    item->key = key;
    RenderItem2_Link(head, (struct RenderItemNode *)item, key);
}

// FUNCTION: LEGOLAND 0x00443120
struct RenderItemNode *AllocRenderItem2(void) {
    struct RenderItemNode *result = RenderItem2PoolNext;
    RenderItem2PoolNext = RenderItem2PoolNext + 1;
    RenderItem2Count = RenderItem2Count + 1;
    return result;
}

struct BlokeSex0 {
    unsigned char pad_0[4];
    struct BlokeSex1 *field_4;
};

struct BlokeSex1 {
    unsigned char pad_0[0x80];
    unsigned int field_80;
    unsigned int field_84;
    unsigned char pad_88[0x8c - 0x88];
    unsigned int field_8c;
    unsigned int field_90;
};

// FUNCTION: LEGOLAND 0x00443140
LEGO_EXPORT unsigned int GetSexOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    return inner->field_84;
}

// FUNCTION: LEGOLAND 0x00443150
LEGO_EXPORT char *GetFaceTextureNameOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    void *ptr;
    char *name;
    switch (inner->field_84) {
    case 0:
        ptr = AltManFileData;
        break;
    case 1:
        ptr = AltWomanFileData;
        break;
    }
    name = (char *)Get3DDataListString((char *)ptr, 0, inner->field_80);
    // STRING: LEGOLAND 0x004b7d24
    _stricmp(name, "chest girly1");
    // STRING: LEGOLAND 0x004b7d14
    return "chest girly2";
}

// FUNCTION: LEGOLAND 0x004431a0
LEGO_EXPORT char *GetChestTextureNameOfBloke(struct BlokeSex0 *param_1) {
    struct BlokeSex1 *inner = param_1->field_4;
    void *ptr;
    char *name;
    switch (inner->field_84) {
    case 0:
        ptr = AltManFileData;
        break;
    case 1:
        ptr = AltWomanFileData;
        break;
    }
    name = (char *)Get3DDataListString((char *)ptr, 1, inner->field_80);
    _stricmp(name, "chest girly1");
    return "chest girly2";
}

// GetLegColourOfBloke: the bloke's leg colour as 0xRRGGBB, from the BlokeColours entry (3 bytes: R, G, B) its
// person's field_8c selects.
// FUNCTION: LEGOLAND 0x004431f0
LEGO_EXPORT unsigned int GetLegColourOfBloke(struct BlokeSex0 *param_1) {
    volatile unsigned int idx = param_1->field_4->field_8c;
    unsigned int g = BlokeColours[idx * 3 + 1];
    unsigned int b = BlokeColours[idx * 3 + 2];
    unsigned int r = BlokeColours[idx * 3];
    return (((r << 8) | g) << 8) | b;
}

// GetArmColourOfBloke: the same as GetLegColourOfBloke for the arm colour (the entry field_90 selects).
// FUNCTION: LEGOLAND 0x00443220
LEGO_EXPORT unsigned int GetArmColourOfBloke(struct BlokeSex0 *param_1) {
    unsigned int idx = param_1->field_4->field_90;
    unsigned int g = BlokeColours[idx * 3 + 1];
    unsigned int b = BlokeColours[idx * 3 + 2];
    volatile int r;
    r = BlokeColours[3 * idx];
    return (((r << 8) | g) << 8) | b;
}

// FUNCTION: LEGOLAND 0x00443250
float SinFloat(float param_1) {
    return (float)sin(param_1);
}

// FUNCTION: LEGOLAND 0x00443260
float CosFloat(float param_1) {
    return (float)cos(param_1);
}

// FUNCTION: LEGOLAND 0x00443270
LEGO_EXPORT void MatrixMultiply(float *A, float *B, float *C) {
    C[0] = A[0] * B[0] + A[1] * B[3] + A[2] * B[6];
    C[1] = A[1] * B[4] + A[2] * B[7] + A[0] * B[1];
    C[2] = A[1] * B[5] + A[2] * B[8] + A[0] * B[2];

    C[3] = A[4] * B[3] + A[3] * B[0] + A[5] * B[6];
    C[4] = A[5] * B[7] + A[4] * B[4] + A[3] * B[1];
    C[5] = A[5] * B[8] + A[3] * B[2] + A[4] * B[5];

    C[6] = A[6] * B[0] + A[8] * B[6] + A[7] * B[3];
    C[7] = A[8] * B[7] + A[7] * B[4] + A[6] * B[1];
    C[8] = A[8] * B[8] + A[7] * B[5] + A[6] * B[2];
}

// FUNCTION: LEGOLAND 0x00443360
LEGO_EXPORT void BuildYRotationMatrix(float angle, float *out) {
    float s = SinFloat(angle);
    float c = CosFloat(angle);
    out[0] = c;
    out[6] = -s;
    out[1] = 0.0f;
    out[2] = s;
    out[8] = c;
    out[3] = 0.0f;
    out[4] = 1.0f;
    out[5] = 0.0f;
    out[7] = 0.0f;
}

// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x004433b0
__declspec(naked) LEGO_EXPORT void TransformVectorsL(void) {
    __asm {
        push ebp
        mov ebp, esp
        push ebx
        push esi
        push edi
        push esi
        push edi
        mov ebx, dword ptr [ebp + 0x10]
        mov edi, dword ptr [ebp + 0x14]
L4433be:
        mov esi, dword ptr [ebp + 8]
        mov eax, dword ptr [ebx]
        imul dword ptr [esi]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 4]
        imul dword ptr [esi + 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 8]
        imul dword ptr [esi + 8]
        shrd eax, edx, 0x10
        add ecx, eax
        push ecx
        mov eax, dword ptr [ebx + 0xc]
        imul dword ptr [esi]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 0x10]
        imul dword ptr [esi + 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 0x14]
        imul dword ptr [esi + 8]
        shrd eax, edx, 0x10
        add ecx, eax
        push ecx
        mov eax, dword ptr [ebx + 0x18]
        imul dword ptr [esi]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 0x1c]
        imul dword ptr [esi + 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 0x20]
        imul dword ptr [esi + 8]
        shrd eax, edx, 0x10
        add ecx, eax
        mov esi, dword ptr [ebp + 0xc]
        mov dword ptr [esi + 8], ecx
        pop dword ptr [esi + 4]
        pop dword ptr [esi]
        add dword ptr [ebp + 8], 0xc
        add dword ptr [ebp + 0xc], 0xc
        dec edi
        jne L4433be
        pop edi
        pop esi
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x00443450
LEGO_EXPORT void NormaliseVector(struct Vec3 *v) {
    float m = (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x = v->x / m;
    v->y = v->y / m;
    v->z = v->z / m;
}

struct Matrix3x3 {
    unsigned int m[9];
};

// FUNCTION: LEGOLAND 0x00443490
LEGO_EXPORT void CopyMatrix(struct Matrix3x3 *src, struct Matrix3x3 *dest) {
    dest->m[0] = src->m[0];
    dest->m[1] = src->m[1];
    dest->m[2] = src->m[2];
    dest->m[3] = src->m[3];
    dest->m[4] = src->m[4];
    dest->m[5] = src->m[5];
    dest->m[6] = src->m[6];
    dest->m[7] = src->m[7];
    dest->m[8] = src->m[8];
}
