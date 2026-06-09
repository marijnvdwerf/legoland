#include <windows.h>
#include "legoland.h"

#include "crt.h"
#include "timer.h"
#include "string.h"
#include "globals.h"

struct StringNode {
    int key;
    char *text;
    struct StringNode *next;
};

// FUNCTION: LEGOLAND 0x00498f50
LEGO_EXPORT char *GetString(int n) {
    struct StringNode *node = (struct StringNode *)strings[n % 10];
    while (node != NULL) {
        if (node->key == n) {
            return node->text;
        }
        node = node->next;
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x00498f80
void FUN_00498f80(const char *text, int key) {
    struct StringNode *node = (struct StringNode *)malloc(sizeof(struct StringNode));
    int index = key % 10;
    node->next = (struct StringNode *)strings[index];
    strings[index] = node;
    node->text = (char *)malloc(strlen(text) + 1);
    memcpy(node->text, text, strlen(text) + 1);
    node->key = key;
}

// FUNCTION: LEGOLAND 0x00498ff0
LEGO_EXPORT void DeleteStrings(void) {
    void **slot;
    struct StringNode *node;
    struct StringNode *next;

    next = NULL;
    slot = (void **)strings;
    do {
        node = (struct StringNode *)*slot;
        if (node != NULL) {
            next = node->next;
        }
        while (next != NULL) {
            void *text = node->text;
            next = node->next;
            free(text);
            free(node);
            node = next;
        }
        *slot = NULL;
        slot++;
    } while ((int)slot < (int)DAT_0079a878);
}

// FUNCTION: LEGOLAND 0x00499040
void FUN_00499040(const char *path, char *directory, char *filename) {
    int len = strlen(path);
    int dot = len;
    int i = len;
    int j;
    int k;

    while (path[i] != '\\') {
        if (i <= 0) {
            break;
        }
        if (path[i] == '.') {
            dot = i;
        }
        i--;
    }

    j = i + 1;
    while (j < dot) {
        *filename = path[j];
        filename++;
        j++;
    }
    *filename = '\0';

    if (i <= 0) {
        *directory = '\0';
        return;
    }
    k = 0;
    while (k < i) {
        *directory = path[k];
        directory++;
        k++;
    }
    *directory = '\0';
}

// FUNCTION: LEGOLAND 0x004990c0
void FUN_004990c0(char *path, char *ext)
{
    int i = strlen(path);
    while (path[i] != '.' && path[i] != '\\' && i > 0) {
        i--;
    }
    if (path[i] != '.') {
        strcat(path, ext);
    }
}

// FUNCTION: LEGOLAND 0x00499120
void FUN_00499120(char *path, char *ext)
{
    int i = strlen(path);
    while (path[i] != '.' && path[i] != '\\' && i > 0) {
        i--;
    }
    if (path[i] == '.') {
        path[i] = '\0';
    }
    strcat(path, ext);
}

// FUNCTION: LEGOLAND 0x00499190
void FUN_00499190(char *path, char *dir)
{
    char tmp[200];
    int i = strlen(path);
    while (path[i] != '\\' && path[i] != ':' && i > 0) {
        i--;
    }
    if (i == 0) {
        strcpy(tmp, path);
        strcpy(path, dir);
        strcat(path, tmp);
    }
}

// FUNCTION: LEGOLAND 0x00499240
void FUN_00499240(char *path, char *dir)
{
    char tmp[200];
    int i = strlen(path);
    while (path[i] != '\\' && path[i] != ':' && i > 0) {
        i--;
    }
    if (i != 0) {
        i++;
    }
    strcpy(tmp, dir);
    strcat(tmp, path + i);
    strcpy(path, tmp);
}

// FUNCTION: LEGOLAND 0x00499300
int FUN_00499300(char *str) {
    int i = 0;

    str[0] = (char)toupper(str[0]);
    if (str[0] != '\0') {
        do {
            int c = str[i + 1];
            i++;
            str[i] = (char)toupper(c);
        } while (str[i] != '\0');
    }
    return i;
}

// FUNCTION: LEGOLAND 0x00499380
unsigned int FUN_00499380(void) {
    if (DAT_0079a890 == 0) {
        DAT_0079a890 = 1;
        DAT_0079a894 = GetTickCount();
        DAT_0079a89c = DAT_008119a4;
        return 0;
    }
    return 1;
}

// FUNCTION: LEGOLAND 0x004993c0
void FUN_004993c0(void) {
    if (DAT_0079a890 == 0) {
        return;
    }
    DAT_0079a890 = 0;
    DAT_0079a898 += GetTickCount() - DAT_0079a894;
    DAT_0079a8a0 += DAT_008119a4 - DAT_0079a89c;
}

// FUNCTION: LEGOLAND 0x00499410
void FUN_00499410(void) {
    unsigned int ticks;
    unsigned int counter;

    ticks = GetTicks();
    DAT_0079a894 = ticks;
    DAT_0079a898 = ticks;
    counter = DAT_008119a4;
    DAT_0079a89c = counter;
    DAT_0079a8a0 = counter;
}
