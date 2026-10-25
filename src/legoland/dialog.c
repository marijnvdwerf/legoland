#include "dialog.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clipping.h"
#include "controller.h"
#include "draw.h"
#include "gfx.h"
#include "globals.h"
#include "image_sprite.h"
#include "input.h"
#include "legoland.h"
#include "llidb.h"
#include "print_sprite.h"
#include "render.h"
#include "text.h"
#include "wndenv.h"

// FUNCTION: LEGOLAND 0x0043e930
int DialogSlider(RECT *rc, int min, int max, int value, int step) {
    int pos;

    pos = (rc->bottom - rc->top) * value / (max - min);
    RenderThickBox(rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, 2, 0);
    RenderBlock(rc->left, rc->top + pos - 1, rc->right - rc->left, 3, GetNearestColour(0xff, 0, 0));
    if ((MouseClickFlags & 4) && MousePos.x >= rc->left && MousePos.x <= rc->right &&
        MousePos.y >= rc->top && MousePos.y <= rc->bottom) {
        DialogSliderDragging = 1;
    } else if (!DialogSliderDragging) {
        return value;
    }
    if (MouseClickFlags & 4) {
        pos = MousePos.y;
        if (pos < rc->top) {
            pos = rc->top;
        } else if (pos > rc->bottom) {
            pos = rc->bottom;
        }
        return (pos - rc->top) * (max - min) / (rc->bottom - rc->top);
    }
    DialogSliderDragging = 0;
    return value;
}

// FUNCTION: LEGOLAND 0x0043ea30
int ListBoxDialog(char **names, char *title, struct Sprite *bg, RECT *box, void (*callback)(int), struct Sprite **icons, int w, int h, int flag) {
    RECT clip;
    RECT bar;
    RECT saved;
    RECT *items;
    int sel;
    int n;
    int retry;
    int i;
    int y;
    int tw;
    int itemW;
    int viewH;
    int over;
    int my;

    sel = -1;
    clip.left = box->left + 8;
    clip.top = box->top + 32;
    clip.right = box->right + box->left - 9;
    clip.bottom = box->top + box->bottom - 9;
    retry = 0;
    DialogSliderDragging = 0;
    if (!flag) {
        DialogListScrollY = 0;
    }
    n = 0;
    while (names[n]) {
        n++;
    }
    items = malloc(n * 16);
    viewH = clip.bottom - clip.top + 1;
    /* lay the items out; if they overflow, narrow the list for a scroll bar and redo it once */
    for (;;) {
        itemW = clip.right - clip.left - 1;
        y = 0;
        for (i = 0; i < n; i++, y += 4) {
            items[i].top = y;
            tw = MeasureTextHeight(names[i], 2, itemW);
            if (tw < 8) {
                tw = 8;
            } else if (tw < 0) {
                tw = 0;
            }
            y += tw;
            items[i].bottom = y;
        }
        if (!retry && y > viewH) {
            clip.right -= 16;
            retry = 1;
            continue;
        }
        break;
    }
    over = y - viewH;
    for (i = 0; i < n; i++) {
        items[i].left = 0;
        items[i].right = itemW;
    }
    bar.left = box->right + box->left - 20;
    bar.top = clip.top;
    bar.right = box->right + box->left - 5;
    bar.bottom = clip.bottom;
    while (ProcessSystemEvents()) {
        ReadGameButtons();
        if ((CancelKeyState & 1) || (RightClickFlags & 1)) {
            return -1;
        }
        if (MousePos.x >= clip.left && MousePos.x <= clip.right && MousePos.y >= clip.top && MousePos.y <= clip.bottom) {
            my = MousePos.y - clip.top + DialogListScrollY;
            for (i = 0; i < n; i++) {
                if (my >= items[i].top) {
                    if (my <= items[i].bottom) {
                        sel = i;
                        if (!DialogSliderDragging && (MouseClickFlags & 2)) {
                            return i;
                        }
                        break;
                    }
                }
            }
            if (i == n) {
                sel = -1;
            }
        } else {
            sel = -1;
        }
        PushRenderingStatusAndLockVideoSurface();
        PrintSprite(bg, 0, 0, 0, 0);
        if (callback) {
            callback(sel);
        }
        RenderBlock(box->left, box->top, box->right, box->bottom, GetNearestColour(0xef, 0xef, 0xef));
        RenderThickBox(box->left, box->top, box->right, box->bottom, 2, 0);
        RenderBlock(box->left + 2, box->top + 2, box->right - 4, 0x18, GetNearestColour(0, 0x3f, 0x7f));
        PrintLimitedText(box->left + 2, box->top + 2, box->right - 4, title, 0, 0xefefef, 0);
        if (retry) {
            DialogListScrollY = DialogSlider(&bar, 0, over, DialogListScrollY, 0x10);
        }
        GetClipping(&saved);
        SetClipping(&clip);
        /* skip the items scrolled off the top */
        for (i = 0; i < n; i++) {
            if (items[i].bottom >= DialogListScrollY) {
                break;
            }
        }
        for (; i < n; i++) {
            if (items[i].top >= DialogListScrollY + viewH) {
                break;
            }
            if (sel == i) {
                RenderBlock(items[i].left + clip.left, items[i].top - DialogListScrollY + clip.top, items[i].right - items[i].left + 1, items[i].bottom - items[i].top + 1, GetNearestColour(0x7f, 0x7f, 0xef));
            } else {
                RenderBox(items[i].left + clip.left, items[i].top - DialogListScrollY + clip.top, items[i].right - items[i].left + 1, items[i].bottom - items[i].top + 1, GetNearestColour(0xcf, 0xcf, 0xcf));
            }
            DrawWrappedTextAt(items[i].left + clip.left, items[i].top - DialogListScrollY + clip.top, names[i], 2, itemW);
        }
        SetClipping(&saved);
        RenderingComplete();
        PopRenderingStatus();
    }
    return -1;
}

// FUNCTION: LEGOLAND 0x0043eee0
struct Element *SelectElementDialog(char *title, struct Sprite *bg, RECT *box, unsigned int mask, int flag) {
    struct Sprite *happy;
    struct Sprite *poor;
    struct Element *e;
    struct Element *t;
    struct Element *u;
    struct Element **list;
    char **names;
    struct Sprite **icons;
    struct Element *result;
    int count;
    int n;
    int c;
    int i;
    int j;
    int k;
    int swapped;
    int r;

    count = LLIDB_GetCount();
    c = 0;
    // STRING: LEGOLAND 0x004b7a84
    happy = LoadSprite("happy.lls", 0);
    poor = LoadSprite("poor.lls", 0);
    for (i = 0; i < count; i++) {
        LLIDB_GetElement(i, &e);
        if (e->flags & mask) {
            c++;
        }
    }
    names = malloc(c * sizeof(*names) + sizeof(*names));
    list = malloc(c * sizeof(*list));
    icons = malloc(c * sizeof(*icons));
    n = 0;
    for (k = 0; k < count; k++) {
        LLIDB_GetElement(k, &e);
        if (e->flags & mask) {
            list[n] = e;
            n++;
        }
    }
    names[n] = 0;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = n - 2; j >= i; j--) {
            if (_strcmpi(list[j + 1]->name, list[j]->name) < 0) {
                t = list[j];
                u = list[j + 1];
                list[j + 1] = t;
                list[j] = u;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
    for (k = 0; k < n; k++) {
        names[k] = list[k]->name;
        if (list[k]->flags & 1) {
            icons[k] = happy;
        } else {
            icons[k] = poor;
        }
    }
    r = ListBoxDialog(names, title, bg, box, 0, icons, 0x2e, 0x28, flag);
    if (r != -1) {
        result = list[r];
    } else {
        result = 0;
    }
    free(names);
    free(list);
    free(icons);
    KillSprite(happy);
    KillSprite(poor);
    return result;
}

// FUNCTION: LEGOLAND 0x0043f0b0
char *FileSelectDialog(char *title, struct Sprite *bg, RECT *box, char *path) {
    struct Sprite *drive;
    struct Sprite *folder;
    struct Sprite *file;
    char cwd[260];
    char drv[3];
    char dir[256];
    char fname[256];
    char ext[256];
    char spec[260];
    struct _finddata_t fd;
    struct FileNode *head;
    struct FileNode *tail;
    struct FileNode *node;
    struct FileNode **nodes;
    char **names;
    struct Sprite **icons;
    char *result;
    long h;
    int count;
    int chdirFlag;
    int i;
    int j;
    int swapped;
    int dirDiff;
    int r;

    tail = 0;
    // STRING: LEGOLAND 0x004b7ab0
    drive = LoadSprite("Drive.bmp", 0);
    // STRING: LEGOLAND 0x004b7aa4
    folder = LoadSprite("Folder.bmp", 0);
    // STRING: LEGOLAND 0x004b7a98
    file = LoadSprite("File.bmp", 0);
    _getcwd(cwd, 260);
    _splitpath(path, drv, dir, fname, ext);
    _chdir(dir);
    // STRING: LEGOLAND 0x004b7a90
    sprintf(spec, "%s%s", fname, ext);
    chdirFlag = 0;
    count = 0;
    h = _findfirst(spec, &fd);
    while (h != -1) {
        /* build a linked list of the directory entries */
        do {
            if (tail) {
                tail->next = malloc(sizeof(struct FileNode));
                tail = tail->next;
            } else {
                head = malloc(sizeof(struct FileNode));
                tail = head;
            }
            count++;
            tail->name = malloc(strlen(fd.name) + 1);
            strcpy(tail->name, fd.name);
            tail->attrib = fd.attrib;
        } while (_findnext(h, &fd) != -1);
        _findclose(h);
        tail->next = 0;
        names = malloc(count * sizeof(*names) + sizeof(*names));
        icons = malloc(count * sizeof(*icons));
        nodes = malloc(count * sizeof(*nodes));
        names[count] = 0;
        node = head;
        for (i = 0; i < count; i++) {
            nodes[i] = node;
            node = node->next;
        }
        /* bubble sort: folders first, then by name */
        for (i = 0; i < count - 1; i++) {
            swapped = 0;
            for (j = count - 2; j >= i; j--) {
                dirDiff = ((nodes[j]->attrib ^ nodes[j + 1]->attrib) >> 4) & 1;
                if ((dirDiff && (nodes[j + 1]->attrib & 0x10)) ||
                    (!dirDiff && _strcmpi(nodes[j + 1]->name, nodes[j]->name) < 0)) {
                    node = nodes[j + 1];
                    nodes[j + 1] = nodes[j];
                    nodes[j] = node;
                    swapped = 1;
                }
            }
            if (!swapped) {
                break;
            }
        }
        for (i = 0; i < count; i++) {
            names[i] = nodes[i]->name;
            if (nodes[i]->attrib & 0x10) {
                icons[i] = folder;
            } else {
                icons[i] = file;
            }
        }
        r = ListBoxDialog(names, title, bg, box, 0, icons, 0x1c, 0x18, 0);
        if (r != -1) {
            if (nodes[r]->attrib & 0x10) {
                chdirFlag = 1;
            }
            result = FileDialogSelectedName;
            strcpy(result, names[r]);
        } else {
            result = 0;
        }
        free(names);
        free(icons);
        for (i = 0; i < count; i++) {
            free(nodes[i]);
        }
        free(nodes);
        if (!chdirFlag) {
            KillSprite(drive);
            KillSprite(folder);
            KillSprite(file);
            _chdir(cwd);
            return result;
        }
        _chdir(FileDialogSelectedName);
        chdirFlag = 0;
        tail = 0;
        count = 0;
        h = _findfirst(spec, &fd);
    }
    return 0;
}

// FUNCTION: LEGOLAND 0x0043f460
int DialogTextInput(RECT *rc, int unused, char *buf, int maxlen, int *pos) {
    char c;

    c = GetInputChar();
    if (c != 0) {
        switch (c) {
        case (char)0xfd:
            return 1;
        case (char)0xfe:
            return 0;
        case (char)0xff:
            if (*pos > 0) {
                (*pos)--;
            }
            buf[*pos] = 0;
            break;
        default:
            if (*pos < maxlen - 1) {
                buf[*pos] = c;
                (*pos)++;
                buf[*pos] = 0;
            }
            break;
        }
    }
    PrintLimitedText(rc->left + 4, rc->top + 4, rc->right - rc->left - 8, buf, 2, 0, 0);
    return 0;
}

// FUNCTION: LEGOLAND 0x0043f4f0
int RunTextInputDialog(struct Sprite *bg, RECT *box, char *title, char *buf, int maxlen) {
    int pos;
    RECT rc;
    int done;

    pos = strlen(buf);
    rc.left = box->left + 8;
    rc.top = box->top + 0x20;
    rc.right = box->right + box->left - 9;
    rc.bottom = box->top + box->bottom - 9;
    done = 0;
    while (!done && ProcessSystemEvents()) {
        ReadGameButtons();
        PushRenderingStatusAndLockVideoSurface();
        PrintSprite(bg, 0, 0, 0, 0);
        RenderBlock(box->left, box->top, box->right, box->bottom, GetNearestColour(0xef, 0xef, 0xef));
        RenderThickBox(box->left, box->top, box->right, box->bottom, 2, 0);
        RenderBlock(box->left + 2, box->top + 2, box->right - 4, 0x18, GetNearestColour(0, 0x3f, 0x7f));
        PrintLimitedText(box->left + 2, box->top + 2, box->right - 4, title, 0, 0xefefef, 0);
        RenderThickBox(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 2, 0);
        done = DialogTextInput(&rc, 0, buf, maxlen, &pos);
        RenderingComplete();
        PopRenderingStatus();
    }
    return pos;
}
