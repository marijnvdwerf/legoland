#include <windows.h>
#include <ctype.h>
#include "legoland.h"

#include "crt.h"
#include "timer.h"
#include "string.h"
#include "globals.h"

#pragma intrinsic(memset)
// FUNCTION: LEGOLAND 0x00498d00
void FUN_00498d00(void) {
    char cwd[256];
    char token[240];
    int current_id;
    char num_str[4];
    char *data_buf;
    int file_size;
    FILE *stream;
    int total;
    int offset;
    int len;
    char c;
    int quote_count;

    memset(token, 0, sizeof(token));
    memset(num_str, 0, sizeof(num_str));
    total = 0;
    offset = 0;

    if (_getcwd(cwd, 256) == 0) {
        return;
    }

    // STRING: LEGOLAND 0x004bfef4
    stream = fopen(".\\strings\\stab.str", "r");
    if (stream == 0) {
        exit(1);
    }

    while (!feof(stream)) {
        len = fread(token, 1, 0xf0, stream);
        if (ferror(stream)) {
            break;
        }
        total += len;
    }
    file_size = total;

    data_buf = (char *)malloc(file_size + 1);
    fseek(stream, 0, 0);
    fread(data_buf, 1, file_size, stream);
    fclose(stream);

    if (_chdir(cwd) != 0) {
        return;
    }

    while (offset < file_size) {
        c = data_buf[offset];
        offset++;
        if (c == 0) {
            break;
        }
        if (isdigit(c)) {
            len = 0;
            while (isdigit(c)) {
                num_str[len] = c;
                len++;
                if (offset < file_size) {
                    c = data_buf[offset];
                    offset++;
                } else {
                    c = 0;
                }
            }
            if (offset != 0) {
                offset--;
            }
            num_str[len] = 0;
            current_id = atoi(num_str);
        } else if ((isalpha(c) | ispunct(c)) != 0) {
            len = 0;
            quote_count = 0;
            while (1) {
                if (c == '"') {
                    if (offset < file_size) {
                        c = data_buf[offset];
                        offset++;
                        if (c == '"') {
                            goto store;
                        }
                    } else {
                        c = 0;
                    }
                    quote_count++;
                }
            store:
                if (quote_count == 2) {
                    break;
                }
                token[len] = c;
                len++;
                if (offset < file_size) {
                    c = data_buf[offset];
                    offset++;
                } else {
                    c = 0;
                }
            }
            token[len] = 0;
            FUN_00498f80(token, current_id);
        }
    }
    free(data_buf);
}

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
