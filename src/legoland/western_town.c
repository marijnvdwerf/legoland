#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "gamemap.h"
#include "llidb.h"
#include "man3d.h"
#include "map_object.h"
#include "money.h"
#include "obj_instance.h"
#include "print_sprite.h"
#include "render3d.h"

struct RideListElem {
    struct RideListElem *next;
    unsigned char pad_4[0x8 - 0x4];
    struct Bloke *bloke;
    unsigned short id;
};

struct Building {
    unsigned char pad_0[0x1c];
    unsigned int flags;
    unsigned char pad_20[0x64 - 0x20];
    void *layer;
    unsigned char pad_68[0xcc - 0x68];
    struct RideListElem *list;
};

struct MapObject {
    unsigned char pad_0[0xc];
    struct Building *building;
};

struct JailCell {
    struct JailCell *next;
    unsigned short field_4;
    unsigned char field_6;
    unsigned char pad_7[0x8 - 0x7];
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
};

#include "image_sprite.h"

// FUNCTION: LEGOLAND 0x004375d0
void FUN_004375d0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb30 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74a4
    DAT_0081cb08 = LoadSprite("G_Store Matte.LLS", 1);
    // STRING: LEGOLAND 0x004b7490
    DAT_0081cb24 = LoadSprite("G_Store Matte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437610
void FUN_00437610(void) {
    KillSprite(DAT_0081cb08);
    KillSprite(DAT_0081cb24);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437630
void FUN_00437630(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb30;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00437670
void FUN_00437670(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6) {
    struct Building *ride = param_1->building;
    struct RideListElem *elem = ride->list;
    char count = 0;
    int array[10];
    int *p;
    int n;
    unsigned short id;
    union { __int64 q; int i[2]; } coords;

    {
        int *fill = array;
        int z;
        array[0] = 0;
        for (z = 9; fill = fill + 1, z != 0; z--) {
            *fill = 0;
        }
    }
    if (elem != NULL) {
        id = *param_4;
        do {
            if (id == elem->id) {
                array[(int)count] = (int)elem->bloke;
                count++;
            }
            elem = elem->next;
        } while (elem != NULL);
        if (count != 0) {
            coords.q = GetScreenCoordsForObject((unsigned char *)param_4, ride);
            if (0 < count) {
                p = array;
                n = count;
                do {
                    if (*(char *)(*p + 0x60) == 4) {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p = p + 1;
                    n = n - 1;
                } while (n != 0);
                if (0 < count) {
                    p = array;
                    n = count;
                    do {
                        if (*(char *)(*p + 0x60) == 5) {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p = p + 1;
                        n = n - 1;
                    } while (n != 0);
                    if (0 < count) {
                        p = array;
                        n = count;
                        do {
                            if (*(char *)(*p + 0x60) == 6) {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p = p + 1;
                            n = n - 1;
                        } while (n != 0);
                    }
                }
            }
            PrintSprite(DAT_0081cb24, coords.i[0], coords.i[1], param_6, NULL);
            if (0 < count) {
                p = array;
                n = count;
                do {
                    if (*(char *)(*p + 0x60) == 0) {
                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                    }
                    p = p + 1;
                    n = n - 1;
                } while (n != 0);
                if (0 < count) {
                    p = array;
                    n = count;
                    do {
                        if (*(char *)(*p + 0x60) == 1) {
                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                        }
                        p = p + 1;
                        n = n - 1;
                    } while (n != 0);
                    if (0 < count) {
                        p = array;
                        n = count;
                        do {
                            if (*(char *)(*p + 0x60) == 2) {
                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                            }
                            p = p + 1;
                            n = n - 1;
                        } while (n != 0);
                        if (0 < count) {
                            p = array;
                            n = count;
                            do {
                                if (*(char *)(*p + 0x60) == 3) {
                                    IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                }
                                p = p + 1;
                                n = n - 1;
                            } while (n != 0);
                            if (0 < count) {
                                p = array;
                                n = count;
                                do {
                                    if (*(char *)(*p + 0x60) == 7) {
                                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                    }
                                    p = p + 1;
                                    n = n - 1;
                                } while (n != 0);
                                if (0 < count) {
                                    p = array;
                                    n = count;
                                    do {
                                        if (*(char *)(*p + 0x60) == 8) {
                                            IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                        }
                                        p = p + 1;
                                        n = n - 1;
                                    } while (n != 0);
                                    if (0 < count) {
                                        p = array;
                                        n = count;
                                        do {
                                            if (*(char *)(*p + 0x60) == 9) {
                                                IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                            }
                                            p = p + 1;
                                            n = n - 1;
                                        } while (n != 0);
                                        if (0 < count) {
                                            n = count;
                                            p = array;
                                            do {
                                                if (*(char *)(*p + 0x60) == 10) {
                                                    IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                                }
                                                p = p + 1;
                                                n = n - 1;
                                            } while (n != 0);
                                            if (0 < count) {
                                                n = count;
                                                p = array;
                                                do {
                                                    if (*(char *)(*p + 0x60) == 11) {
                                                        IP_RenderBlokeIn3DNow((struct Bloke *)*p);
                                                    }
                                                    p = p + 1;
                                                    n = n - 1;
                                                } while (n != 0);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            PrintSprite(DAT_0081cb08, coords.i[0], coords.i[1], param_6, NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x004378e0
void FUN_004378e0(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437ba0
void FUN_00437ba0(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb14 = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74b8
    DAT_0081cb38 = LoadSprite("Sherifshut Matte.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bd0
void FUN_00437bd0(void) {
    KillSprite(DAT_0081cb38);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00437bf0
void FUN_00437bf0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb14;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00437c30
void FUN_00437c30(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437c90
void FUN_00437c90(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00437f10
void FUN_00437f10(unsigned short *param) {
    struct JailCell *cell = malloc(sizeof(struct JailCell));
    if (cell != NULL) {
        memset(cell, 0, sizeof(struct JailCell));
        cell->field_4 = *param;
        cell->field_6 = 9;
        cell->field_8 = 0;
        cell->field_c = 0;
        cell->field_10 = 0;
        cell->field_14 = 0;
        cell->field_18 = 0;
        cell->next = DAT_0062fd3c;
        DAT_0062fd3c = cell;
    }
}

// FUNCTION: LEGOLAND 0x00437f60
void FUN_00437f60(struct EditObject *editObj, int *coords) {
    unsigned char key[2];
    key[0] = (unsigned char)coords[0];
    key[1] = (unsigned char)coords[1];
    AddBasicObject(editObj, coords);
    FUN_00437f10((unsigned short *)key);
}

// FUNCTION: LEGOLAND 0x00437f90
struct JailCell *FUN_00437f90(unsigned short *key) {
    struct JailCell *cell = DAT_0062fd3c;
    while (cell != NULL) {
        if (cell->field_4 == *key) {
            return cell;
        }
        cell = cell->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00437fc0
void FUN_00437fc0(struct JailCell *cell) {
    struct JailCell *prev = DAT_0062fd3c;
    if (prev == cell) {
        DAT_0062fd3c = cell->next;
        free(cell);
        return;
    }
    if (prev->next != cell) {
        prev = prev->next;
        while (prev != NULL && prev->next != cell) {
            prev = prev->next;
        }
    }
    if (prev != NULL) {
        prev->next = cell->next;
    }
    free(cell);
}

// FUNCTION: LEGOLAND 0x00438000
void FUN_00438000(void) {
    while (DAT_0062fd3c != NULL) {
        FUN_00437fc0(DAT_0062fd3c);
    }
}

// FUNCTION: LEGOLAND 0x00438020
void FUN_00438020(struct MapObject *editObj, unsigned int coords, struct Cursor *cursor) {
    struct JailCell *cell = FUN_00437f90((unsigned short *)&coords);
    if (cell != NULL) {
        FUN_00437fc0(cell);
    }
    StandardRemoveObject((struct EditObject *)editObj, coords, cursor);
    RemoveAllBlokesFromRide((struct Ride *)editObj->building, coords);
}

// FUNCTION: LEGOLAND 0x00438070
void FUN_00438070(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb10 = building;
    building->flags |= 0x420;
    DAT_0062fd40 = DAT_0081cb10->layer;
    DAT_0081cb10->flags |= 0x2000;
    // STRING: LEGOLAND 0x004b74d0
    DAT_0081cb0c = LoadSprite("JailCellMask.LLS", 1);
    HideLayer(DAT_0062fd40, 1);
    StopLayerPlaying((unsigned int)DAT_0062fd40, 1);
    LLSSetFrame((struct LLS *)GetLLSForLayer((unsigned int)DAT_0062fd40, 1), 9);
}

// FUNCTION: LEGOLAND 0x004380f0
void FUN_004380f0(void) {
    KillSprite(DAT_0081cb0c);
    FUN_00438000();
}

// FUNCTION: LEGOLAND 0x00438110
void FUN_00438110(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb10;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00438150
void FUN_00438150(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438430
void FUN_00438430(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438780
LEGO_EXPORT unsigned int SaveJailCells(void) {
    struct JailCell *cell = DAT_0062fd3c;
    unsigned int marker = 1;
    unsigned int terminator = 0;

    while (cell != NULL) {
        if (SaveGameWrite(&marker, 4) == 0) {
            return 0;
        }
        if (SaveGameWrite(cell, 0x1c) == 0) {
            return 0;
        }
        cell = cell->next;
    }
    return SaveGameWrite(&terminator, 4) != 0;
}

// FUNCTION: LEGOLAND 0x004387f0
LEGO_EXPORT unsigned int LoadJailCells(void) {
    unsigned int marker;
    struct JailCell *prev = NULL;

    if (SaveGameRead(&marker, 4) == 0) {
        return 0;
    }
    while (marker != 0) {
        struct JailCell *cell = malloc(0x1c);
        if (SaveGameRead(cell, 0x1c) == 0) {
            return 0;
        }
        cell->next = NULL;
        if (prev != NULL) {
            prev->next = cell;
        } else {
            DAT_0062fd3c = cell;
        }
        prev = cell;
        if (SaveGameRead(&marker, 4) == 0) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x00438870
void FUN_00438870(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb2c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b74e4
    DAT_0081cb34 = LoadSprite("Bank Matte.lls", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388a0
void FUN_004388a0(void) {
    KillSprite(DAT_0081cb34);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x004388c0
void FUN_004388c0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb2c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00438900
void FUN_00438900(struct MapObject *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, unsigned int param_5, unsigned int param_6) {
    struct Building *ride = param_1->building;
    struct RideListElem *elem = ride->list;
    int count = 0;
    union { __int64 q; int i[2]; } coords;
    if (elem != NULL) {
        do {
            if (*param_4 == elem->id) {
                RenderBlokeIn3D(elem->bloke);
                count++;
            }
            elem = elem->next;
        } while (elem != NULL);
        if (count != 0) {
            coords.q = GetScreenCoordsForObject((unsigned char *)param_4, ride);
            PrintSprite(DAT_0081cb34, coords.i[0], coords.i[1], param_6, NULL);
        }
    }
}

// FUNCTION: LEGOLAND 0x00438960
void FUN_00438960(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438c60
void FUN_00438c60(struct MapObject *obj) {
    struct Building *building = obj->building;
    DAT_0081cb1c = building;
    building->flags |= 0x420;
    // STRING: LEGOLAND 0x004b7508
    DAT_0081cb00 = LoadSprite("SaloonMatte1.LLS", 1);
    // STRING: LEGOLAND 0x004b74f4
    DAT_0081cb04 = LoadSprite("SaloonMatte2.LLS", 1);
    LoadMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438ca0
void FUN_00438ca0(void) {
    KillSprite(DAT_0081cb00);
    KillSprite(DAT_0081cb04);
    KillMoneySFX();
}

// FUNCTION: LEGOLAND 0x00438cc0
void FUN_00438cc0(void) {
    EditMode = 1;
    DAT_008119b8 = DAT_0081cb1c;
    DefaultCursor(&EditCursor);
    SetEditCursorFootPrint((char *)DAT_008119b8 + 0x3c);
}

// FUNCTION: LEGOLAND 0x00438d00
void FUN_00438d00(void) { STUB(); }

// FUNCTION: LEGOLAND 0x00438f10
void FUN_00438f10(void) { STUB(); }
