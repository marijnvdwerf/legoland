#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"

struct Vertex {
    unsigned char data[20];
};

struct BinVObject {
    int count;
    struct BinVObject *next;
    struct Vertex *vertices;
    char *name;
};

struct BinVFrame {
    int count;
    struct BinVObject *objects;
    struct BinVFrame *next;
};

struct BinVFile {
    short magic;
    unsigned short frameCount;
    unsigned char pad_4[0x1c];
    struct BinVFrame *frames;
};

// FUNCTION: LEGOLAND 0x0044dc90
LEGO_EXPORT struct BinVFile *LoadBinV(const char *filename) {
    void *stream;
    unsigned int size;
    struct BinVFile *file;
    struct BinVFrame *frame;
    struct BinVObject *object;
    int i;
    int j;

    // STRING: LEGOLAND 0x004b81b8
    stream = fopen(filename, "rb");
    if (stream != NULL) {
        fseek(stream, 0, 2);
        size = ftell(stream);
        fseek(stream, 0, 0);
        file = (struct BinVFile *)malloc(size);
        fread(file, 1, size, stream);
        fclose(stream);
        if (file->magic != 0x101) {
            free(file);
        } else {
            file->frames = (struct BinVFrame *)((int)file->frames + (int)file);
            frame = file->frames;
            for (i = 0; i < file->frameCount; i++) {
                object = (struct BinVObject *)((int)frame->objects + (int)file);
                frame->objects = object;
                for (j = 0; j < frame->count; j++) {
                    object->vertices = (struct Vertex *)((int)object->vertices + (int)file);
                    object->name = (char *)((int)object->name + (int)file);
                    if (object->next != NULL) {
                        object->next = (struct BinVObject *)((int)object->next + (int)file);
                        object = object->next;
                    }
                }
                if (frame->next != NULL) {
                    frame->next = (struct BinVFrame *)((int)frame->next + (int)file);
                    frame = frame->next;
                }
            }
            return file;
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0044dd60
LEGO_EXPORT void FreeBinV(void *binv) {
    if (binv != NULL) {
        free(binv);
    }
}

// FUNCTION: LEGOLAND 0x0044dd70
LEGO_EXPORT struct BinVFrame *GetBinVFrame(struct BinVFile *file, int index) {
    struct BinVFrame *frame;
    int remaining;

    if (file == NULL) {
        return NULL;
    }
    frame = file->frames;
    if (index >= file->frameCount) {
        return NULL;
    }
    remaining = file->frameCount - 1;
    if (index < remaining) {
        remaining = remaining - index;
        do {
            frame = frame->next;
            remaining = remaining - 1;
        } while (remaining != 0);
    }
    return frame;
}

// FUNCTION: LEGOLAND 0x0044dda0
LEGO_EXPORT struct BinVObject *GetObjectFromName(struct BinVFrame *frame, const char *name) {
    struct BinVObject *object;
    int i;

    if (frame == NULL) {
        return NULL;
    }
    object = frame->objects;
    for (i = 0; i < frame->count; i++) {
        if (_stricmp(name, object->name) == 0) {
            break;
        }
        object = object->next;
    }
    return object;
}

// FUNCTION: LEGOLAND 0x0044ddf0
LEGO_EXPORT struct Vertex *GetVertex(struct BinVObject *object, int index) {
    if (object == NULL) {
        return NULL;
    }
    if (index >= object->count) {
        return NULL;
    }
    return object->vertices + index;
}

// FUNCTION: LEGOLAND 0x0044de20
LEGO_EXPORT double GetZSkew(struct BinVFile *file, struct BinVObject *object, struct Vertex *vertex) {
    float *v = (float *)vertex;
    float *f = (float *)file;

    return v[4] * v[4] / ((f[5] + f[5] - DAT_004ab38c) * v[4] - v[3] * f[5]);
}

// FUNCTION: LEGOLAND 0x0044de50
LEGO_EXPORT float GetUnitDepth(float near_z, float far_z) {
    float scale = DAT_004ab4d8 / (near_z - far_z);
    float a = scale * (DOUBLE_004ab460 - far_z) + DOUBLE_004ab4d0;
    float b = scale * (DAT_004ab3a8 - far_z) + DOUBLE_004ab4d0;

    return a - b;
}
