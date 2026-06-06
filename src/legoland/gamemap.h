#pragma once

// Opaque types; full definitions live in gamemap.c (only its bodies need them).
struct RenderObject;
struct RenderObjectVtable;

// The map edit cursor. EditCursor / QueryCursor / DAT_0082f760 are instances of
// this 0x1834-byte structure. Only the fields actually touched are named; the
// gaps are explicit padding so the layout matches the original byte-for-byte.
struct Cursor {
    unsigned char pad_0[0x1404];
    unsigned int field_1404;
    unsigned int field_1408;
    int field_140c;
    int field_1410;
    unsigned int field_1414[5];
    unsigned char pad_1428[0x1828 - 0x1428];
    unsigned int field_1828;
    unsigned char pad_182c[0x1830 - 0x182c];
    unsigned int field_1830;
};

void FUN_00459960(void);
void FUN_00459970(void);
int FindObjectsPower(void *object);
void FUN_0045a060(void);
void DefaultCursor(struct Cursor *cursor);
void CalculateMapRenderOrder(void);
struct RenderObject *GetFirstRenderObject(void);
struct RenderObject *GetNextRenderObject(struct RenderObject *object);
struct RenderObject *GetFirstObjectMatching(struct RenderObjectVtable *vtable);
