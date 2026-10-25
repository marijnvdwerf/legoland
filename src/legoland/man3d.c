#include "man3d.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloke.h"
#include "challenge.h"
#include "draw.h"
#include "globals.h"
#include "legoland.h"
#include "map_object.h"
#include "math.h"
#include "print_sprite.h"
#include "render.h"
#include "render3d.h"
#include "resource.h"
#include "worker_mouse.h"

struct Mesh {
    /* 0x00 */ int count;
    /* 0x04 */ struct MeshElem *elems;
    /* 0x08 */ void *field_8;
};

// Loads a .pos animation file: two counts (frames per entry, entries), then for every frame a
// position (3 floats) and a 3x3 matrix (9 floats). Each matrix is turned 90 degrees about Y
// (BuildYRotationMatrix(pi/2) * mat) as it is loaded. Scale fields start at 1.0, offsets at 0.
// FUNCTION: LEGOLAND 0x0043f660
LEGO_EXPORT struct Position *LoadPos(const char *path) {
    struct ResFile *file;
    struct Position *pos;
    struct PosFrame *frames;
    float rot[3][3];
    float tmp[3][3];
    int i;
    int j;
    int row;
    int col;

    file = RES_OpenFile(path);
    pos = (struct Position *)malloc(sizeof(struct Position));
    pos->field_8 = 1.0f;
    pos->field_c = 1.0f;
    pos->field_10 = 1.0f;
    pos->field_14 = 0;
    pos->field_18 = 0;
    RES_ReadFile(file, &pos->count_inner, 4);
    RES_ReadFile(file, &pos->count, 4);
    pos->entries = (struct PosFrame **)malloc(pos->count * sizeof(struct PosFrame *));
    for (i = 0; i < pos->count; i++) {
        pos->entries[i] = (struct PosFrame *)malloc(pos->count_inner * sizeof(struct PosFrame));
        frames = pos->entries[i];
        for (j = 0; j < pos->count_inner; j++) {
            RES_ReadFile(file, &frames[j].pos[0], 4);
            RES_ReadFile(file, &frames[j].pos[1], 4);
            RES_ReadFile(file, &frames[j].pos[2], 4);
            for (row = 0; row < 3; row++) {
                for (col = 0; col < 3; col++) {
                    RES_ReadFile(file, &frames[j].mat[row][col], 4);
                }
            }
            BuildYRotationMatrix(1.5707963f, &rot[0][0]);
            MatrixMultiply(&rot[0][0], &frames[j].mat[0][0], &tmp[0][0]);
            CopyMatrix((struct Matrix3x3 *)&tmp[0][0], (struct Matrix3x3 *)&frames[j].mat[0][0]);
        }
    }
    RES_CloseFile(file);
    return pos;
}

// FUNCTION: LEGOLAND 0x0043f7d0
LEGO_EXPORT void UnloadPos(struct Position *pos) {
    int i;

    i = 0;
    if (pos->count > 0) {
        do {
            free(pos->entries[i]);
            i++;
        } while (i < pos->count);
    }
    free(pos->entries);
    free(pos);
}

// FUNCTION: LEGOLAND 0x0043f810
void AddPersonToList(struct Person *person) {
    person->prev = 0;
    person->next = 0;
    if (PersonListHead != 0) {
        person->next = PersonListHead;
        ((struct Person *)PersonListHead)->prev = person;
    }
    PersonListHead = person;
}

// FUNCTION: LEGOLAND 0x0043f840
void RemovePersonFromList(struct Person *person) {
    if (person->prev != 0) {
        person->prev->next = person->next;
    } else {
        PersonListHead = person->next;
    }
    if (person->next != 0) {
        person->next->prev = person->prev;
    }
}

// FUNCTION: LEGOLAND 0x0043f870
void FreePerson(struct Person *person) {
    if (person->field_50 != 0) {
        free(person->field_50);
    }
    free(person);
}

// FUNCTION: LEGOLAND 0x0043f890
LEGO_EXPORT struct Person *Find3DPersonFromBloke(struct Bloke *bloke) {
    struct Person *person;

    person = PersonListHead;
    if (person == 0) {
        return 0;
    }
    while (person->bloke != bloke) {
        person = person->next;
        if (person == 0) {
            return 0;
        }
    }
    return person;
}

/* Allocates a zeroed 3D person for bloke with the given character (character 1 gets a random variant bit) and fills
 * in its default fields. */
// FUNCTION: LEGOLAND 0x0043f8c0
struct Person *CreatePerson(struct Bloke *param_1, unsigned int param_2) {
    struct Person *person;

    person = malloc(sizeof(struct Person));
    if (person != 0) {
        memset(person, 0, sizeof(struct Person));
        if (param_2 == 1) {
            person->random = rand() & 1;
        } else {
            person->random = 0;
        }
        person->character = param_2;
        person->bloke = param_1;
        person->field_7c = 0xffffffff;
        person->field_80 = 0xffffffff;
        person->anim = 0xffffffff;
        person->field_90 = 0xffffffff;
        person->field_8c = 0xffffffff;
        person->field_10 = 0x40000000;
        person->field_14 = 0x40000000;
        person->field_18 = 0x40000000;
        person->prev = 0;
        person->next = 0;
        person->frame = 0;
        person->field_1c = 0;
        person->field_20 = 0;
        person->field_2c = 0;
        person->field_30 = 0;
        person->field_34 = 0xff;
        person->field_38 = 0;
    }
    return person;
}

// FUNCTION: LEGOLAND 0x0043f970
void RelocateLocData(struct LocFile *buffer) {
    struct LocFile *p = buffer;

    *(UINT_PTR *)&p->textures = *(UINT_PTR *)&p->textures + (UINT_PTR)p;
    *(UINT_PTR *)&p->cells = *(UINT_PTR *)&p->cells + (UINT_PTR)p;
}

// FUNCTION: LEGOLAND 0x0043f990
struct LocFile *LoadLocFile(const char *param_1, const char *param_2) {
    char path[256];
    struct ResFile *file;
    unsigned int size;
    struct LocFile *buffer;

    sprintf(path, Path3DFormat, param_2, param_1);
    file = RES_OpenFile(path);
    if (file != 0) {
        size = RES_GetFileSize(file);
        buffer = malloc(size);
        if (buffer != 0) {
            RES_ReadFile(file, buffer, size);
            RES_CloseFile(file);
            RelocateLocData(buffer);
            return buffer;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0043fa10
void ClampMeshUVs(float *param_1, int param_2) {
    int n;
    if (param_2 > 0) {
        n = param_2;
        do {
            if (*param_1 < FLOAT_004ab390) {
                *param_1 = 0.0f;
            }
            if (*param_1 > 1.0f) {
                *param_1 = 1.0f;
            }
            if (param_1[1] < FLOAT_004ab390) {
                param_1[1] = 0.0f;
            }
            if (param_1[1] > 1.0f) {
                param_1[1] = 1.0f;
            }
            param_1 = param_1 + 2;
            n = n - 1;
        } while (n != 0);
    }
}

// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x0043fa80
__declspec(naked) void *Load3DMesh(const char *name, const char *dir, unsigned int ctx) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x11c
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp + 0xc]
        push ebx
        push esi
        push edi
        push eax
        push ecx
        lea edx, [ebp - 0x11c]
        push offset Path3DFormat
        push edx
        mov dword ptr [ebp - 0x10], 0x47800000
        xor esi, esi
        call sprintf
        lea eax, [ebp - 0x11c]
        push eax
        call RES_OpenFile
        mov ebx, eax
        add esp, 0x14
        test ebx, ebx
        je L43fdd3
        push 0x24
        call malloc
        mov esi, eax
        mov ecx, 9
        xor eax, eax
        mov edi, esi
        rep stosd
        push 0xc
        call malloc
        xor ecx, ecx
        mov dword ptr [ebp - 0x14], eax
        mov dword ptr [eax], ecx
        lea edx, [ebp + 8]
        push 4
        push edx
        mov dword ptr [eax + 4], ecx
        push ebx
        mov dword ptr [eax + 8], ecx
        call RES_ReadFile
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [esi], eax
        mov eax, dword ptr [ebp + 8]
        lea ecx, [eax*8]
        sub ecx, eax
        shl ecx, 3
        push ecx
        call malloc
        mov edi, eax
        add esp, 0x18
        mov dword ptr [esi + 4], edi
        mov eax, dword ptr [ebp + 8]
        lea ecx, [eax*8]
        sub ecx, eax
        xor eax, eax
        shl ecx, 3
        mov edx, ecx
        shr ecx, 2
        rep stosd
        mov ecx, edx
        and ecx, 3
        rep stosb
        mov eax, dword ptr [ebp + 8]
        xor edi, edi
        cmp eax, edi
        mov dword ptr [ebp - 0x18], edi
        jle L43fcf6
L43fb4c:
        lea eax, [ebp - 4]
        push 4
        push eax
        push ebx
        call RES_ReadFile
        mov ecx, dword ptr [esi + 4]
        mov edx, dword ptr [ebp - 4]
        mov dword ptr [ecx + edi + 0x18], edx
        mov eax, dword ptr [ebp - 4]
        lea eax, [eax + eax*2]
        shl eax, 2
        push eax
        call malloc
        mov ecx, dword ptr [esi + 4]
        mov dword ptr [ecx + edi + 0x1c], eax
        mov ecx, dword ptr [ebp - 4]
        mov edx, dword ptr [esi + 4]
        lea ecx, [ecx + ecx*2]
        mov eax, dword ptr [edx + edi + 0x1c]
        shl ecx, 2
        push ecx
        push eax
        push ebx
        mov dword ptr [ebp - 8], eax
        call RES_ReadFile
        mov eax, dword ptr [ebp - 4]
        add esp, 0x1c
        xor ecx, ecx
        test eax, eax
        jle L43fbbf
        xor edx, edx
L43fba1:
        mov eax, dword ptr [esi + 4]
        inc ecx
        mov eax, dword ptr [eax + edi + 0x1c]
        fld dword ptr [eax + edx + 4]
        lea eax, [eax + edx + 4]
        add edx, 0xc
        fchs
        fstp dword ptr [eax]
        mov eax, dword ptr [ebp - 4]
        cmp ecx, eax
        jl L43fba1
L43fbbf:
        lea edx, [eax + eax*2]
        xor ecx, ecx
        shl edx, 2
        test edx, edx
        mov dword ptr [ebp - 0xc], ecx
        jle L43fbee
L43fbce:
        mov eax, dword ptr [ebp - 8]
        add eax, dword ptr [ebp - 0xc]
        fld dword ptr [eax]
        fmul dword ptr [ebp - 0x10]
        fistp dword ptr [eax]
        mov eax, dword ptr [ebp - 4]
        add ecx, 4
        mov dword ptr [ebp - 0xc], ecx
        lea eax, [eax + eax*2]
        shl eax, 2
        cmp ecx, eax
        jl L43fbce
L43fbee:
        lea ecx, [ebp + 0xc]
        push 4
        push ecx
        push ebx
        call RES_ReadFile
        mov edx, dword ptr [esi + 4]
        mov eax, dword ptr [ebp + 0xc]
        mov dword ptr [edx + edi + 0x24], eax
        mov eax, dword ptr [ebp + 0xc]
        lea ecx, [eax + eax*2]
        shl ecx, 2
        push ecx
        call malloc
        mov edx, dword ptr [esi + 4]
        mov dword ptr [edx + edi + 0x28], eax
        mov ecx, dword ptr [ebp + 0xc]
        mov eax, dword ptr [esi + 4]
        lea ecx, [ecx + ecx*2]
        mov eax, dword ptr [eax + edi + 0x28]
        shl ecx, 2
        push ecx
        push eax
        push ebx
        mov dword ptr [ebp - 0x1c], eax
        call RES_ReadFile
        mov eax, dword ptr [ebp + 0xc]
        xor ecx, ecx
        add esp, 0x1c
        cmp eax, ecx
        mov dword ptr [ebp - 8], ecx
        jle L43fc73
        mov dword ptr [ebp - 0xc], ecx
L43fc47:
        mov edx, dword ptr [esi + 4]
        mov eax, dword ptr [edx + edi + 0x28]
        mov edx, dword ptr [ebp - 0xc]
        add eax, edx
        push eax
        call NormaliseVector
        mov eax, dword ptr [ebp - 0xc]
        mov ecx, dword ptr [ebp - 8]
        add eax, 0xc
        add esp, 4
        mov dword ptr [ebp - 0xc], eax
        mov eax, dword ptr [ebp + 0xc]
        inc ecx
        cmp ecx, eax
        mov dword ptr [ebp - 8], ecx
        jl L43fc47
L43fc73:
        xor ecx, ecx
        test eax, eax
        jle L43fc99
        xor edx, edx
L43fc7b:
        mov eax, dword ptr [esi + 4]
        inc ecx
        mov eax, dword ptr [eax + edi + 0x28]
        fld dword ptr [eax + edx + 4]
        lea eax, [eax + edx + 4]
        add edx, 0xc
        fchs
        fstp dword ptr [eax]
        mov eax, dword ptr [ebp + 0xc]
        cmp ecx, eax
        jl L43fc7b
L43fc99:
        lea edx, [eax + eax*2]
        xor ecx, ecx
        shl edx, 2
        test edx, edx
        mov dword ptr [ebp - 8], ecx
        jle L43fcc8
L43fca8:
        mov eax, dword ptr [ebp - 0x1c]
        add eax, dword ptr [ebp - 8]
        fld dword ptr [eax]
        fmul dword ptr [ebp - 0x10]
        fistp dword ptr [eax]
        mov eax, dword ptr [ebp + 0xc]
        add ecx, 4
        mov dword ptr [ebp - 8], ecx
        lea eax, [eax + eax*2]
        shl eax, 2
        cmp ecx, eax
        jl L43fca8
L43fcc8:
        mov ecx, dword ptr [esi + 4]
        mov edx, dword ptr [ebp - 0x14]
        mov dword ptr [ecx + edi + 0x20], edx
        mov eax, dword ptr [esi + 4]
        add eax, edi
        push eax
        push eax
        call ComputeMeshElemBounds
        mov eax, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [ebp + 8]
        add esp, 8
        inc eax
        add edi, 0x38
        cmp eax, ecx
        mov dword ptr [ebp - 0x18], eax
        jl L43fb4c
L43fcf6:
        lea ecx, [ebp + 8]
        push 4
        push ecx
        push ebx
        call RES_ReadFile
        mov edi, dword ptr [ebp - 0x14]
        push 4
        lea edx, [edi + 4]
        push edx
        push ebx
        call RES_ReadFile
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [edi], eax
        mov eax, dword ptr [ebp + 8]
        lea ecx, [eax + eax*2]
        shl ecx, 2
        push ecx
        call malloc
        mov dword ptr [edi + 8], eax
        mov ecx, dword ptr [ebp + 8]
        lea edx, [ecx + ecx*2]
        shl edx, 2
        push edx
        push eax
        push ebx
        call RES_ReadFile
        mov eax, dword ptr [ebp + 8]
        lea eax, [eax + eax*8]
        shl eax, 2
        push eax
        call malloc
        mov dword ptr [esi + 8], eax
        mov ecx, dword ptr [ebp + 8]
        lea ecx, [ecx + ecx*8]
        shl ecx, 2
        push ecx
        push eax
        push ebx
        call RES_ReadFile
        push ebx
        call RES_CloseFile
        add esp, 0x3c
        test esi, esi
        je L43fdd3
        mov eax, dword ptr [ebp + 8]
        xor ebx, ebx
        test eax, eax
        jle L43fdd3
        xor edi, edi
L43fd74:
        mov edx, dword ptr [esi + 8]
        mov ecx, dword ptr [edi + edx]
        lea eax, [edi + edx]
        test ch, 0x20
        je L43fdab
        mov cl, byte ptr [eax + 6]
        mov byte ptr [ebp + 0xe], cl
        mov dl, byte ptr [eax + 5]
        lea ecx, [ebp + 0xc]
        mov byte ptr [ebp + 0xd], dl
        mov al, byte ptr [eax + 4]
        push ecx
        push 0x40
        mov byte ptr [ebp + 0xc], al
        call GetOrCreateColorRamp
        mov edx, dword ptr [esi + 8]
        add esp, 8
        mov dword ptr [edi + edx + 8], eax
        jmp L43fdb6
L43fdab:
        mov ecx, dword ptr [ebp + 0x10]
        mov edx, dword ptr [eax + 8]
        add edx, ecx
        mov dword ptr [eax + 8], edx
L43fdb6:
        mov edx, dword ptr [esi + 8]
        push 3
        lea eax, [edi + edx + 0xc]
        push eax
        call ClampMeshUVs
        mov eax, dword ptr [ebp + 8]
        add esp, 8
        inc ebx
        add edi, 0x24
        cmp ebx, eax
        jl L43fd74
L43fdd3:
        mov eax, esi
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x0043fde0
void FreeMesh(struct Mesh *mesh) {
    int count;
    int i;

    if (mesh != 0) {
        count = mesh->count;
        if (count > 0) {
            i = 0;
            do {
                free(mesh->elems[i].verts);
                free(mesh->elems[i].norms);
                i = i + 1;
                count = count - 1;
            } while (count != 0);
        }
        free(mesh->elems->shared->field_8);
        free(mesh->elems->shared);
        free(mesh->elems);
        free(mesh->field_8);
        free(mesh);
    }
}

// FUNCTION: LEGOLAND 0x0043fe50
LEGO_EXPORT void Render3DPerson(struct Person *person) {
    RECT bounds;
    RECT clip;
    struct VideoArg vid;

    person->scale.x = 1.0f;
    person->scale.y = 1.0f;
    person->scale.z = 1.0f;
    /* 160x120 render box at the person's screen position */
    bounds.left = person->screen.x;
    bounds.top = person->screen.y;
    bounds.right = bounds.left + 0xa0;
    bounds.bottom = bounds.top + 0x78;
    clip = SPRITE_ClipRect;
    clip.right--;
    clip.bottom--;
    if (IntersectRect(&clip, &bounds, &clip) != 0) {
        OffsetRect(&clip, -person->screen.x, -person->screen.y);
        if (GetVideoSurface(&vid) != 0) {
            SetRender3DTarget((unsigned char *)vid.bits + person->screen.y * vid.pitch + person->screen.x * 2, vid.pitch,
                vid.width, vid.height);
            StartMousePixelHitTest((unsigned char *)vid.bits, (struct RenderViewport *)&MousePos);
            Render_SetViewport(&clip);
            __asm { fstcw word ptr [SavedFPUControlWord] }
            __asm {fldcw word ptr[PersonRenderFpuControlWord]} DrawPersonMesh(person);
            __asm { fldcw word ptr [SavedFPUControlWord] }
            if (MousePixelCovered != 0) {
                if (WorkerOnMouseActive != 0 && person->bloke == GetWorkerOnMouse()) {
                    return;
                }
                switch (person->character) {
                case 2:
                    Hover.type = 0x307;
                    break;
                case 3:
                    Hover.type = 0x308;
                    break;
                default:
                    Hover.type = 0x306;
                    break;
                }
                Hover.ptr = person->bloke;
            }
        }
    }
}

// FUNCTION: LEGOLAND 0x0043ffb0
LEGO_EXPORT void RenderBlokeIn3D(struct Bloke *bloke) {
    struct Person *person;

    person = bloke->person;
    if (person != 0) {
        Render3DPerson(person);
    }
}

// FUNCTION: LEGOLAND 0x0043ffd0
LEGO_EXPORT void SortBlokeIn3D(struct Bloke *bloke) {
    struct {
        unsigned int field_0;
        struct Bloke *bloke;
        unsigned short field_8;
    } info;

    info.field_0 = 0x306;
    info.bloke = bloke;
    info.field_8 = 0;
    if (bloke->person != 0) {
        SortPerson(bloke->person, bloke->person->sort_id, &info);
    }
}

// FUNCTION: LEGOLAND 0x00440010
LEGO_EXPORT void IP_RenderBlokeIn3DNow(struct Bloke *bloke) {
    RenderBlokeIn3D(bloke);
}

// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00440020
__declspec(naked) LEGO_EXPORT void SetPersonRotation(struct Person *person, float *src) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0xc
        mov eax, dword ptr [ebp + 0xc]
        push ebx
        push esi
        push edi
        mov edi, dword ptr [ebp + 8]
        mov ecx, dword ptr [eax]
        mov dword ptr [ebp - 8], 0x47800000
        mov dword ptr [edi + 0x40], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [edi + 0x44], edx
        mov ecx, dword ptr [eax + 8]
        mov dword ptr [edi + 0x48], ecx
        mov edx, dword ptr [eax + 4]
        lea eax, [edi + 0x58]
        mov dword ptr [ebp - 0xc], edx
        mov dword ptr [ebp - 4], eax
        push esi
        fld dword ptr [ebp - 0xc]
        fsin
        fmul dword ptr [ebp - 8]
        fistp dword ptr [ebp + 8]
        fld dword ptr [ebp - 0xc]
        fcos
        fmul dword ptr [ebp - 8]
        fistp dword ptr [ebp + 0xc]
        mov eax, dword ptr [ebp - 4]
        xor ebx, ebx
        mov ecx, 0x10000
        mov edx, dword ptr [ebp + 8]
        mov esi, dword ptr [ebp + 0xc]
        mov dword ptr [eax], esi
        mov dword ptr [eax + 4], ebx
        mov dword ptr [eax + 8], edx
        mov dword ptr [eax + 0xc], ebx
        mov dword ptr [eax + 0x10], ecx
        mov dword ptr [eax + 0x14], ebx
        neg edx
        mov dword ptr [eax + 0x18], edx
        mov dword ptr [eax + 0x1c], ebx
        mov dword ptr [eax + 0x20], esi
        pop esi
        mov ecx, dword ptr [edi + 0x68]
        neg ecx
        mov dword ptr [edi + 0x68], ecx
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

// FUNCTION: LEGOLAND 0x004400b0
LEGO_EXPORT void SetPersonDirection(struct Person *person, unsigned int direction) {
    person->field_48 = 0.0f;
    person->field_40 = 0.0f;
    switch (direction) {
    case 0:
        person->field_44 = -0.7853979468345642f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 1:
        person->field_44 = 4.712387561798096f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 2:
        person->field_44 = 3.926989793777466f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 3:
        person->field_44 = 3.141591787338257f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 4:
        person->field_44 = 2.356193780899048f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 5:
        person->field_44 = 1.5707958936691284f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 6:
        person->field_44 = 0.7853979468345642f;
        SetPersonRotation(person, &person->field_40);
        return;
    case 7:
        person->field_44 = 0.0f;
    }
    SetPersonRotation(person, &person->field_40);
}

// FUNCTION: LEGOLAND 0x00440190
LEGO_EXPORT void SetPersonPosition(struct Person *person, unsigned int x, unsigned int y) {
    person->field_1c = x;
    person->field_20 = y;
}

/* Copies a bloke's state to its 3D person: direction, screen position (isometric projection of the map position
 * minus the scroll, plus the view offset, raised by half the bloke height), sort id and, unless bloke flag 0x100 is
 * set, the animation frame. */
// FUNCTION: LEGOLAND 0x004401b0
void UpdatePersonFromBloke(struct Person *person, struct Bloke *bloke) {
    struct Point pt;
    int x;
    int y;
    short s;
    int w;
    int h;

    int dir;
    dir = bloke->dir;
    SetPersonDirection(person, dir);
    y = bloke->pos.y;
    x = bloke->pos.x;
    GetTileDimensions(&w, &h);
    pt.x = (x - y) * w;
    pt.y = (y + x) * h;
    pt.x >>= 9;
    pt.y >>= 9;
    s = (short)Get_XScroll();
    pt.x -= s;
    s = (short)Get_YScroll();
    pt.y -= s;
    person->sort_id = pt.y;
    pt.x += lpConfig->view_x;
    pt.y = pt.y + (lpConfig->view_y - (bloke->height >> 1));
    AdjustBlokePosition(&pt);
    SetPersonPosition(person, pt.x, pt.y);
    if (!(bloke->flags & 0x100)) {
        person->frame = bloke->frame;
    }
}

// FUNCTION: LEGOLAND 0x00440290
LEGO_EXPORT void UpdatePerson(Bloke *bloke) {
    if ((bloke->flags & 0x80) != 0) {
        return;
    }
    if (bloke->person == 0) {
        return;
    }
    UpdatePersonFromBloke(bloke->person, bloke);
}

// FUNCTION: LEGOLAND 0x004402b0
LEGO_EXPORT void Control3DPeople(void) {
    Bloke *bloke;

    bloke = FirstBloke;
    if (bloke == 0) {
        return;
    }
    do {
        UpdatePerson(bloke);
        bloke = bloke->next;
    } while (bloke != 0);
}

// FUNCTION: LEGOLAND 0x004402d0
void *Load3DDataFile(const char *param_1, const char *param_2) {
    char path[256];
    struct ResFile *file;
    unsigned int size;
    void *buffer;

    sprintf(path, Path3DFormat, param_1, param_2);
    file = RES_OpenFile(path);
    if (file != 0) {
        size = RES_GetFileSize(file);
        buffer = malloc(size);
        if (buffer != 0) {
            RES_ReadFile(file, buffer, size);
            RES_CloseFile(file);
        }
    }
    return buffer;
}

// FUNCTION: LEGOLAND 0x00440350
LEGO_EXPORT void InitMan(void) {
    unsigned int ctx;
    // STRING: LEGOLAND 0x004b7bb0
    const char *proj = "NewProject.txt";

    InitRender3D((DisplayPixelFormat == 2) + 5);
    ctx = GetLoadedTextureCount();
    // STRING: LEGOLAND 0x004b7cdc
    VisitorLocData = LoadLocFile("NewProject.loc", "visitor");
    VisitorLocData->texture_base = ctx;
    // STRING: LEGOLAND 0x004b7cec
    LoadTextureBitmaps(VisitorLocData, "visitor");
    // STRING: LEGOLAND 0x004b7cc4
    WomanMeshes[1] = Load3DMesh("WomanWalk.WomanWalk.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7cac
    WomanMeshes[0] = Load3DMesh("WomanSit.WomanSit.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c94
    WomanMeshes[2] = Load3DMesh("WomanWave.WomanWave.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c78
    WomanMeshes[3] = Load3DMesh("WomanStand.WomanStand.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c5c
    WomanMeshes[5] = Load3DMesh("WomanPanWalk.WomPanWalk.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c44
    WomanMeshes[4] = Load3DMesh("WomanPan.WomanPan.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c30
    ManMeshes[1] = Load3DMesh("ManWalk.ManWalk.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c1c
    ManMeshes[0] = Load3DMesh("ManSit.ManSit.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7c08
    ManMeshes[2] = Load3DMesh("ManWave.ManWave.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7bf0
    ManMeshes[3] = Load3DMesh("ManStand.ManStand.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7bd4
    ManMeshes[5] = Load3DMesh("ManPanWalk.ManPanWalk.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7bc0
    ManMeshes[4] = Load3DMesh("ManPan.ManPan.3d", "visitor", ctx);
    // STRING: LEGOLAND 0x004b7ba4
    LoadPersonAltTextureList("altman.txt", proj, "visitor", 0, ctx);
    // STRING: LEGOLAND 0x004b7b94
    LoadPersonAltTextureList("altwoman.txt", proj, "visitor", 1, ctx);
    AltManFileData = Load3DDataFile("visitor", "altman.txt");
    AltWomanFileData = Load3DDataFile("visitor", "altwoman.txt");
    ctx = GetLoadedTextureCount();
    // STRING: LEGOLAND 0x004b7b80
    GeoffLocData = LoadLocFile("geoff.loc", "geoff");
    GeoffLocData->texture_base = ctx;
    // STRING: LEGOLAND 0x004b7b8c
    LoadTextureBitmaps(GeoffLocData, "geoff");
    // STRING: LEGOLAND 0x004b7b68
    GeoffMeshes[0] = Load3DMesh("geofWalk.GeofWalk.3d", "geoff", ctx);
    // STRING: LEGOLAND 0x004b7b50
    GeoffMeshes[1] = Load3DMesh("GeofPour.GeofPour.3d", "geoff", ctx);
    ctx = GetLoadedTextureCount();
    // STRING: LEGOLAND 0x004b7b3c
    TracyLocData = LoadLocFile("tracy.loc", "tracy");
    TracyLocData->texture_base = ctx;
    // STRING: LEGOLAND 0x004b7b48
    LoadTextureBitmaps(TracyLocData, "tracy");
    // STRING: LEGOLAND 0x004b7b24
    TracyWalkMesh = Load3DMesh("TracyWalk.TraceWalk.3d", "tracy", ctx);
}

// FUNCTION: LEGOLAND 0x004405a0
LEGO_EXPORT void UnInitMan(void) {
    void **p;

    if (VisitorLocData != 0) {
        free(VisitorLocData);
    }
    if (GeoffLocData != 0) {
        free(GeoffLocData);
    }
    if (TracyLocData != 0) {
        free(TracyLocData);
    }
    if (AltManFileData != 0) {
        free(AltManFileData);
    }
    if (AltWomanFileData != 0) {
        free(AltWomanFileData);
    }
    p = ManMeshes;
    do {
        if (*p != 0) {
            FreeMesh(*p);
        }
        p++;
    } while ((INT_PTR)p < (INT_PTR)WomanMeshes);
    p = WomanMeshes;
    do {
        if (*p != 0) {
            FreeMesh(*p);
        }
        p++;
    } while ((INT_PTR)p < (INT_PTR)RenderItemPoolNext);
    p = GeoffMeshes;
    do {
        if (*p != 0) {
            FreeMesh(*p);
        }
        p++;
    } while ((INT_PTR)p < (INT_PTR)AltManTextureCountB);
    if (TracyWalkMesh != 0) {
        FreeMesh(TracyWalkMesh);
    }
    FreeRender3DBuffers();
    FreeCursorCache();
    FreeTextureNodes();
    FreeZBuffer();
}

// FUNCTION: LEGOLAND 0x00440680
LEGO_EXPORT void Add3DBlokeToList(struct Bloke *bloke, unsigned int param_2) {
    struct Person *person;

    person = CreatePerson(bloke, param_2);
    bloke->person = person;
    if (person != 0) {
        AddPersonToList(person);
        UpdatePersonFromBloke(person, bloke);
        BlokeWalkAnim(bloke);
    }
}

// FUNCTION: LEGOLAND 0x004406c0
LEGO_EXPORT void BlokeSetAnim(struct Bloke *bloke, int anim) {
    struct Person *person;
    int kind;
    void **base;
    struct Mesh *mesh;
    struct LocFile *context;

    person = bloke->person;
    if (person->anim != (unsigned int)anim) {
        kind = person->character;
        person->anim = anim;
        switch (kind) {
        case 1:
            if (person->random == 0) {
                base = ManMeshes;
            } else {
                base = WomanMeshes;
            }
            break;
        case 2:
            base = GeoffMeshes;
            break;
        case 3:
            base = &TracyWalkMesh;
            break;
        }
        mesh = (struct Mesh *)base[anim];
        if (kind == 1 && person->field_50 != 0) {
            free(person->field_50);
        }
        switch (person->character) {
        case 1:
            context = VisitorLocData;
            break;
        case 2:
            context = GeoffLocData;
            break;
        case 3:
            context = TracyLocData;
            break;
        }
        {
            struct MeshElem *e = mesh->elems;
            struct PersonFace *f8 = mesh->field_8;
            person->field_50 = CreatePersonFaceList(person, context, f8, e->shared->count, person->random);
        }
    }
}

// FUNCTION: LEGOLAND 0x00440780
LEGO_EXPORT void BlokeSitAnim(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 0);
}

// FUNCTION: LEGOLAND 0x00440790
LEGO_EXPORT struct Anim3D *GetBlokeAnim3D(struct Bloke *bloke) {
    struct Person *person;
    struct Anim3D *result;
    void **base;

    result = 0;
    person = bloke->person;
    if (person != 0) {
        switch (person->character) {
        case 1:
            if (person->random == 0) {
                base = ManMeshes;
            } else {
                base = WomanMeshes;
            }
            break;
        case 2:
            base = GeoffMeshes;
            break;
        case 3:
            base = &TracyWalkMesh;
            break;
        }
        result = (struct Anim3D *)base[person->anim];
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00440800
LEGO_EXPORT struct Anim3D *GetBlokeAnim3DFromPerson(struct Person *person) {
    struct Anim3D *result;
    void **base;

    result = 0;
    if (person != 0) {
        switch (person->character) {
        case 1:
            if (person->random == 0) {
                base = ManMeshes;
            } else {
                base = WomanMeshes;
            }
            break;
        case 2:
            base = GeoffMeshes;
            break;
        case 3:
            base = &TracyWalkMesh;
            break;
        }
        result = (struct Anim3D *)base[person->anim];
    }
    return result;
}

// FUNCTION: LEGOLAND 0x00440870
LEGO_EXPORT void BlokeSetFrame(struct Bloke *bloke, int frame) {
    struct Person *person;
    struct Anim3D *anim;

    person = bloke->person;
    if (person != 0) {
        anim = GetBlokeAnim3D(bloke);
        person->frame = frame % anim->divisor;
    }
}

// FUNCTION: LEGOLAND 0x004408a0
LEGO_EXPORT int PlayBlokeAnim(struct Bloke *bloke) {
    struct Person *person;
    struct Anim3D *anim;
    int frame;

    person = bloke->person;
    if (person != 0) {
        anim = GetBlokeAnim3DFromPerson(person);
        frame = person->frame + 1;
        person->frame = frame;
        if (anim->divisor <= frame) {
            person->frame = 0;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x004408e0
LEGO_EXPORT void BlokeAnimNextFrame(struct Bloke *bloke) {
    struct Person *person;
    struct Anim3D *anim;

    person = bloke->person;
    if (person != 0) {
        anim = GetBlokeAnim3D(bloke);
        person->frame = (person->frame + 1) % anim->divisor;
    }
}

// FUNCTION: LEGOLAND 0x00440910
LEGO_EXPORT void BlokeWalkAnim(struct Bloke *bloke) {
    int anim;

    switch (bloke->person->character) {
    case 2:
        anim = 0;
        break;
    case 3:
        anim = 0;
        break;
    default:
        anim = 1;
        break;
    }
    BlokeSetAnim(bloke, anim);
    BlokeSetFrame(bloke, 0);
}

// FUNCTION: LEGOLAND 0x00440960
LEGO_EXPORT void BlokeWalkWithPan(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 5);
}

// FUNCTION: LEGOLAND 0x00440970
LEGO_EXPORT void BlokePanWithPan(struct Bloke *bloke) {
    BlokeSetAnim(bloke, 4);
}

struct IntVec3 {
    int x;
    int y;
    int z;
};

// FUNCTION: LEGOLAND 0x00440980
void ComputeMeshElemBounds(struct MeshElem *elem, struct IntVec3 *out) {
    int n;
    int *verts;
    struct IntVec3 mn;
    struct IntVec3 mx;
    int vx;
    int vy;
    int vz;

    verts = (int *)elem->verts;
    mn.x = verts[0];
    mn.y = verts[1];
    mn.z = verts[2];
    mx = mn;
    verts = verts + 3;
    n = elem->vert_count;
    if (n > 1) {
        n = n - 1;
        do {
            vx = verts[0];
            vy = verts[1];
            vz = verts[2];
            if (vx < mn.x) {
                mn.x = vx;
            }
            if (vx > mx.x) {
                mx.x = vx;
            }
            if (vy < mn.y) {
                mn.y = vy;
            }
            if (vy > mx.y) {
                mx.y = vy;
            }
            if (vz < mn.z) {
                mn.z = vz;
            }
            if (vz > mx.z) {
                mx.z = vz;
            }
            verts = verts + 3;
        } while (--n != 0);
    }
    out[0] = mn;
    out[1] = mx;
}

/* Draws the person's animated 3D mesh into the current 3D target, which Render3DPerson sets up first: gets the
 * animation pose, resets the Z buffer, transforms the vertices and rasterises each face with its colour ramp (flag
 * 0x2000) or texture. */
// Hand-written assembly in the original (MSVC6 never emits it from C); transcribed as naked __asm.
// FUNCTION: LEGOLAND 0x00440a30
__declspec(naked) void DrawPersonMesh(struct Person *person) {
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x1b4
        push ebx
        push esi
        push edi
        mov edi, dword ptr [ebp + 8]
        push edi
        call GetBlokeAnim3DFromPerson
        fld dword ptr [edi + 0x10]
        fmul dword ptr [FLOAT_004ab4b8]
        mov ecx, dword ptr [edi + 0x4c]
        mov eax, dword ptr [eax + 4]
        add esp, 4
        mov dword ptr [ebp - 0x1c], 0x47800000
        lea edx, [ecx*8]
        fstp dword ptr [ebp - 0x2c]
        fld dword ptr [edi + 0x18]
        fmul dword ptr [FLOAT_004ab4b8]
        sub edx, ecx
        mov ecx, dword ptr [edi + 0x50]
        mov dword ptr [ebp - 0x60], ecx
        lea esi, [eax + edx*8]
        fstp dword ptr [ebp - 0x14]
        fld dword ptr [edi + 0x38]
        mov eax, dword ptr [esi + 0x20]
        mov dword ptr [ebp - 0x1b4], esi
        fst dword ptr [ebp - 0x58]
        fcomp dword ptr [FLOAT_004ab390]
        mov edx, dword ptr [eax]
        mov ecx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [ebp - 0x1b0], edx
        mov edx, dword ptr [esi + 0x18]
        mov dword ptr [ebp - 0x1ac], eax
        fnstsw ax
        mov dword ptr [ebp - 0x1a8], ecx
        mov ecx, dword ptr [esi + 0x28]
        mov dword ptr [ebp - 0x20], edx
        mov edx, dword ptr [edi + 0x14]
        test ah, 0x40
        mov dword ptr [ebp - 0xe4], ecx
        mov dword ptr [ebp - 0x10], edx
        jne L440ad7
        fld dword ptr [ebp - 0x58]
        fmul dword ptr [ebp - 0x1c]
        fistp dword ptr [ebp - 0x58]
        shl dword ptr [ebp - 0x58], 8
L440ad7:
        mov eax, dword ptr [edi + 0x28]
        mov ecx, dword ptr [edi + 0x24]
        mov edx, dword ptr [edi + 0x2c]
        push eax
        push ecx
        push edx
        call LoadZBufferFromSprite
        mov ecx, dword ptr [edi + 0x64]
        mov edx, dword ptr [edi + 0x70]
        lea eax, [edi + 0x58]
        mov dword ptr [ebp - 0x50], ecx
        mov ecx, dword ptr [edi + 0x68]
        mov dword ptr [ebp - 0x5c], eax
        mov dword ptr [ebp - 0xdc], eax
        mov eax, dword ptr [eax]
        mov dword ptr [ebp - 0x54], eax
        mov eax, dword ptr [edi + 0x5c]
        mov dword ptr [ebp - 0x4c], edx
        mov edx, dword ptr [edi + 0x74]
        mov dword ptr [ebp - 0x48], eax
        mov eax, dword ptr [edi + 0x60]
        mov dword ptr [ebp - 0x44], ecx
        mov ecx, dword ptr [edi + 0x6c]
        mov dword ptr [ebp - 0x40], edx
        mov edx, dword ptr [edi + 0x78]
        mov dword ptr [ebp - 0xc], 0xffffe800
        mov dword ptr [ebp - 8], 0xffffb000
        mov dword ptr [ebp - 4], 0x3000
        mov dword ptr [ebp - 0x3c], eax
        mov dword ptr [ebp - 0x38], ecx
        mov dword ptr [ebp - 0x34], edx
        fld dword ptr [ebp - 0x2c]
        fmul dword ptr [ebp - 0x1c]
        fistp dword ptr [ebp - 0x2c]
        fld dword ptr [ebp - 0x10]
        fmul dword ptr [ebp - 0x1c]
        fistp dword ptr [ebp - 0x10]
        fld dword ptr [ebp - 0x14]
        fmul dword ptr [ebp - 0x1c]
        fistp dword ptr [ebp - 0x14]
        mov edi, dword ptr [esi + 0xc]
        mov ebx, dword ptr [esi]
        mov eax, dword ptr [esi + 0x14]
        add edi, ebx
        mov ebx, dword ptr [esi + 8]
        add ebx, eax
        mov eax, esi
        neg edi
        mov ecx, dword ptr [eax]
        mov dword ptr [ebp - 0xd8], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [ebp - 0xd4], edx
        mov eax, dword ptr [eax + 8]
        mov dword ptr [ebp - 0xd0], eax
        mov ecx, dword ptr [esi]
        mov dword ptr [ebp - 0xcc], ecx
        mov edx, dword ptr [esi + 4]
        mov dword ptr [ebp - 0xc8], edx
        mov eax, dword ptr [esi + 0x14]
        mov dword ptr [ebp - 0xc4], eax
        mov ecx, dword ptr [esi]
        mov dword ptr [ebp - 0xc0], ecx
        mov edx, dword ptr [esi + 0x10]
        mov dword ptr [ebp - 0xbc], edx
        mov eax, dword ptr [esi + 8]
        mov dword ptr [ebp - 0xb8], eax
        mov ecx, dword ptr [esi]
        mov dword ptr [ebp - 0xb4], ecx
        mov edx, dword ptr [esi + 0x10]
        mov dword ptr [ebp - 0xb0], edx
        mov eax, dword ptr [esi + 0x14]
        mov dword ptr [ebp - 0xac], eax
        mov ecx, dword ptr [esi + 0xc]
        mov dword ptr [ebp - 0xa8], ecx
        mov edx, dword ptr [esi + 4]
        mov dword ptr [ebp - 0xa4], edx
        mov eax, dword ptr [esi + 8]
        mov dword ptr [ebp - 0xa0], eax
        mov ecx, dword ptr [esi + 0xc]
        mov dword ptr [ebp - 0x9c], ecx
        mov edx, dword ptr [esi + 4]
        mov dword ptr [ebp - 0x98], edx
        mov eax, dword ptr [esi + 0x14]
        neg ebx
        sar edi, 1
        sar ebx, 1
        mov dword ptr [ebp - 0x94], eax
        mov ecx, dword ptr [esi]
        push 1
        mov dword ptr [ebp - 0x90], ecx
        mov edx, dword ptr [esi + 0x10]
        mov dword ptr [ebp - 0x8c], edx
        mov eax, dword ptr [esi + 0x14]
        mov dword ptr [ebp - 0x88], eax
        mov ecx, dword ptr [esi + 0xc]
        mov dword ptr [ebp - 0x84], ecx
        mov edx, dword ptr [esi + 0x10]
        mov dword ptr [ebp - 0x80], edx
        mov eax, dword ptr [esi + 0x14]
        lea ecx, [ebp - 0x54]
        mov dword ptr [ebp - 0x7c], eax
        lea edx, [ebp - 0xc]
        push ecx
        lea eax, [ebp - 0xc]
        push edx
        push eax
        call TransformVectorsL
        mov ecx, dword ptr [ebp - 0x5c]
        push 8
        lea edx, [ebp - 0xd8]
        push ecx
        lea eax, [ebp - 0xd8]
        push edx
        push eax
        call TransformVectorsL
        add esp, 0x2c
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 4]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 4], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 8]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 8], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0xc]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0xc], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x10]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x10], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x14]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x14], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x18]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x18], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x1c]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x1c], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x20]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x20], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x24]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x24], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x28]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x28], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x2c]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x2c], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x30]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x30], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x34]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x34], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x38]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x38], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x3c]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x3c], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x40]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x40], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x44]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x44], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x48]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x48], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x4c]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x4c], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x50]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x50], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x54]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x54], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x58]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x58], eax
        lea eax, [ebp - 0xd8]
        mov eax, dword ptr [eax + 0x5c]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        lea edx, [ebp - 0x150]
        mov dword ptr [edx + 0x5c], eax
        mov eax, dword ptr [ebp - 0x150]
        lea ecx, [ebp - 0x140]
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x18], eax
        mov eax, dword ptr [ebp - 0x14c]
        mov dword ptr [ebp - 0x30], ecx
        mov dword ptr [ebp - 0x28], eax
        mov dword ptr [ebp - 0x5c], 7
L440f19:
        mov edx, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [edx - 4]
        mov edx, dword ptr [ebp - 0x24]
        cmp ecx, edx
        jge L440f29
        mov dword ptr [ebp - 0x24], ecx
L440f29:
        cmp ecx, dword ptr [ebp - 0x18]
        jle L440f31
        mov dword ptr [ebp - 0x18], ecx
L440f31:
        mov ecx, dword ptr [ebp - 0x30]
        mov edx, dword ptr [ebp - 0x28]
        mov ecx, dword ptr [ecx]
        cmp ecx, edx
        jge L440f40
        mov dword ptr [ebp - 0x28], ecx
L440f40:
        cmp ecx, eax
        jle L440f46
        mov eax, ecx
L440f46:
        mov edx, dword ptr [ebp - 0x30]
        mov ecx, dword ptr [ebp - 0x5c]
        add edx, 0xc
        dec ecx
        mov dword ptr [ebp - 0x30], edx
        mov dword ptr [ebp - 0x5c], ecx
        jne L440f19
        mov edx, dword ptr [ebp - 0x18]
        mov ecx, dword ptr [ebp - 0x24]
        sub edx, ecx
        mov ecx, 0x500000
        sar edx, 1
        sub ecx, edx
        mov edx, dword ptr [ebp - 0x28]
        sub eax, edx
        mov dword ptr [ebp - 0x30], ecx
        sar eax, 1
        mov ecx, 0x5a0000
        sub ecx, eax
        mov dword ptr [ebp - 0x18], ecx
        mov ecx, dword ptr [ebp - 0x20]
        test ecx, ecx
        jle L440fba
        xor eax, eax
L440f86:
        mov edx, dword ptr [esi + 0x1c]
        add eax, 0xc
        mov edx, dword ptr [eax + edx - 0xc]
        add edx, edi
        mov dword ptr [eax + PersonMeshVertices], edx
        mov edx, dword ptr [esi + 0x1c]
        mov edx, dword ptr [eax + edx - 8]
        sub edx, dword ptr [esi + 4]
        mov dword ptr [eax + PersonMeshVertices + 0x4], edx
        mov edx, dword ptr [esi + 0x1c]
        mov edx, dword ptr [eax + edx - 4]
        add edx, ebx
        dec ecx
        mov dword ptr [eax + PersonMeshVertices + 0x8], edx
        jne L440f86
L440fba:
        mov ebx, dword ptr [ebp - 0xdc]
        mov edi, dword ptr [ebp - 0x20]
        lea esi, [PersonMeshVertices + 0xc]
L440fc9:
        mov eax, dword ptr [esi]
        imul dword ptr [ebx]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [esi + 4]
        imul dword ptr [ebx + 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [esi + 8]
        imul dword ptr [ebx + 8]
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
        mov dword ptr [esi + 8], ecx
        pop dword ptr [esi + 4]
        pop dword ptr [esi]
        add esi, 0xc
        dec edi
        jne L440fc9
        mov ecx, dword ptr [PersonMeshVertices + 0x14]
        mov edi, dword ptr [ebp - 0x20]
        mov esi, ecx
        test edi, edi
        mov dword ptr [ebp - 0x28], esi
        jle L441071
        mov edx, offset PersonMeshVertices + 0x14
        mov ebx, edi
L44105a:
        mov eax, dword ptr [edx]
        cmp eax, esi
        jge L441065
        mov dword ptr [ebp - 0x28], eax
        mov esi, eax
L441065:
        cmp eax, ecx
        jle L44106b
        mov ecx, eax
L44106b:
        add edx, 0xc
        dec ebx
        jne L44105a
L441071:
        mov eax, 0x40000000
        sub ecx, esi
        cdq
        sar ecx, 5
        idiv ecx
        mov ebx, dword ptr [ebp + 8]
        mov dword ptr [ebp - 0xe0], eax
        mov eax, dword ptr [ebx + 0x3c]
        mov dword ptr [ebp - 0x24], eax
        fld dword ptr [ebp - 0x24]
        fmul dword ptr [ebp - 0x1c]
        fistp dword ptr [ebp - 0x24]
        test edi, edi
        jle L44115d
        mov ecx, dword ptr [ebp - 0x20]
        mov edi, offset PersonMeshVertexZ
        mov esi, offset PersonMeshVertices + 0x14
        mov dword ptr [ebp - 0x5c], ecx
L4410ae:
        mov eax, dword ptr [esi]
        mov ecx, dword ptr [ebp - 0x28]
        lea edx, [esi - 8]
        sub eax, ecx
        mov dword ptr [ebp - 0x1c], edx
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0xe0]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp + 8], eax
        mov eax, dword ptr [ebx + 0x2c]
        test eax, eax
        je L4410e6
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x24]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp + 8], eax
L4410e6:
        mov ecx, dword ptr [esi - 4]
        mov dword ptr [ebp - 0x20], ecx
        mov eax, dword ptr [ebp - 0x20]
        mov ecx, dword ptr [ebp - 0x58]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x20], eax
        mov edx, dword ptr [ebx + 0x34]
        mov ecx, dword ptr [ebp + 8]
        mov eax, dword ptr [ebp - 0x20]
        shl edx, 0x18
        add edx, ecx
        add edx, eax
        mov dword ptr [edi], edx
        mov eax, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [ebp - 0x2c]
        imul ecx
        shrd eax, edx, 0x10
        mov edx, dword ptr [ebp - 0x1c]
        mov dword ptr [edx], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [eax + 4]
        mov ecx, dword ptr [ebp - 0x10]
        imul ecx
        shrd eax, edx, 0x10
        mov edx, dword ptr [ebp - 0x1c]
        mov dword ptr [edx + 4], eax
        mov eax, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [eax + 8]
        mov ecx, dword ptr [ebp - 0x14]
        imul ecx
        shrd eax, edx, 0x10
        mov edx, dword ptr [ebp - 0x1c]
        mov dword ptr [edx + 8], eax
        mov eax, dword ptr [ebp - 0x5c]
        add esi, 0xc
        add edi, 4
        dec eax
        mov dword ptr [ebp - 0x5c], eax
        jne L4410ae
L44115d:
        mov eax, dword ptr [ebp - 0xdc]
        push eax
        call TMNegParity
        mov ecx, dword ptr [ebp - 0x1a8]
        mov dword ptr [ebp - 0xe0], eax
        add esp, 4
        xor eax, eax
        test ecx, ecx
        mov dword ptr [ebp + 8], eax
        jle L4414cd
L441185:
        mov edx, dword ptr [ebp - 0x1ac]
        lea ecx, [eax + eax*2]
        mov eax, dword ptr [edx + ecx*4]
        lea edx, [edx + ecx*4]
        mov dword ptr [ebp - 0x10], edx
        lea eax, [eax + eax*2]
        lea ecx, [eax*4 + PersonMeshVertices + 0xc]
        mov esi, dword ptr [eax*4 + PersonMeshVertices + 0xc]
        mov eax, dword ptr [ecx + 4]
        mov edi, dword ptr [ecx + 8]
        mov ecx, dword ptr [edx + 4]
        mov edx, dword ptr [edx + 8]
        mov dword ptr [ebp - 0xe8], edi
        lea ecx, [ecx + ecx*2]
        lea edx, [edx + edx*2]
        lea ecx, [ecx*4 + PersonMeshVertices + 0xc]
        mov ebx, ecx
        lea edx, [edx*4 + PersonMeshVertices + 0xc]
        mov ecx, dword ptr [ebx]
        mov dword ptr [ebp - 0x78], ecx
        mov ecx, dword ptr [ebx + 4]
        mov ebx, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x70], ebx
        mov ebx, edx
        mov edx, dword ptr [ebx]
        mov dword ptr [ebp - 0x6c], edx
        mov edx, dword ptr [ebx + 4]
        mov ebx, dword ptr [ebx + 8]
        mov dword ptr [ebp - 0x64], ebx
        mov ebx, dword ptr [ebp - 0xe0]
        test ebx, ebx
        jne L441271
        add edi, esi
        sub eax, esi
        mov esi, dword ptr [ebp - 0xe8]
        mov ebx, dword ptr [ebp - 0x30]
        add eax, esi
        mov esi, dword ptr [ebp - 0x18]
        add eax, esi
        mov esi, dword ptr [ebp - 0x70]
        mov dword ptr [ebp - 0x168], eax
        mov eax, dword ptr [ebp - 0x78]
        add esi, eax
        lea edi, [ebx + edi*2]
        sub ecx, eax
        mov eax, dword ptr [ebp - 0x18]
        lea esi, [ebx + esi*2]
        mov ebx, dword ptr [ebp - 0x70]
        add ecx, ebx
        mov ebx, dword ptr [ebp - 0x64]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x6c]
        add ebx, eax
        mov eax, dword ptr [ebp - 0x30]
        mov dword ptr [ebp - 0x16c], edi
        mov dword ptr [ebp - 0x188], esi
        lea eax, [eax + ebx*2]
        mov ebx, dword ptr [ebp - 0x6c]
        sub edx, ebx
        mov ebx, dword ptr [ebp - 0x64]
        add edx, ebx
        mov ebx, dword ptr [ebp - 0x18]
        add edx, ebx
        mov dword ptr [ebp - 0x184], ecx
        mov ebx, edx
        mov edx, dword ptr [ebp - 0x168]
        mov dword ptr [ebp - 0x1a4], eax
        mov dword ptr [ebp - 0x1a0], ebx
        jmp L4412e4
L441271:
        lea ebx, [edi + esi]
        mov edi, dword ptr [ebp - 0x30]
        sub eax, esi
        mov esi, dword ptr [ebp - 0x70]
        lea ebx, [edi + ebx*2]
        mov dword ptr [ebp - 0x1a4], ebx
        mov ebx, dword ptr [ebp - 0xe8]
        add eax, ebx
        mov ebx, dword ptr [ebp - 0x18]
        add eax, ebx
        mov dword ptr [ebp - 0x1a0], eax
        mov eax, dword ptr [ebp - 0x78]
        add esi, eax
        sub ecx, eax
        mov eax, dword ptr [ebp - 0x70]
        add ecx, eax
        mov eax, dword ptr [ebp - 0x6c]
        add ecx, ebx
        mov ebx, dword ptr [ebp - 0x64]
        add ebx, eax
        lea esi, [edi + esi*2]
        sub edx, eax
        mov eax, dword ptr [ebp - 0x18]
        lea edi, [edi + ebx*2]
        mov ebx, dword ptr [ebp - 0x64]
        add edx, ebx
        mov ebx, dword ptr [ebp - 0x1a0]
        add edx, eax
        mov eax, dword ptr [ebp - 0x1a4]
        mov dword ptr [ebp - 0x188], esi
        mov dword ptr [ebp - 0x184], ecx
        mov dword ptr [ebp - 0x16c], edi
        mov dword ptr [ebp - 0x168], edx
L4412e4:
        sub esi, eax
        sub ecx, ebx
        sub edi, eax
        sub edx, ebx
        mov dword ptr [ebp - 0x24], esi
        mov dword ptr [ebp - 0x20], ecx
        mov dword ptr [ebp - 0x58], edi
        mov dword ptr [ebp - 0x28], edx
        mov eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x58]
        mov ecx, dword ptr [ebp - 0x20]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x14], eax
        mov ecx, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [ebp - 0x14]
        sub ecx, eax
        jns L4414af
        mov eax, dword ptr [ebp - 0x10]
        mov edx, dword ptr [eax]
        mov ecx, dword ptr [edx*4 + PersonMeshVertexZ]
        mov edx, dword ptr [eax + 4]
        mov dword ptr [ebp - 0x19c], ecx
        mov ecx, dword ptr [edx*4 + PersonMeshVertexZ]
        mov edx, dword ptr [eax + 8]
        mov dword ptr [ebp - 0x180], ecx
        mov eax, dword ptr [edx*4 + PersonMeshVertexZ]
        mov dword ptr [ebp - 0x164], eax
        mov ecx, dword ptr [ebp + 8]
        lea ebx, [ecx + ecx*8]
        lea ebx, [ebx*4]
        mov ecx, dword ptr [ebp - 0x1b4]
        add ebx, dword ptr [ecx + 0x28]
        mov eax, dword ptr [ebx]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 4]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 8]
        imul dword ptr [ebp - 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, ecx
        sar ecx, 0x1f
        shr eax, cl
        mov ecx, eax
        add ecx, 0x3333
        lea edx, [ebp - 0x1a4]
        mov dword ptr [edx + 0x14], ecx
        mov eax, dword ptr [ebx + 0xc]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 0x10]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 0x14]
        imul dword ptr [ebp - 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, ecx
        sar ecx, 0x1f
        shr eax, cl
        mov ecx, eax
        add ecx, 0x3333
        lea edx, [ebp - 0x188]
        mov dword ptr [edx + 0x14], ecx
        mov eax, dword ptr [ebx + 0x18]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 0x1c]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 0x20]
        imul dword ptr [ebp - 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, ecx
        sar ecx, 0x1f
        shr eax, cl
        mov ecx, eax
        add ecx, 0x3333
        lea edx, [ebp - 0x16c]
        mov dword ptr [edx + 0x14], ecx
        mov esi, dword ptr [ebp - 0x60]
        mov eax, dword ptr [esi]
        test ah, 0x20
        je L441451
        mov ecx, dword ptr [esi + 8]
        push ecx
        call SetCurrentColorRamp
        lea edx, [ebp - 0x16c]
        lea eax, [ebp - 0x188]
        push edx
        lea ecx, [ebp - 0x1a4]
        push eax
        push ecx
        call DrawGouraudRampTriangle
        add esp, 0x10
        jmp L4414b2
L441451:
        mov edx, dword ptr [esi + 0xc]
        mov eax, dword ptr [esi + 0x10]
        mov ecx, dword ptr [esi + 0x14]
        mov dword ptr [ebp - 0x198], edx
        mov edx, dword ptr [esi + 0x18]
        mov dword ptr [ebp - 0x194], eax
        mov eax, dword ptr [esi + 0x1c]
        mov dword ptr [ebp - 0x178], edx
        mov edx, dword ptr [esi + 8]
        mov dword ptr [ebp - 0x17c], ecx
        mov ecx, dword ptr [esi + 0x20]
        push edx
        mov dword ptr [ebp - 0x160], eax
        mov dword ptr [ebp - 0x15c], ecx
        call SetCurrentTextureByIndex
        lea eax, [ebp - 0x16c]
        lea ecx, [ebp - 0x188]
        push eax
        lea edx, [ebp - 0x1a4]
        push ecx
        push edx
        call DrawGouraudTexturedTriangle
        add esp, 0x10
        jmp L4414b2
L4414af:
        mov esi, dword ptr [ebp - 0x60]
L4414b2:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x1a8]
        add esi, 0x24
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 0x60], esi
        mov dword ptr [ebp + 8], eax
        jl L441185
L4414cd:
        mov edx, dword ptr [ebp - 0xe4]
        lea ecx, [eax + eax*2]
        lea ecx, [edx + ecx*8]
        mov dword ptr [ebp - 0xe4], ecx
        mov ecx, dword ptr [ebp - 0x1b0]
        cmp eax, ecx
        jge L4417ec
        jmp L4414f2
L4414ef:
        mov eax, dword ptr [ebp + 8]
L4414f2:
        lea edx, [eax + eax*2]
        mov eax, dword ptr [ebp - 0x1ac]
        lea ecx, [eax + edx*4]
        mov dword ptr [ebp - 0x10], ecx
        mov eax, dword ptr [ecx]
        lea edx, [eax + eax*2]
        lea eax, [edx*4 + PersonMeshVertices + 0xc]
        mov edx, dword ptr [edx*4 + PersonMeshVertices + 0xc]
        mov dword ptr [ebp - 0x78], edx
        mov esi, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [ebp - 0x70], eax
        mov eax, dword ptr [ecx + 4]
        lea edx, [eax + eax*2]
        lea eax, [edx*4 + PersonMeshVertices + 0xc]
        mov edx, dword ptr [edx*4 + PersonMeshVertices + 0xc]
        mov dword ptr [ebp - 0xf0], edx
        mov ebx, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 8]
        mov dword ptr [ebp - 0xe8], eax
        mov eax, dword ptr [ecx + 8]
        lea ecx, [eax + eax*2]
        lea edx, [ecx*4 + PersonMeshVertices + 0xc]
        mov eax, dword ptr [ecx*4 + PersonMeshVertices + 0xc]
        mov dword ptr [ebp - 0x6c], eax
        mov edi, dword ptr [edx + 4]
        mov ecx, dword ptr [edx + 8]
        mov edx, dword ptr [ebp - 0xdc]
        push edx
        mov dword ptr [ebp - 0x64], ecx
        call TMNegParity
        add esp, 4
        test eax, eax
        jne L441603
        mov edx, dword ptr [ebp - 0x78]
        mov eax, dword ptr [ebp - 0x70]
        sub esi, edx
        lea ecx, [eax + edx]
        mov edx, dword ptr [ebp - 0x70]
        mov eax, dword ptr [ebp - 0x30]
        add esi, edx
        mov edx, dword ptr [ebp - 0x18]
        add esi, edx
        mov edx, dword ptr [ebp - 0xf0]
        mov dword ptr [ebp - 0x168], esi
        mov esi, dword ptr [ebp - 0xe8]
        add esi, edx
        lea ecx, [eax + ecx*2]
        sub ebx, edx
        mov edx, dword ptr [ebp - 0x18]
        lea eax, [eax + esi*2]
        mov esi, dword ptr [ebp - 0xe8]
        add ebx, esi
        mov esi, dword ptr [ebp - 0x64]
        add ebx, edx
        mov edx, dword ptr [ebp - 0x6c]
        add esi, edx
        mov edx, dword ptr [ebp - 0x30]
        mov dword ptr [ebp - 0x16c], ecx
        mov dword ptr [ebp - 0x188], eax
        lea edx, [edx + esi*2]
        mov esi, dword ptr [ebp - 0x6c]
        sub edi, esi
        mov esi, dword ptr [ebp - 0x64]
        add edi, esi
        mov esi, dword ptr [ebp - 0x18]
        add edi, esi
        mov dword ptr [ebp - 0x184], ebx
        mov esi, edi
        mov edi, dword ptr [ebp - 0x168]
        mov dword ptr [ebp - 0x1a4], edx
        mov dword ptr [ebp - 0x1a0], esi
        jmp L441682
L441603:
        mov eax, dword ptr [ebp - 0x78]
        mov ecx, dword ptr [ebp - 0x70]
        sub esi, eax
        lea edx, [ecx + eax]
        mov ecx, dword ptr [ebp - 0x30]
        mov eax, dword ptr [ebp - 0xe8]
        lea edx, [ecx + edx*2]
        mov dword ptr [ebp - 0x1a4], edx
        mov edx, dword ptr [ebp - 0x70]
        add esi, edx
        mov edx, dword ptr [ebp - 0x18]
        add esi, edx
        mov dword ptr [ebp - 0x1a0], esi
        mov esi, dword ptr [ebp - 0xf0]
        add eax, esi
        sub ebx, esi
        mov esi, dword ptr [ebp - 0xe8]
        add ebx, esi
        mov esi, dword ptr [ebp - 0x64]
        add ebx, edx
        mov edx, dword ptr [ebp - 0x6c]
        add esi, edx
        lea eax, [ecx + eax*2]
        sub edi, edx
        mov edx, dword ptr [ebp - 0x18]
        lea ecx, [ecx + esi*2]
        mov esi, dword ptr [ebp - 0x64]
        add edi, esi
        mov esi, dword ptr [ebp - 0x1a0]
        add edi, edx
        mov edx, dword ptr [ebp - 0x1a4]
        mov dword ptr [ebp - 0x188], eax
        mov dword ptr [ebp - 0x184], ebx
        mov dword ptr [ebp - 0x16c], ecx
        mov dword ptr [ebp - 0x168], edi
L441682:
        sub eax, edx
        sub ebx, esi
        sub ecx, edx
        sub edi, esi
        mov dword ptr [ebp - 0x24], eax
        mov dword ptr [ebp - 0x20], ebx
        mov dword ptr [ebp - 0x58], ecx
        mov dword ptr [ebp - 0x28], edi
        mov eax, dword ptr [ebp - 0x24]
        mov ecx, dword ptr [ebp - 0x28]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x1c], eax
        mov eax, dword ptr [ebp - 0x58]
        mov ecx, dword ptr [ebp - 0x20]
        imul ecx
        shrd eax, edx, 0x10
        mov dword ptr [ebp - 0x14], eax
        mov ecx, dword ptr [ebp - 0x1c]
        mov eax, dword ptr [ebp - 0x14]
        sub ecx, eax
        jns L4417ce
        mov eax, dword ptr [ebp - 0x10]
        mov edx, dword ptr [eax]
        mov ecx, dword ptr [edx*4 + PersonMeshVertexZ]
        mov edx, dword ptr [eax + 4]
        mov dword ptr [ebp - 0x19c], ecx
        mov ecx, dword ptr [edx*4 + PersonMeshVertexZ]
        mov edx, dword ptr [eax + 8]
        mov dword ptr [ebp - 0x180], ecx
        mov eax, dword ptr [edx*4 + PersonMeshVertexZ]
        mov dword ptr [ebp - 0x164], eax
        mov ecx, dword ptr [ebp + 8]
        lea ecx, [ecx + ecx*2]
        mov ebx, dword ptr [ebp - 0xe4]
        lea ebx, [ebx + ecx*4]
        mov eax, dword ptr [ebx]
        imul dword ptr [ebp - 0xc]
        shrd eax, edx, 0x10
        mov ecx, eax
        mov eax, dword ptr [ebx + 4]
        imul dword ptr [ebp - 8]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, dword ptr [ebx + 8]
        imul dword ptr [ebp - 4]
        shrd eax, edx, 0x10
        add ecx, eax
        mov eax, ecx
        sar ecx, 0x1f
        shr eax, cl
        mov ecx, eax
        add ecx, 0x3333
        lea edx, [ebp - 0x1a4]
        mov dword ptr [edx + 0x14], ecx
        mov esi, dword ptr [ebp - 0x60]
        mov eax, dword ptr [esi]
        test ah, 0x20
        je L441770
        mov ecx, dword ptr [esi + 8]
        push ecx
        call SetCurrentColorRamp
        lea edx, [ebp - 0x16c]
        lea eax, [ebp - 0x188]
        push edx
        lea ecx, [ebp - 0x1a4]
        push eax
        push ecx
        call DrawFlatRampTriangle
        add esp, 0x10
        jmp L4417d1
L441770:
        mov edx, dword ptr [esi + 0xc]
        mov eax, dword ptr [esi + 0x10]
        mov ecx, dword ptr [esi + 0x14]
        mov dword ptr [ebp - 0x198], edx
        mov edx, dword ptr [esi + 0x18]
        mov dword ptr [ebp - 0x194], eax
        mov eax, dword ptr [esi + 0x1c]
        mov dword ptr [ebp - 0x178], edx
        mov edx, dword ptr [esi + 8]
        mov dword ptr [ebp - 0x17c], ecx
        mov ecx, dword ptr [esi + 0x20]
        push edx
        mov dword ptr [ebp - 0x160], eax
        mov dword ptr [ebp - 0x15c], ecx
        call SetCurrentTextureByIndex
        lea eax, [ebp - 0x16c]
        lea ecx, [ebp - 0x188]
        push eax
        lea edx, [ebp - 0x1a4]
        push ecx
        push edx
        call DrawFlatTexturedTriangle
        add esp, 0x10
        jmp L4417d1
L4417ce:
        mov esi, dword ptr [ebp - 0x60]
L4417d1:
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [ebp - 0x1b0]
        add esi, 0x24
        inc eax
        cmp eax, ecx
        mov dword ptr [ebp - 0x60], esi
        mov dword ptr [ebp + 8], eax
        jl L4414ef
L4417ec:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}
