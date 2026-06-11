#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (stream == NULL) {
        return NULL;
    }

    fseek(stream, 0, 2);
    size = ftell(stream);
    fseek(stream, 0, 0);
    file = (struct BinVFile *)malloc(size);
    fread(file, 1, size, stream);
    fclose(stream);

    if (file->magic != 0x101) {
        free(file);
        return NULL;
    }

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
            return object;
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
LEGO_EXPORT void GetZSkew(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0044de50
LEGO_EXPORT void GetUnitDepth(void) { STUB(); }
