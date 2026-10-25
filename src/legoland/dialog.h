#pragma once

#include <windows.h>

int DialogSlider(RECT *rc, int min, int max, int value, int step);
struct Sprite;
struct Element;
int ListBoxDialog(char **names, char *title, struct Sprite *bg, RECT *box, void (*callback)(int), struct Sprite **icons, int w, int h, int flag);
struct Element *SelectElementDialog(char *title, struct Sprite *bg, RECT *box, unsigned int mask, int flag);
struct FileNode {
    struct FileNode *next;
    char *name;
    unsigned int attrib;
};
char *FileSelectDialog(char *title, struct Sprite *bg, RECT *box, char *path);
int RunTextInputDialog(struct Sprite *bg, RECT *box, char *title, char *buf, int maxlen);
int DialogTextInput(RECT *rc, int unused, char *buf, int maxlen, int *pos);
