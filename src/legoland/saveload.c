#include "legoland.h"

#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include "bloke.h"
#include "bloke_ai.h"
#include "bricks.h"
#include "challenge.h"
#include "debug_alloc.h"
#include "draw.h"
#include "gamemap.h"
#include "globals.h"
#include "input.h"
#include "interface.h"
#include "llidb.h"
#include "main.h"
#include "man3d.h"
#include "map_object.h"
#include "mapscreen.h"
#include "nerps.h"
#include "obj_instance.h"
#include "pathfind.h"
#include "render3d.h"
#include "saveload.h"
#include "screens.h"
#include "tilemap.h"
#include "timer.h"
#include "worker.h"

/* Abort a load: close the file and report failure. */
#pragma auto_inline(off)

/* A node of OverlayList (the overlays saved at the end of a save game). */
// FUNCTION: LEGOLAND 0x0047d790
LEGO_EXPORT int BeginMeasuredBlock(void) {
    int pos;
    int marker;

    pos = _tell(SaveFileHandle);
    marker = 0;
    if (pos == -1) {
        return 0;
    }
    MeasuredBlockStarts[MeasuredBlockDepth] = pos;
    MeasuredBlockDepth = MeasuredBlockDepth + 1;
    return SaveGameWrite(&marker, 4) != 0;
}

// FUNCTION: LEGOLAND 0x0047d7e0
int SkipSaveGameDword(void) {
    unsigned int buffer;

    return SaveGameRead(&buffer, 4);
}

// FUNCTION: LEGOLAND 0x0047d800
LEGO_EXPORT int EndMeasuredBlock(void) {
    int end_pos;

    end_pos = _tell(SaveFileHandle);
    if (end_pos == -1) {
        return 0;
    }
    MeasuredBlockDepth = MeasuredBlockDepth - 1;
    if (_lseek(SaveFileHandle, MeasuredBlockStarts[MeasuredBlockDepth], 0) == -1) {
        return 0;
    }
    if (SaveGameWrite(&end_pos, 4) == 0) {
        return 0;
    }
    return _lseek(SaveFileHandle, end_pos, 0) != -1;
}

// FUNCTION: LEGOLAND 0x0047d880
LEGO_EXPORT int FindeIneList(union SavedElement *handle) {
    int i;

    for (i = 0; i < SavedElementCount; i++) {
        if (handle->element == SavedElementTable[i]) {
            handle->index = i;
            return 1;
        }
    }
    handle->index = -1;
    return 0;
}

// FUNCTION: LEGOLAND 0x0047d8c0
LEGO_EXPORT struct Element *GeteListPtr(int idx) {
    if (idx == -1) {
        return 0;
    }
    return SavedElementTable[idx];
}

// FUNCTION: LEGOLAND 0x0047d8e0
LEGO_EXPORT int SaveGame(char *filename) {
    // STRING: LEGOLAND 0x004bcb6c
    char header[0x21] = "00002 LEGOLAND Save Game V0.02 \x1a";
    int i;
    int k;
    int x;
    int n_elems;
    int tab_count;
    int len;
    struct MapElement tile;
    struct FXSpriteList *tab[256];

    SaveFileHandle = _open(filename, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, 0x180);
    if (SaveFileHandle == -1) {
        switch (errno) {
        case EACCES:
            // STRING: LEGOLAND 0x004bcb14
            LogPrintf("can't open read-only file for writing, or file\x92s sharing mode forbids operations (%s)\n", filename);
            return 0;
        case EINVAL:
            // STRING: LEGOLAND 0x004bcaec
            LogPrintf("Invalid oflag or pmode argument (%s)\n", filename);
            return 0;
        case EMFILE:
            // STRING: LEGOLAND 0x004bcab0
            LogPrintf("No more file handles available (too many open files) (%s)\n", filename);
            return 0;
        case ENOENT:
            // STRING: LEGOLAND 0x004bca90
            LogPrintf("File or path not found (%s)\n", filename);
            return 0;
        default:
            // STRING: LEGOLAND 0x004bca68
            LogPrintf("Unknown error (%d) openning file %s\n", errno, filename);
            return 0;
        }
    }
    MeasuredBlockDepth = 0;
    if (SaveGameWrite(header, 0x20) == 0) {
        // STRING: LEGOLAND 0x004bca54
        LogPrintf("Header write failed");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bca38
        LogPrintf("Measured block begin failed");
        goto fail;
    }
    {
        struct Element *elem;
        int flags;
        unsigned int *fl;
        n_elems = LLIDB_GetCount();
        SavedElementCount = 0;
        fl = &ElemID(PathControlName)->flags;
        *fl |= 4;
        for (i = 0; i < n_elems; i++) {
            LLIDB_GetElement(i, &elem);
            if ((elem->flags & 4) != 0) {
                SavedElementCount++;
            }
        }
        if (SaveGameWrite(&SavedElementCount, 4) == 0) {
            // STRING: LEGOLAND 0x004bca24
            LogPrintf("Num elements failed");
            goto fail;
        }
        if (SavedElementTable != 0) {
            free(SavedElementTable);
        }
        SavedElementTable = malloc(SavedElementCount * sizeof(*SavedElementTable));
        k = 0;
        for (i = 0; i < n_elems; i++) {
            DrawWatchSprite();
            LLIDB_GetElement(i, &elem);
            if ((elem->flags & 4) != 0) {
                len = strlen(elem->name);
                if (SaveGameWrite(&len, 4) == 0) {
                    goto fail;
                }
                if (SaveGameWrite(elem->name, len) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc9e8
                        LogPrintf("Element name write failed %s", elem->name);
                        goto fail;
                    }
                }
                SavedElementTable[k++] = elem;
                flags = elem->flags & 0x3000e;
                if (SaveGameWrite(&flags, 4) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc9c4
                        LogPrintf("Flags of interest write failed %s", elem->name);
                        goto fail;
                    }
                }
            }
        }
        tab_count = CollectTileSpriteLists(tab);
        if (SaveGameWrite(&tab_count, 4) == 0) {
            // STRING: LEGOLAND 0x004bca08
            LogPrintf("TSF Pointers write failed");
            goto fail;
        }
        for (i = 0; i < tab_count; i++) {
            DrawWatchSprite();
            LLIDB_FindElementFromDataPtr(tab[i], &elem, 0);
            len = strlen(elem->name);
            if (SaveGameWrite(&len, 4) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc980
                    LogPrintf("TSF element length write failed %s, %d", elem->name, len);
                    goto fail;
                }
            }
            if (SaveGameWrite(elem->name, len) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc95c
                    LogPrintf("TSF Element name writer failed %s", elem->name);
                    goto fail;
                }
            }
        }
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc9a8
        LogPrintf("End measured block1 failed");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc93c
        LogPrintf("Begin Measured block2 failed");
        goto fail;
    }
    if (SaveGameWrite(lpConfig, 0x44) == 0) {
        // STRING: LEGOLAND 0x004bc920
        LogPrintf("Host Config write failed");
        goto fail;
    }
    {
        int y;
        for (y = 0; y < lpConfig->height; y++) {
            for (x = 0; x < lpConfig->width; x++) {
                struct FXSpriteList *src;
                tile = GameMap[y][x];
                DrawWatchSprite();
                if ((tile.flags & 0x8a8) != 0) {
                    if (FindeIneList((union SavedElement *)&tile) == 0) {
                        {
                            // STRING: LEGOLAND 0x004bc8d8
                            LogPrintf("Failed to locate Object instance at (%d,%d)", x, y);
                            goto fail;
                        }
                    }
                } else {
                    tile.field_0 = 0;
                }
                src = TileSpriteInfo[tile.field_8].src;
                for (k = 0; k < tab_count && src != tab[k]; k++) {
                }
                if (k != tab_count) {
                    tile.field_8 = (tile.field_8 - *(unsigned short *)src) | ((k + 1) << 8);
                } else {
                    tile.field_8 = 0;
                }
                src = TileSpriteInfo[tile.field_a].src;
                for (k = 0; k < tab_count && src != tab[k]; k++) {
                }
                if (k != tab_count) {
                    tile.field_a = (tile.field_a - *(unsigned short *)src) | ((k + 1) << 8);
                } else {
                    tile.field_a = 0;
                }
                if (SaveGameWrite(&tile, sizeof(struct MapElement)) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc8ac
                        LogPrintf("Failed to write mapinfo struct at (%d,%d)", x, y);
                        goto fail;
                    }
                }
            }
        }
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc904
        LogPrintf("EndMeasured Block2 failed");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc88c
        LogPrintf("Begin Measured block3 failed");
        goto fail;
    }
    if (SaveGameWrite(&MapStats, 0x3f0) == 0) {
        // STRING: LEGOLAND 0x004bc874
        LogPrintf("Mapstats write failed");
        goto fail;
    }
    if (SaveGameWrite(&ScrollX, 4) == 0) {
        // STRING: LEGOLAND 0x004bc860
        LogPrintf("Scrollx (%d) Failed", ScrollX);
        goto fail;
    }
    if (SaveGameWrite(&ScrollY, 4) == 0) {
        // STRING: LEGOLAND 0x004bc84c
        LogPrintf("Scrolly (%d) Failed", ScrollY);
        goto fail;
    }
    if (SaveGameWrite(&EditMode, sizeof(struct EditState)) == 0) {
        // STRING: LEGOLAND 0x004bc83c
        LogPrintf("EditMode Failed");
        goto fail;
    }
    SaveInputState();
    if (SaveScripts() == 0) {
        // STRING: LEGOLAND 0x004bc828
        LogPrintf("Scripts Save Failed");
        goto fail;
    }
    if (SaveReport() == 0) {
        // STRING: LEGOLAND 0x004bc814
        LogPrintf("Report Save Failed");
        goto fail;
    }
    if (SaveCurrency() == 0) {
        // STRING: LEGOLAND 0x004bc7fc
        LogPrintf("Currency Save Failed");
        goto fail;
    }
    if (SaveGameWrite(ButtonFlashStates, 0x24) == 0) {
        // STRING: LEGOLAND 0x004bc7dc
        LogPrintf("Button flash states Save Failed");
        goto fail;
    }
    DrawWatchSprite();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc7c8
        LogPrintf("EndMeasured Block 3");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc7b0
        LogPrintf("Begin Measured Block 4");
        goto fail;
    }
    {
        struct Bloke *bloke;
        int n;
        int num;
        n = 0;
        for (bloke = FirstBloke; bloke != 0; bloke = bloke->next) {
            n++;
        }
        if (SaveGameWrite(&n, 4) == 0) {
            // STRING: LEGOLAND 0x004bc794
            LogPrintf("NumBlokes (%d) save Failed", n);
            goto fail;
        }
        for (bloke = FirstBloke; bloke != 0; bloke = bloke->next) {
            DrawWatchSprite();
            num = GetBlokeNum(bloke);
            if (SaveGameWrite(&num, 4) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc764
                    LogPrintf("Bloke Num (d) Save Failed", num);
                    goto fail;
                }
            }
            BlokeSaveBuffer.action = bloke->action;
            BlokeSaveBuffer.low_level_action = bloke->low_level_action;
            BlokeSaveBuffer.pending_action = bloke->pending_action;
            if (bloke->target != 0) {
                BlokeSaveBuffer.target.element = bloke->target;
                FindeIneList(&BlokeSaveBuffer.target);
            } else {
                BlokeSaveBuffer.target.index = -1;
            }
            if (bloke->last_ride != 0) {
                BlokeSaveBuffer.last_ride.element = bloke->last_ride;
                FindeIneList(&BlokeSaveBuffer.last_ride);
            } else {
                BlokeSaveBuffer.last_ride.index = -1;
            }
            BlokeSaveBuffer.field_1c = bloke->field_1c;
            BlokeSaveBuffer.field_20 = bloke->field_20;
            BlokeSaveBuffer.dest.x = bloke->dest.x;
            BlokeSaveBuffer.dest.y = bloke->dest.y;
            BlokeSaveBuffer.goal.x = bloke->goal.x;
            BlokeSaveBuffer.goal.y = bloke->goal.y;
            memcpy(&BlokeSaveBuffer.block_34, &bloke->field_34, sizeof(BlokeSaveBuffer.block_34));
            BlokeSaveBuffer.field_5c = bloke->field_5c;
            BlokeSaveBuffer.param_action = bloke->param_action;
            BlokeSaveBuffer.flags = bloke->flags;
            BlokeSaveBuffer.field_64 = bloke->field_64;
            BlokeSaveBuffer.field_78 = bloke->field_78;
            BlokeSaveBuffer.mood = bloke->mood;
            BlokeSaveBuffer.field_7c = bloke->field_7c;
            BlokeSaveBuffer.field_7e = bloke->field_7e;
            BlokeSaveBuffer.speed = bloke->speed;
            BlokeSaveBuffer.field_80 = bloke->field_80;
            BlokeSaveBuffer.field_81 = bloke->field_81;
            BlokeSaveBuffer.field_82 = bloke->field_82;
            BlokeSaveBuffer.favourite[0].element = bloke->favourite_attraction_0;
            FindeIneList(&BlokeSaveBuffer.favourite[0]);
            BlokeSaveBuffer.favourite[1].element = bloke->favourite_attraction_1;
            FindeIneList(&BlokeSaveBuffer.favourite[1]);
            BlokeSaveBuffer.favourite[2].element = bloke->favourite_attraction_2;
            FindeIneList(&BlokeSaveBuffer.favourite[2]);
            BlokeSaveBuffer.favourite[3].element = bloke->favourite_food;
            FindeIneList(&BlokeSaveBuffer.favourite[3]);
            BlokeSaveBuffer.pos.x = bloke->pos.x;
            BlokeSaveBuffer.pos.y = bloke->pos.y;
            BlokeSaveBuffer.height = bloke->height;
            BlokeSaveBuffer.dir = bloke->dir;
            BlokeSaveBuffer.target_dir = bloke->target_dir;
            BlokeSaveBuffer.frame = bloke->frame;
            BlokeSaveBuffer.field_75 = bloke->field_75;
            BlokeSaveBuffer.nav = bloke->nav;
            BlokeSaveBuffer.person_8 = bloke->person->character;
            BlokeSaveBuffer.scale = bloke->person->scale;
            BlokeSaveBuffer.screen = bloke->person->screen;
            BlokeSaveBuffer.offset = bloke->person->offset;
            BlokeSaveBuffer.field_34 = bloke->person->field_34;
            BlokeSaveBuffer.field_38 = bloke->person->field_38;
            BlokeSaveBuffer.depth = bloke->person->depth;
            BlokeSaveBuffer.rotation = bloke->person->rotation;
            BlokeSaveBuffer.person_4c = bloke->person->frame;
            BlokeSaveBuffer.anim = (unsigned int)bloke->person->anim;
            BlokeSaveBuffer.sort_id = bloke->person->sort_id;
            BlokeSaveBuffer.field_38 = bloke->person->field_38;
            memcpy(BlokeSaveBuffer.m, bloke->person->m, sizeof(BlokeSaveBuffer.m));
            BlokeSaveBuffer.field_7c_p = bloke->person->field_7c;
            BlokeSaveBuffer.field_80_p = bloke->person->field_80;
            BlokeSaveBuffer.field_8c_p = bloke->person->field_8c;
            BlokeSaveBuffer.field_90_p = bloke->person->field_90;
            BlokeSaveBuffer.random = bloke->person->random;
            BlokeSaveBuffer.prev_param = bloke->prev_param;
            BlokeSaveBuffer.prev_action = bloke->prev_action;
            BlokeSaveBuffer.field_30 = bloke->person->field_30;
            if (SaveGameWrite(&BlokeSaveBuffer, sizeof(BlokeSaveBuffer)) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc74c
                    LogPrintf("Bloke data failed (%d)", num);
                    goto fail;
                }
            }
            if (BlokeSaveBuffer.block_34.path != NULL) {
                if (SaveGameWrite(BlokeSaveBuffer.block_34.path, sizeof(struct BNVPath)) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc730
                        LogPrintf("Bloke BNV path data (%d)", num);
                        goto fail;
                    }
                }
            }
        }
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc780
        LogPrintf("EndMeasured Block 4");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc718
        LogPrintf("Begin Measured Block 5");
        goto fail;
    }
    SaveGardeners();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc704
        LogPrintf("EndMeasuredBlock5");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc6ec
        LogPrintf("Begin Measured Block 6");
        goto fail;
    }
    DrawWatchSprite();
    SaveMechanics();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc6d4
        LogPrintf("End Measured Block 6");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc6bc
        LogPrintf("Begin measured block 7");
        goto fail;
    }
    SaveGardenerOrders();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc6a4
        LogPrintf("End Measured Block 7");
        goto fail;
    }
    DrawWatchSprite();
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc68c
        LogPrintf("Begin Measured Block 8");
        goto fail;
    }
    SaveMechanicOrders();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc674
        LogPrintf("End Measured Block 8");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc65c
        LogPrintf("Begin Measured Block 9");
        goto fail;
    }
    if (SaveGameWrite(&CastlePlacedFlag, 4) == 0) {
        // STRING: LEGOLAND 0x004bc634
        LogPrintf("Castle Placed Flag %s",
            // STRING: LEGOLAND 0x004bc654
            CastlePlacedFlag != 0 ? "TRUE" :
                                  // STRING: LEGOLAND 0x004bc64c
                "FALSE");
        goto fail;
    }
    if (SaveGameWrite(&BuildObjCount, 4) == 0) {
        // STRING: LEGOLAND 0x004bc614
        LogPrintf("Num Build Objs (%d) Save Failed", BuildObjCount);
        goto fail;
    }
    if (SaveGameWrite(BuildObjArray, sizeof(BuildObjArray)) == 0) {
        // STRING: LEGOLAND 0x004bc5f0
        LogPrintf("BuildObjList (size %dS) Save Failed", BuildObjCount);
        goto fail;
    }
    for (i = 0; i < SavedElementCount; i++) {
        struct Ride *ride;
        struct ObjInstance *inst;
        struct RideNode *rnode;
        int count;
        DrawWatchSprite();
        if ((SavedElementTable[i]->flags & 0x10) != 0) {
            ride = SavedElementTable[i]->ride;
            SaveGameWrite(ride->element->name, 8);
            if (ride->type != 2 && ride->type != 0) {
                if (SaveGameWrite(ride->counters, lpConfig->max_blokes) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc5c4
                        LogPrintf("BeenOn Flags for %s Save Failed", ride->element->name);
                        goto fail;
                    }
                }
            }
            if (SaveGameWrite(&ride->field_8, 4) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc5b0
                    LogPrintf("Count for Object %s", ride->element->name);
                    goto fail;
                }
            }
            count = 0;
            for (inst = ride->instances; inst != 0; inst = inst->next) {
                count++;
            }
            if (SaveGameWrite(&count, 4) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc598
                    LogPrintf("Instance Count for %s", ride->element->name);
                    goto fail;
                }
            }
            for (inst = ride->instances; inst != 0; inst = inst->next) {
                if (SaveGameWrite(&inst->flags, 4) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc57c
                        LogPrintf("Flags for instance of %s", ride->element->name);
                        goto fail;
                    }
                }
                if (SaveGameWrite(&inst->uid, 4) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc560
                        LogPrintf("Objuid for instance of %s", ride->element->name);
                        goto fail;
                    }
                }
                if (SaveGameWrite(&inst->field_10, 4) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc540
                        LogPrintf("TickCount for instance of %s", ride->element->name);
                        goto fail;
                    }
                }
            }
            count = 0;
            for (rnode = ride->riders; rnode != 0; rnode = rnode->next) {
                count++;
            }
            if (SaveGameWrite(&count, 4) == 0) {
                {
                    // STRING: LEGOLAND 0x004bc51c
                    LogPrintf("Num Blokes On Ride for object %s", ride->element->name);
                    goto fail;
                }
            }
            for (rnode = ride->riders; rnode != 0; rnode = rnode->next) {
                int num;
                num = GetBlokeNum(rnode->rider);
                if (SaveGameWrite(&num, 4) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc500
                        LogPrintf("Bloke Num for bloke on %s", ride->element->name);
                        goto fail;
                    }
                }
                if (SaveGameWrite(&rnode->tile, 2) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc4e8
                        LogPrintf("Ride ID for bloke on %s", ride->element->name);
                        goto fail;
                    }
                }
            }
            if (ride->save_hook != 0) {
                if (ride->save_hook(ride->element) == 0) {
                    {
                        // STRING: LEGOLAND 0x004bc4c8
                        LogPrintf("Ride specific save data for %s", ride->element->name);
                        goto fail;
                    }
                }
            }
        }
    }
    if (SaveGameWrite(ObjInstanceTable, sizeof(ObjInstanceTable)) == 0) {
        // STRING: LEGOLAND 0x004bc5e4
        LogPrintf("RideTotal");
        goto fail;
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc4b0
        LogPrintf("End Measured VBlock 9");
        goto fail;
    }
    DrawWatchSprite();
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc498
        LogPrintf("Begin Measured Block 10");
        goto fail;
    }
    if (SaveBestNodeList() == 0) {
        // STRING: LEGOLAND 0x004bc48c
        LogPrintf("Path Rects");
        goto fail;
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc474
        LogPrintf("End Measured Block 10");
        goto fail;
    }
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc458
        LogPrintf("Begin Measured VBlock 11");
        goto fail;
    }
    SaveBuildObjList();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc440
        LogPrintf("End Measured Block 11");
        goto fail;
    }
    DrawWatchSprite();
    if (BeginMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc428
        LogPrintf("Begin Measured Block 12");
        goto fail;
    }
    {
        struct Overlay *ov;
        int n;
        n = strlen(OverlayILFHandle->name);
        SaveGameWrite(&n, 4);
        SaveGameWrite(OverlayILFHandle->name, n);
        if (BridgesHandle != 0) {
            n = strlen(((struct Element *)BridgesHandle)->name);
        } else {
            n = 0;
        }
        SaveGameWrite(&n, 4);
        if (n != 0) {
            SaveGameWrite(((struct Element *)BridgesHandle)->name, n);
        }
        n = 0;
        for (ov = OverlayList; ov != 0; ov = ov->next) {
            n++;
        }
        SaveGameWrite(&n, 4);
        for (ov = OverlayList; ov != 0; ov = ov->next) {
            SaveGameWrite(&ov->param, sizeof(ov->param));
        }
    }
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc410
        LogPrintf("End Measured Block 12");
        goto fail;
    }
    DrawWatchSprite();
    if (EndMeasuredBlock() == 0) {
        // STRING: LEGOLAND 0x004bc3f8
        LogPrintf("End Measured Block 13");
        goto fail;
    }
    _close(SaveFileHandle);
    return 1;
fail:
    _close(SaveFileHandle);
    return 0;
}

// FUNCTION: LEGOLAND 0x0047e980
LEGO_EXPORT int LoadGame(char *path) {
    int i;
    int x;
    int y;
    struct Ride *ride;
    struct MapElement *tile;
    struct Bloke *bloke;
    struct Anim3D *anim;
    struct ObjInstance *inst;
    struct ObjInstance *inst_prev;
    struct RideNode *rnode;
    struct RideNode *rnode_prev;
    struct Ride *entrance;
    struct MapElement *first;
    union {
        int n;
        struct Element *e; /* the slot also holds element handles */
    } u;
    unsigned int len;
    int m;
    char header[0x20];
    char name[0x200];

    SaveFileHandle = _open(path, _O_BINARY);
    if (SaveFileHandle == -1) {
        return 0;
    }
    MeasuredBlockDepth = 0;
    LoadInProgress = 1;
    if (SaveGameRead(header, 0x20) == 0) {
        goto fail;
    }
    header[5] = 0;
    if (atoi(header) != 2) {
        return 0;
    }
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    if (SaveGameRead(&SavedElementCount, 4) == 0) {
        goto fail;
    }
    if (SavedElementTable != 0) {
        free(SavedElementTable);
        SavedElementTable = 0;
    }
    SavedElementTable = malloc(SavedElementCount * sizeof(*SavedElementTable));
    for (i = 0; i < SavedElementCount; i++) {
        DrawWatchSprite();
        if (SaveGameRead(&len, 4) == 0) {
            goto fail;
        }
        if (SaveGameRead(name, len) == 0) {
            goto fail;
        }
        name[len] = 0;
        if (LLIDB_FindElement(name, &u.e, 0) != 0) {
            goto fail;
        }
        SavedElementTable[i] = u.e;
        if (SaveGameRead(&m, 4) == 0) {
            goto fail;
        }
        LLIDB_LoadData(u.e);
        (u.e)->flags &= 0xfffcfff1;
        (u.e)->flags |= m;
    }
    {
        unsigned short *tab[256];
        if (SaveGameRead(&SaveGameElementCount, 4) == 0) {
            goto fail;
        }
        for (i = 0; i < SaveGameElementCount; i++) {
            DrawWatchSprite();
            if (SaveGameRead(&len, 4) == 0) {
                goto fail;
            }
            if (SaveGameRead(name, len) == 0) {
                goto fail;
            }
            name[len] = 0;
            if (LLIDB_FindElement(name, &u.e, 0) != 0) {
                goto fail;
            }
            SaveGameElements[i] = u.e;
            LLIDB_LoadData(u.e);
            tab[i] = (u.e)->data;
        }
        if (SkipSaveGameDword() == 0) {
            goto fail;
        }
        if (SaveGameRead(lpConfig, 0x44) == 0) {
            goto fail;
        }
        ClearGameMap();
        for (y = 0; y < lpConfig->height; y++) {
            DrawWatchSprite();
            for (x = 0; x < lpConfig->width; x++) {
                tile = &GameMap[y][x];
                if (SaveGameRead(tile, sizeof(struct MapElement)) == 0) {
                    goto fail;
                }
                if ((tile->flags & 0x8a8) != 0) {
                    tile->field_0 = SavedElementTable[(int)(INT_PTR)tile->field_0];
                } else {
                    tile->field_0 = 0;
                }
                if (tile->field_8 != 0) {
                    tile->field_8 = *tab[(tile->field_8 >> 8) - 1] + (tile->field_8 & 0xff);
                }
                if (tile->field_a != 0) {
                    tile->field_a = *tab[(tile->field_a >> 8) - 1] + (tile->field_a & 0xff);
                }
            }
        }
    }
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    if (SaveGameRead(&MapStats, 0x3f0) == 0) {
        goto fail;
    }
    if (SaveGameRead(&ScrollX, 4) == 0) {
        goto fail;
    }
    if (SaveGameRead(&ScrollY, 4) == 0) {
        goto fail;
    }
    if (SaveGameRead(&EditMode, sizeof(struct EditState)) == 0) {
        goto fail;
    }
    EditMode.unk4 = 3;
    EditMode.unk8 = 0;
    LoadInputState();
    SetDefaultMouseHandlers();
    DrawWatchSprite();
    if (LoadScripts() == 0) {
        goto fail;
    }
    if (LoadReport() == 0) {
        goto fail;
    }
    if (LoadCurrency() == 0) {
        goto fail;
    }
    if (SaveGameRead(ButtonFlashStates, 0x24) == 0) {
        goto fail;
    }
    DrawWatchSprite();
    MapDataLoaded = 1;
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    while (FirstBloke != 0) {
        DestroyBloke(FirstBloke);
    }
    u.n = 0;
    DrawWatchSprite();
    if (SaveGameRead(&u.n, 4) == 0) {
        goto fail;
    }
    while (u.n-- != 0) {
        if (SaveGameRead(&m, 4) == 0) {
            goto fail;
        }
        bloke = &BlokePool[m];
        bloke->next = FirstBloke;
        FirstBloke = bloke;
        if (SaveGameRead(&BlokeSaveBuffer, sizeof(BlokeSaveBuffer)) == 0) {
            goto fail;
        }
        if (BlokeSaveBuffer.block_34.path != NULL) {
            BlokeSaveBuffer.block_34.path = malloc(sizeof(struct BNVPath));
            if (SaveGameRead(BlokeSaveBuffer.block_34.path, sizeof(struct BNVPath)) == 0) {
                goto fail;
            }
        }
        bloke->action = BlokeSaveBuffer.action;
        bloke->low_level_action = BlokeSaveBuffer.low_level_action;
        bloke->pending_action = BlokeSaveBuffer.pending_action;
        if (BlokeSaveBuffer.target.index != -1) {
            bloke->target = SavedElementTable[BlokeSaveBuffer.target.index];
        } else {
            bloke->target = 0;
        }
        if (BlokeSaveBuffer.last_ride.index != -1) {
            bloke->last_ride = SavedElementTable[BlokeSaveBuffer.last_ride.index];
        } else {
            bloke->last_ride = 0;
        }
        bloke->field_1c = BlokeSaveBuffer.field_1c;
        bloke->field_20 = BlokeSaveBuffer.field_20;
        bloke->dest.x = BlokeSaveBuffer.dest.x;
        bloke->dest.y = BlokeSaveBuffer.dest.y;
        bloke->goal.x = BlokeSaveBuffer.goal.x;
        bloke->goal.y = BlokeSaveBuffer.goal.y;
        memcpy(&bloke->field_34, &BlokeSaveBuffer.block_34, sizeof(BlokeSaveBuffer.block_34));
        bloke->field_5c = BlokeSaveBuffer.field_5c;
        bloke->param_action = BlokeSaveBuffer.param_action;
        bloke->flags = BlokeSaveBuffer.flags;
        bloke->field_64 = BlokeSaveBuffer.field_64;
        bloke->field_78 = BlokeSaveBuffer.field_78;
        bloke->mood = BlokeSaveBuffer.mood;
        bloke->field_7c = BlokeSaveBuffer.field_7c;
        bloke->field_7e = BlokeSaveBuffer.field_7e;
        bloke->speed = BlokeSaveBuffer.speed;
        bloke->field_80 = BlokeSaveBuffer.field_80;
        bloke->field_81 = BlokeSaveBuffer.field_81;
        bloke->field_82 = BlokeSaveBuffer.field_82;
        if (BlokeSaveBuffer.favourite[0].index < SavedElementCount) {
            bloke->favourite_attraction_0 = SavedElementTable[BlokeSaveBuffer.favourite[0].index];
        } else {
            bloke->favourite_attraction_0 = 0;
        }
        if (BlokeSaveBuffer.favourite[1].index < SavedElementCount) {
            bloke->favourite_attraction_1 = SavedElementTable[BlokeSaveBuffer.favourite[1].index];
        } else {
            bloke->favourite_attraction_1 = 0;
        }
        if (BlokeSaveBuffer.favourite[2].index < SavedElementCount) {
            bloke->favourite_attraction_2 = SavedElementTable[BlokeSaveBuffer.favourite[2].index];
        } else {
            bloke->favourite_attraction_2 = 0;
        }
        if (BlokeSaveBuffer.favourite[3].index < SavedElementCount) {
            bloke->favourite_food = SavedElementTable[BlokeSaveBuffer.favourite[3].index];
        } else {
            bloke->favourite_food = 0;
        }
        bloke->pos.x = BlokeSaveBuffer.pos.x;
        bloke->pos.y = BlokeSaveBuffer.pos.y;
        bloke->height = BlokeSaveBuffer.height;
        bloke->dir = BlokeSaveBuffer.dir;
        bloke->target_dir = BlokeSaveBuffer.target_dir;
        bloke->frame = BlokeSaveBuffer.frame;
        bloke->field_75 = BlokeSaveBuffer.field_75;
        bloke->nav = BlokeSaveBuffer.nav;
        bloke->person = malloc(sizeof(Person));
        AddPersonToList(bloke->person);
        bloke->person->bloke = bloke;
        bloke->person->character = BlokeSaveBuffer.person_8;
        bloke->person->scale = BlokeSaveBuffer.scale;
        bloke->person->screen = BlokeSaveBuffer.screen;
        bloke->person->offset = BlokeSaveBuffer.offset;
        bloke->person->field_34 = BlokeSaveBuffer.field_34;
        bloke->person->field_38 = BlokeSaveBuffer.field_38;
        bloke->person->depth = BlokeSaveBuffer.depth;
        bloke->person->rotation = BlokeSaveBuffer.rotation;
        bloke->person->frame = BlokeSaveBuffer.person_4c;
        bloke->person->anim = BlokeSaveBuffer.anim;
        bloke->person->sort_id = BlokeSaveBuffer.sort_id;
        bloke->person->field_38 = BlokeSaveBuffer.field_38;
        memcpy(bloke->person->m, BlokeSaveBuffer.m, sizeof(BlokeSaveBuffer.m));
        bloke->person->field_7c = BlokeSaveBuffer.field_7c_p;
        bloke->person->field_80 = BlokeSaveBuffer.field_80_p;
        bloke->person->field_8c = BlokeSaveBuffer.field_8c_p;
        bloke->person->field_90 = BlokeSaveBuffer.field_90_p;
        bloke->person->random = BlokeSaveBuffer.random;
        bloke->prev_param = BlokeSaveBuffer.prev_param;
        bloke->prev_action = BlokeSaveBuffer.prev_action;
        bloke->person->field_2c = 0;
        bloke->person->field_30 = BlokeSaveBuffer.field_30;
        anim = GetBlokeAnim3DFromPerson(bloke->person);
        bloke->person->field_50 = CreatePersonFaceList(bloke->person, VisitorLocData, anim->field_8, anim->elems->shared->count, bloke->person->random);
    }
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    LoadGardeners();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    LoadMechanics();
    DrawWatchSprite();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    LoadGardenerOrders();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    LoadMechanicWorkOrders();
    DrawWatchSprite();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    if (SaveGameRead(&CastlePlacedFlag, 4) == 0) {
        goto fail;
    }
    if (SaveGameRead(&BuildObjCount, 4) == 0) {
        goto fail;
    }
    if (SaveGameRead(BuildObjArray, sizeof(BuildObjArray)) == 0) {
        goto fail;
    }
    /* Per-ride state of every placed ride element. */
    for (i = 0; i < SavedElementCount; i++) {
        DrawWatchSprite();
        if ((SavedElementTable[i]->flags & 0x10) != 0) {
            SavedElementTable[i]->flags |= 4;
            ride = SavedElementTable[i]->ride;
            {
                // STRING: LEGOLAND 0x004bcb94
                char label[9] = "xxxxxxxx";
                if (SaveGameRead(label, 8) == 0) {
                    goto fail;
                }
                // STRING: LEGOLAND 0x004bcb90
                DBPrintf("%s\n", label);
            }
            if (ride->type != 2 && ride->type != 0) {
                ride->counters = malloc(lpConfig->max_blokes);
                if (SaveGameRead(ride->counters, lpConfig->max_blokes) == 0) {
                    goto fail;
                }
            }
            if (SaveGameRead(&ride->field_8, 4) == 0) {
                goto fail;
            }
            inst_prev = 0;
            u.n = 0;
            if (SaveGameRead(&u.n, 4) == 0) {
                goto fail;
            }
            while (u.n-- != 0) {
                inst = malloc(sizeof(struct ObjInstance));
                inst->next = 0;
                inst->ride = ride;
                if (inst_prev == 0) {
                    ride->instances = inst;
                    inst->prev = inst_prev;
                } else {
                    inst_prev->next = inst;
                    inst->prev = inst_prev;
                }
                inst_prev = inst;
                if (SaveGameRead(&inst->flags, 4) == 0) {
                    goto fail;
                }
                if (SaveGameRead(&inst->uid, 4) == 0) {
                    goto fail;
                }
                if (SaveGameRead(&inst->field_10, 4) == 0) {
                    goto fail;
                }
            }
            rnode_prev = 0;
            u.n = 0;
            if (SaveGameRead(&u.n, 4) == 0) {
                goto fail;
            }
            while (u.n-- != 0) {
                rnode = malloc(sizeof(struct RideNode));
                rnode->next = 0;
                if (rnode_prev == 0) {
                    ride->riders = rnode;
                    rnode->prev = rnode_prev;
                } else {
                    rnode_prev->next = rnode;
                    rnode->prev = rnode_prev;
                }
                rnode_prev = rnode;
                if (SaveGameRead(&m, 4) == 0) {
                    goto fail;
                }
                if (SaveGameRead(&rnode->tile, 2) == 0) {
                    goto fail;
                }
                rnode->rider = GetBlokePtr(m);
                rnode->person = rnode->rider->person;
            }
            if (ride->load_hook != 0) {
                if (ride->load_hook(ride->element) == 0) {
                    goto fail;
                }
            }
        }
    }
    if (SaveGameRead(ObjInstanceTable, sizeof(ObjInstanceTable)) == 0) {
        goto fail;
    }
    DrawWatchSprite();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    if (LoadBestNodeList() == 0) {
        goto fail;
    }
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    LoadBuildObjArray();
    DrawWatchSprite();
    if (SkipSaveGameDword() == 0) {
        goto fail;
    }
    {
        char name2[0x200];
        struct OverlayParam ov;
        SaveGameRead(&u.n, 4);
        SaveGameRead(name2, u.n);
        name2[u.n] = 0;
        LLIDB_FindElement(name2, &OverlayILFHandle, 0);
        OverlayILF = LLIDB_LoadData(OverlayILFHandle);
        SaveGameRead(&u.n, 4);
        if (u.n != 0) {
            SaveGameRead(name2, u.n);
            name2[u.n] = 0;
            LLIDB_FindElement(name2, &BridgesHandle, 0);
            BridgesData = LLIDB_LoadData(BridgesHandle);
            SetBridgeParamsByName(name2);
        } else {
            BridgesHandle = 0;
            BridgesData = 0;
        }
        SaveGameRead(&u.n, 4);
        for (i = 0; i < u.n; i++) {
            SaveGameRead(&ov, sizeof(ov));
            AddOvSav(&ov);
        }
    }
    _close(SaveFileHandle);
    DrawWatchSprite();
    LoadInProgress = 0;
    GamePad |= 0x20;
    CalculateMapRenderOrder();
    // STRING: LEGOLAND 0x004b83d0
    Entrance1Elem = ElemID("ENTRANCE 1");
    entrance = Entrance1Elem->ride;
    first = GetFirstObjectMatching(Entrance1Elem);
    if (first != 0) {
        ParkExitPos.x = (first->field_4 + entrance->footprint.v[0]) * 0x100 + -0x100;
        ParkExitPos.y = (((entrance->footprint.v[3] - entrance->footprint.v[1]) << 7) & ~0xff) + ((first->field_5 + entrance->footprint.v[1]) << 8);
    }
    RestoreBuildMenuAfterLoad();
    SetMapLoaded(1);
    LevelLoadedFromSave = 1;
    MouseClickConsumed = 1;
    EditMode.unk0 = 0;
    GamePad &= ~0x1000;
    RepostScriptIntro();
    return 1;
fail:
    _close(SaveFileHandle);
    LoadInProgress = 0;
    return 0;
}

// FUNCTION: LEGOLAND 0x0047f760
LEGO_EXPORT void UnloadSaveGameMap(void) {
    int i;

    if (SavedElementTable != 0) {
        for (i = 0; i < SavedElementCount; i++) {
            LLIDB_UnLoadData(SavedElementTable[i]);
        }
        free(SavedElementTable);
        SavedElementTable = 0;
    }

    for (i = 0; i < SaveGameElementCount; i++) {
        LLIDB_UnLoadData(SaveGameElements[i]);
    }

    ClearGameMap();
    LLIDB_UnLoadData(OverlayILFHandle);
    if (BridgesHandle != 0) {
        LLIDB_UnLoadData(BridgesHandle);
    }
    ClearOverlays();
    FreeBestNodeList();
    MapDataLoaded = 0;
}

// FUNCTION: LEGOLAND 0x0047f810
void MarkGameTimer(void) {
    GameTimerMark = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x0047f820
int GetGameTimerSinceMark(void) {
    return GetGameTimer() - GameTimerMark;
}

// FUNCTION: LEGOLAND 0x0047f830
unsigned int OpenLogFile(const char *path) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0047f840
int CloseLogFile(void) {
    return 1;
}

/* Empty in the original (does nothing). */
// FUNCTION: LEGOLAND 0x0047f850
void Empty_0047f850(void) {
    return;
}
