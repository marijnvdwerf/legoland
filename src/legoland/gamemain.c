#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "bloke.h"
#include "build.h"
#include "challenge.h"
#include "controller.h"
#include "draw.h"
#include "gamemain.h"
#include "gamemap.h"
#include "interface.h"
#include "llidb.h"
#include "map_object.h"
#include "nerps.h"
#include "objclass.h"
#include "objectives.h"
#include "path_control.h"
#include "pathfind.h"
#include "resource.h"
#include "string.h"
#include "tilemap.h"
#include "title.h"
#include "worker.h"

struct GameMainNode {
    struct GameMainNode *next;
    struct GameMainNode *parent;
    int x;
    int y;
    int step_cost;
    int path_cost;
    int heuristic;
    int field_1c;
    int field_20;
    int field_24;
};

// FUNCTION: LEGOLAND 0x00477680
int IsInMapBounds(int a, int b) {
    unsigned short limit;

    if (a < 0) {
        return 0;
    }
    if (b < 0) {
        return 0;
    }
    if (a >= lpConfig->width) {
        return 0;
    }
    limit = lpConfig->height;
    return b < limit;
}

// FUNCTION: LEGOLAND 0x004776c0
void AddToClosedList(struct GameMainNode *node) {
    node->next = ClosedPathNodeList;
    ClosedPathNodeList = node;
}

// FUNCTION: LEGOLAND 0x004776e0
void InsertOpenListSorted(struct GameMainNode *node) {
    struct GameMainNode *current;
    struct GameMainNode *previous;

    if (OpenPathNodeList == NULL) {
        OpenPathNodeList = node;
        node->next = NULL;
    } else {
        current = OpenPathNodeList;
        previous = NULL;
        while (current != NULL) {
            if (current->field_1c >= node->field_1c) {
                break;
            }
            previous = current;
            current = current->next;
        }
        if (previous != NULL) {
            node->next = previous->next;
            previous->next = node;
        } else {
            node->next = OpenPathNodeList;
            OpenPathNodeList = node;
        }
    }
}

// FUNCTION: LEGOLAND 0x00477730
struct GameMainNode *FindClosedPathNode(struct Point *ctx) {
    struct GameMainNode *node;

    node = ClosedPathNodeList;
    if (node == NULL) {
        return NULL;
    }
    while (node != NULL) {
        if (node->x == ctx->x && node->y == ctx->y) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00477760
void RemoveFromClosedList(struct GameMainNode *ctx) {
    struct GameMainNode *prev;
    struct GameMainNode *node;

    prev = NULL;
    node = ClosedPathNodeList;
    while (node != NULL) {
        if (node == ctx) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        ClosedPathNodeList = node->next;
    }
}

// FUNCTION: LEGOLAND 0x00477790
void RemoveFromOpenList(struct GameMainNode *param_1) {
    struct GameMainNode *prev;
    struct GameMainNode *node;

    prev = NULL;
    node = OpenPathNodeList;
    while (node != NULL) {
        if (node == param_1) {
            break;
        }
        prev = node;
        node = node->next;
    }
    if (prev != NULL) {
        prev->next = node->next;
        return;
    }
    OpenPathNodeList = node->next;
}

// FUNCTION: LEGOLAND 0x004777c0
struct GameMainNode *FindOpenPathNode(struct Point *arg) {
    struct GameMainNode *node = OpenPathNodeList;
    int first;

    if (node == NULL) {
        return NULL;
    }
    first = arg->x;
    do {
        if (node->x == first && node->y == arg->y) {
            return node;
        }
        node = node->next;
    } while (node != NULL);
    return NULL;
}

// FUNCTION: LEGOLAND 0x004777f0
struct GameMainNode *GetPathNodeAt(struct Point *pos, int *result) {
    struct GameMainNode *node;
    struct MapElement *tile;
    struct Ride *ride;
    unsigned short flags;
    int dx;
    int dy;

    node = FindClosedPathNode(pos);
    if (node != NULL) {
        *result = 2;
        return node;
    }
    node = FindOpenPathNode(pos);
    if (node != NULL) {
        *result = 1;
        return node;
    }
    *result = 0;
    node = (struct GameMainNode *)malloc(sizeof(struct GameMainNode));
    if (pos->x >= 0 && pos->x < lpConfig->width && pos->y >= 0 && pos->y < lpConfig->height) {
        tile = &GameMap[pos->y][pos->x];
    } else {
        tile = NULL;
    }
    flags = tile->flags;
    if ((flags & 0x10) || (tile->field_10 & 1)) {
        if (IsPathPointLinked(pos)) {
            node->field_20 = 1;
        } else {
            node->field_20 = 0;
        }
        node->step_cost = 1;
    } else if (flags & 0x40) {
        node->field_20 = 5;
        node->step_cost = -1;
    } else if (flags & 0x8a0) {
        ride = tile->field_0->ride;
        if (ride->flags & 0x200000) {
            if (ride->range > 1) {
                node->field_20 = 4;
                node->step_cost = 0x14;
            } else {
                node->field_20 = 3;
                node->step_cost = 9;
            }
        } else {
            node->field_20 = 5;
            node->step_cost = -1;
        }
    } else if (tile->field_10 & 2) {
        node->field_20 = 5;
        node->step_cost = -1;
    } else {
        node->field_20 = 2;
        node->step_cost = 3;
    }
    node->x = pos->x;
    node->y = pos->y;
    node->path_cost = 0x7fffffff;
    dy = abs(pos->y - AStarGoalY);
    dx = abs(pos->x - AStarGoalX);
    node->parent = 0;
    node->field_24 = 0;
    node->heuristic = dx + dy;
    node->field_1c = node->step_cost + node->heuristic;
    return node;
}

/* A-star axis-change penalty: returns zero when previous and next axis masks overlap, otherwise four. Used in
 * addition to tile step cost for eligible path nodes. */
// FUNCTION: LEGOLAND 0x00477980
unsigned int GetPathAxisTurnCost(unsigned int param_1, unsigned int param_2) {
    return ((param_1 & param_2) != 0 ? 0xfffffffc : 0u) + 4;
}

/* Axis mask between two tile positions for A-star turn costs: 1 for vertical, 2 for horizontal, zero for coincident
 * or diagonal positions. */
// FUNCTION: LEGOLAND 0x004779a0
int GetPathStepAxisMask(int x0, int y0, int x1, int y1) {
    int dx = x0 - x1;
    int dy = y0 - y1;

    if (dx != 0 && dy != 0) {
        return 0;
    }
    if (dx != 0) {
        return 2;
    }
    return dy != 0;
}

// FUNCTION: LEGOLAND 0x004779d0
void RemoveObjectAtPoint(struct Point *p) {
    struct MapElement *tile;
    struct Element *elem;
    struct Ride *saved_class;
    struct Cursor saved;
    TileId t;
    struct Point pos;
    WorkOrder *order;

    struct Element *ptmp12;
    register int ptmp60;
    int ptmp62;
    void *ptmp63;
    int ptmp184;
    if (p->x >= 0 && p->x < (unsigned int)lpConfig->width && p->y >= 0 && (unsigned int)p->y < lpConfig->height) {
        tile = &GameMap[p->y][p->x];
    } else {
        tile = NULL;
    }
    if (tile->flags & 0x40) {
        return;
    }
    if (0xa0 & tile->flags) {
        elem = tile->field_0;
        saved_class = QueryClass;
        memcpy(&saved, &QueryCursor, sizeof(struct Cursor));
        QueryClass = (struct Ride *)elem->data;
        t.pos.x = tile->field_4;
        ptmp60 = t.pos.x;
        pos.x = ptmp60;
        t.pos.y = tile->field_5;
        pos.y = t.pos.y;
        QueryClass->cb_94((unsigned int *)elem, &pos);
        if (tile->flags & 0x20) {
            if (QueryClass->flags & 0x200000) {
                RemoveObjectFromBuildList(t);
                ClearObjectEntrances((struct Element *)elem, &pos.x);
                IncrementObjectCount((struct Ride *)QueryClass);
                RemoveObjectFromMap(t);
            }
        } else {
            RemObjFromMap(QueryClass, elem, t, &QueryCursor);
        }
        memcpy(&QueryCursor, &saved, sizeof(struct Cursor));
        QueryClass = saved_class;
        return;
    }
    if (tile->flags & 0x800) {
        order = GetGardenerWorkOrderAt(p->x, p->y);
        if (NULL != order) {
            ClearObjectEntrances((struct Element *)order->element, &order->pos.x);
            ptmp63 = order->element->data;
            ClearPathUnderFootprint((struct Ride *)ptmp63, &order->pos.x);
            EraseGardenerOrder(order);
            return;
        }
        ptmp62 = p->x;
        ptmp184 = p->y;
        order = GetMechanicWorkOrderAt(ptmp62, ptmp184);
        if (order != NULL) {
            ptmp12 = order->element;
            ClearObjectEntrances((struct Element *)ptmp12, &order->pos.x);
            ClearPathUnderFootprint((struct Ride *)order->element->data, &order->pos.x);
            EraseMechanicOrder(order);
            return;
        }
    } else if (8 & tile->flags) {
        RemovePathSquare(p);
        tile->flags &= 0xfff7;
        tile->field_10 &= 0xfe;
    }
}

// FUNCTION: LEGOLAND 0x00477bd0
void FindMapPathAStar(int x, int y, int a, int b) {
    register struct GameMainNode *best;
    struct GameMainNode *cur;
    register struct GameMainNode *nb;
    struct Element *id;
    struct MapElement *tile;
    int result;
    struct Point pos;
    struct Point *pp;
    int cost;

    unsigned int cur_x;
    int cur_y;
    int pos_x;
    int cur_x2;
    AStarStartPoint.x = x;
    AStarStartPoint.y = y;
    best = NULL;
    AStarGoalX = a;
    AStarGoalY = b;
    nb = GetPathNodeAt(&AStarStartPoint, &result);
    nb->path_cost = 0;
    InsertOpenListSorted((struct GameMainNode *)nb);
    while ((cur = OpenPathNodeList) != NULL) {
        OpenPathNodeList = cur->next;

        if ((AStarGoalX == cur->x && cur->y == AStarGoalY) || cur->field_20 == 1) {
            best = cur;
        } else {
            pos.x = cur->x;
            pos.y = cur->y - 1;
            if (IsInMapBounds(pos.x, pos.y)) {
                nb = GetPathNodeAt(&pos, &result);
                if (nb->step_cost != -1) {
                    if (nb->field_20 && (unsigned)nb->field_20 != 1) {
                        cur_y = cur->y;
                        cost = GetPathAxisTurnCost(cur->field_24, GetPathStepAxisMask(cur->x, cur_y, nb->x, nb->y)) + cur->path_cost + nb->step_cost;
                    } else {
                        cost = cur->path_cost + nb->step_cost;
                    }
                    if (result == 0) {
                        if (NULL != best && cost > best->path_cost) {
                            AddToClosedList((struct GameMainNode *)nb);
                        } else {
                            cur_x2 = cur->x;
                            nb->field_24 = GetPathStepAxisMask(cur_x2, cur->y, nb->x, nb->y);
                            nb->parent = cur;
                            nb->path_cost = cost;
                            nb->field_1c = nb->heuristic + cost;
                            if (result == 2) {
                                RemoveFromClosedList((struct GameMainNode *)nb);
                            }
                            if (result == 1) {
                                RemoveFromOpenList((struct GameMainNode *)nb);
                            }
                            InsertOpenListSorted((struct GameMainNode *)nb);
                        }
                    }
                }
            }
            pos.x = cur->x + 1;
            pos.y = cur->y;
            if (IsInMapBounds(pos.x, pos.y)) {
                nb = GetPathNodeAt(&pos, &result);
                cur_x = cur->x;
                nb->field_24 = GetPathStepAxisMask(cur_x, cur->y, nb->x, nb->y);
                if (nb->step_cost != -1) {
                    if (!(0 != nb->field_20 && nb->field_20 != 1)) {
                        cost = cur->path_cost + nb->step_cost;
                    } else {
                        cost = GetPathAxisTurnCost(cur->field_24, GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y)) + cur->path_cost + nb->step_cost;
                    }
                    if (result == 0) {
                        if (NULL != best && cost > best->path_cost) {
                            AddToClosedList((struct GameMainNode *)nb);
                        } else {
                            nb->field_24 = GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y);
                            nb->parent = cur;
                            nb->path_cost = cost;
                            nb->field_1c = nb->heuristic + cost;
                            if (result == 2) {
                                RemoveFromClosedList((struct GameMainNode *)nb);
                            }
                            if (result == 1) {
                                RemoveFromOpenList((struct GameMainNode *)nb);
                            }
                            InsertOpenListSorted((struct GameMainNode *)nb);
                        }
                    }
                }
            }
            pos_x = cur->x;
            pos.x = pos_x;
            pos.y = cur->y + 1;
            if (IsInMapBounds(pos.x, pos.y)) {
                nb = GetPathNodeAt(&pos, &result);
                nb->field_24 = GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y);
                if (nb->step_cost != -1) {
                    if (!(nb->field_20 != 0 && nb->field_20 != 1)) {
                        cost = cur->path_cost + nb->step_cost;
                    } else {
                        cost = GetPathAxisTurnCost(cur->field_24, GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y)) + cur->path_cost + nb->step_cost;
                    }
                    if (result == 0) {
                        if (NULL != best && cost > best->path_cost) {
                            AddToClosedList((struct GameMainNode *)nb);
                        } else {
                            nb->field_24 = GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y);
                            nb->parent = cur;
                            nb->path_cost = cost;
                            nb->field_1c = nb->heuristic + cost;
                            if (result == 2) {
                                RemoveFromClosedList((struct GameMainNode *)nb);
                            }
                            if (result == 1) {
                                RemoveFromOpenList((struct GameMainNode *)nb);
                            }
                            InsertOpenListSorted((struct GameMainNode *)nb);
                        }
                    }
                }
            }
            pos.x = cur->x - 1;
            pos.y = cur->y;
            if (IsInMapBounds(pos.x, pos.y)) {
                nb = GetPathNodeAt(&pos, &result);
                nb->field_24 = GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y);
                if (nb->step_cost != -1) {
                    if (0 != nb->field_20 && nb->field_20 != 1) {
                        cost = GetPathAxisTurnCost(cur->field_24, GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y)) + cur->path_cost + nb->step_cost;
                    } else {
                        cost = cur->path_cost + nb->step_cost;
                    }
                    if (result == 0) {
                        if (best != NULL && cost > best->path_cost) {
                            AddToClosedList((struct GameMainNode *)nb);
                        } else {
                            nb->field_24 = GetPathStepAxisMask(cur->x, cur->y, nb->x, nb->y);
                            nb->parent = cur;
                            nb->path_cost = cost;
                            nb->field_1c = nb->heuristic + cost;
                            if (2 == result) {
                                RemoveFromClosedList((struct GameMainNode *)nb);
                            }
                            if (result == 1) {
                                RemoveFromOpenList((struct GameMainNode *)nb);
                            }
                            InsertOpenListSorted((struct GameMainNode *)nb);
                        }
                    }
                }
            }
        }
        AddToClosedList((struct GameMainNode *)cur);
    }
    id = ElemID(PathControlName);
    nb = best;
    while (nb) {
        pp = (struct Point *)&nb->x;
        if (0 <= pp->x && pp->x < lpConfig->width && pp->y >= 0 && pp->y < lpConfig->height) {
            tile = &GameMap[pp->y][pp->x];
        } else {
            tile = NULL;
        }
        if (!(tile->flags & 0x10)) {
            if ((tile->field_10 & 3) != 3) {
                RemoveObjectAtPoint(pp);
                AddBasicPath((struct Element *)id, (int *)pp);
            }
        }
        nb = nb->parent;
    }
    while (ClosedPathNodeList != NULL) {
        RemoveFromClosedList(ClosedPathNodeList);
    }
}

// FUNCTION: LEGOLAND 0x00478110
int TokenizeString(char *str, const char *delims, char **out) {
    int count = 0;
    char *p;
    int n;

    while (*str) {
        str += strspn(str, delims);
        if (*str) {
            if (*str == '"') {
                *str = 0;
                str++;
                *out = str;
                count++;
                out++;
                p = strchr(str, '"');
                if (!p)
                    return count;
                *p = 0;
                str = p + 1;
            } else {
                *out = str;
                count++;
                out++;
                n = strcspn(str, delims);
                p = strchr(str, '"');
                if (p && p - str < n) {
                    str = p;
                } else {
                    str += n;
                    if (!*str)
                        return count;
                    *str = 0;
                    str++;
                }
            }
        }
    }
    return count;
}

// FUNCTION: LEGOLAND 0x004781b0
int FindStringNoCase(const char *param_1, const void *param_2, int param_3) {
    int i;

    for (i = 0; i < param_3; i++) {
        if (_strcmpi(param_1, ((const char **)param_2)[i]) == 0) {
            return i;
        }
    }
    return -1;
}
// FUNCTION: LEGOLAND 0x004781f0
int ParseScriptFile(const char *name, struct ScriptCommand *commands, int count, int flags) {
    char path[256];
    struct ResFile *file;
    int result;

    sprintf(path, "%s%s",
        // STRING: LEGOLAND 0x004bc084
        "Scripts\\", name);
    file = RES_OpenFile(path);
    if (file != NULL) {
        strncpy(CurrentScriptFileName, name, 128);
        result = ParseScriptResFile(file, commands, count, flags);
        RES_CloseFile(file);
        return result;
    }
    return 0;
}

/* Reads resource-script lines, strips hash comments, tokenizes them and dispatches matching or default command
 * handlers. Counts zero-status failures and optionally calls the final check handler. */
// FUNCTION: LEGOLAND 0x00478280
int ParseScriptResFile(struct ResFile *file, struct ScriptCommand *commands, int count, int flags) {
    char line[1024];
    char *tokens[20];
    char *p;
    unsigned int r;
    int errors = 0;
    ScriptCommandFn deflt = NULL;
    int ntok;
    int i;
    char found;
    struct ScriptCommand *cmd;

    ScriptResourceLineNumber = 0;
    cmd = commands;
    r = 0;
    // STRING: LEGOLAND 0x004bbdcc
    if (strcmp(cmd->name, "none") == 0)
        deflt = cmd->fn;
    while (ReadResFileLine(file, line, 1024)) {
        ScriptResourceLineNumber++;
        DrawWatchSprite();
        p = strchr(line, '#');
        if (p)
            *p = 0;
        // STRING: LEGOLAND 0x004bc098
        ntok = TokenizeString(line, " ,;:(){}\xa0\n\t", tokens);
        if (ntok) {
            UppercaseStringAndGetLength(tokens[0]);
            found = 0;
            for (i = 0; i < count; i++, cmd++) {
                if (strcmp(tokens[0], cmd->name) == 0) {
                    found = 1;
                    r = commands[i].fn(tokens, ntok - 1, flags);
                    if (0 == r)
                        errors++;
                    break;
                }
            }
            cmd = commands;
            if (!found) {
                if (deflt)
                    r = deflt(tokens, ntok - 1, flags);
            }
        }
        if (r < 0)
            return r;
    }
    if (r < 0)
        return r;
    // STRING: LEGOLAND 0x004bc090
    if (strcmp(commands[count - 1].name, "check") == 0)
        r = commands[count - 1].fn(NULL, errors, flags);
    if (r < 0)
        return r;
    return errors;
}

// FUNCTION: LEGOLAND 0x004784c0
void ResetLevelScriptState(void) {
    NEWFLC_Repeat = 0;
    NEWFLC_BuffSize = 1;
    NEWFLC_CheckDuplicate = 1;
    ScriptSectionTypeMask = 0;
    ScriptObjectiveKind = 1;
    CurrentObjectiveEventFlags = 0;
    CurrentScriptSection = 0;
    NextScriptSectionId = 0;

    lpConfig->field_30 = 0;
    lpConfig->gardeners_enabled = 1;
    lpConfig->mechanics_enabled = 1;
    lpConfig->max_blokes = 0xc8;
    MapStats.capacity_max = lpConfig->max_blokes;
    MapStats.capacity_min = 0;

    FreeScriptStrings();
    ScriptPromptStringIndex = AddScriptString(0, 0, 0);
    ScriptConditionActive = 1;

    ResetReportFlags();
    ResetAppraisalStreak();
    StopAppraisalTimer();
    ClearObjectiveCounters();
    Empty_004688e0();
    ConfigureAppraisal(0, 0);
    ClearBriefingAndHintsFileNames();
    SetDefaultMoodDeltas();
    SetDefaultClassLimits();
    ClearInterfaceSlotValues();
    ClearButtonFlashStates();
    SetPendingMovie(EmptyString);
    ResetDamageTimer();
    ResetPathUpdateTimer();
    ResetPathSceneryTimer();

    MapStats.brick_meter_max = 1000;
    MapStats.show_capacity = 0;
    MapStats.ride_wear = 0;
    MapStats.field_3a8 = 1;

    SetScriptStopped(0);
}

// FUNCTION: LEGOLAND 0x004785d0
void SetScriptSectionType(char *param_1, unsigned int param_2) {
    strcpy(ScriptSectionName, param_1);
    ScriptSectionTypeMask = param_2;
}

// FUNCTION: LEGOLAND 0x00478610
void SetScriptObjectiveKind(unsigned int param_1) {
    ScriptObjectiveKind = param_1;
    switch (param_1) {
    case 1:
        CurrentObjectiveEventFlags = 1;
        break;
    case 2:
        CurrentObjectiveEventFlags = 2;
        break;
    case 3:
        CurrentObjectiveEventFlags = 4;
        break;
    default:
        CurrentObjectiveEventFlags = 0;
        break;
    }
}

/* For a script command with an argument, sets current objective flag 8 when arg1 is PURGE (case insensitive),
 * otherwise clears it. Leaves the flags unchanged when there is no argument. */
// FUNCTION: LEGOLAND 0x00478650
void SetScriptObjectivePurgeFlag(struct CommandArgs *param_1, unsigned int param_2) {
    if (param_2 == 0) {
        return;
    }
    // STRING: LEGOLAND 0x004bb9ec
    if (_stricmp(param_1->arg1, "PURGE") == 0) {
        CurrentObjectiveEventFlags |= 0x8;
    } else {
        CurrentObjectiveEventFlags &= 0xf7;
    }
}

// FUNCTION: LEGOLAND 0x00478690
int HasEnoughScriptArgs(void *param_1, unsigned int param_2, unsigned int param_3) {
    return (int)param_2 >= (int)param_3;
}

// FUNCTION: LEGOLAND 0x004786a0
int IsScriptCommandAllowed(void *param_1, unsigned int param_2, unsigned int param_3) {
    return (ScriptSectionTypeMask & param_3) != 0;
}

// FUNCTION: LEGOLAND 0x004786c0
unsigned int CheckScriptCommand(void *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4) {
    if (IsScriptCommandAllowed(param_1, param_2, param_3) == 0) {
        return 0;
    }
    return HasEnoughScriptArgs(param_1, param_2, param_4) != 0;
}

// FUNCTION: LEGOLAND 0x00478700
void ParseRect(int *param_1, char **param_2, int param_3) {
    int temp;

    param_1[0] = atoi(param_2[param_3 + 0]);
    param_1[1] = atoi(param_2[param_3 + 1]);
    param_1[2] = atoi(param_2[param_3 + 2]);
    param_1[3] = atoi(param_2[param_3 + 3]);

    if (param_1[0] > param_1[2]) {
        temp = param_1[0];
        param_1[0] = param_1[2];
        param_1[2] = temp;
    }
    if (param_1[1] > param_1[3]) {
        temp = param_1[1];
        param_1[1] = param_1[3];
        param_1[3] = temp;
    }
}

// FUNCTION: LEGOLAND 0x00478770
void ParseIntPair(int *param_1, char **param_2, int param_3) {
    param_1[0] = atoi(param_2[param_3 + 0]);
    param_1[1] = atoi(param_2[param_3 + 1]);
}

// FUNCTION: LEGOLAND 0x004787a0
char *GetScriptArg1(struct CommandArgs *param_1, unsigned int param_2) {
    return param_1->arg1;
}
