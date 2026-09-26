#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "legoland.h"

#include "binv.h"

// FUNCTION: LEGOLAND 0x0044dc90
LEGO_EXPORT BinVFile *LoadBinV(const char *filename) {
    FILE *stream;
    unsigned int size;
    union {
        BinVFile *file;
        unsigned int base;
    } mem;
    BinVFile *file;
    BinVFrame *frame;
    BinVObject *object;
    int i;
    int j;

    // STRING: LEGOLAND 0x004b81b8
    stream = fopen(filename, "rb");
    if (stream != NULL) {
        fseek(stream, 0, SEEK_END);
        size = ftell(stream);
        fseek(stream, 0, SEEK_SET);
        mem.file = malloc(size);
        file = mem.file;
        fread(file, 1, size, stream);
        fclose(stream);
        if (file->magic != 0x101) {
            free(file);
        } else {
            file->frames_ofs += mem.base;
            frame = file->frames;
            for (i = 0; i < file->frameCount; i++) {
                frame->objects_ofs += mem.base;
                object = frame->objects;
                for (j = 0; j < frame->count; j++) {
                    object->vertices_ofs += mem.base;
                    object->name_ofs += mem.base;
                    if (object->next != NULL) {
                        object->next_ofs += mem.base;
                        object = object->next;
                    }
                }
                if (frame->next != NULL) {
                    frame->next_ofs += mem.base;
                    frame = frame->next;
                }
            }
            return file;
        }
    }
    return NULL;
}

// FUNCTION: LEGOLAND 0x0044dd60
LEGO_EXPORT void FreeBinV(BinVFile *binv) {
    if (binv != NULL) {
        free(binv);
    }
}

// FUNCTION: LEGOLAND 0x0044dd70
LEGO_EXPORT BinVFrame *GetBinVFrame(BinVFile *file, int index) {
    BinVFrame *frame;
    int remaining;

    if (file == NULL) {
        return NULL;
    }
    frame = file->frames;
    if (index >= file->frameCount) {
        return NULL;
    }
    remaining = file->frameCount - 1;
    if (remaining > index) {
        remaining = remaining - index;
        do {
            frame = frame->next;
            remaining = remaining - 1;
        } while (remaining != 0);
    }
    return frame;
}

// FUNCTION: LEGOLAND 0x0044dda0
LEGO_EXPORT BinVObject *GetObjectFromName(BinVFrame *frame, const char *name) {
    BinVObject *object;
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
LEGO_EXPORT Vertex *GetVertex(BinVObject *object, int index) {
    if (object == NULL) {
        return NULL;
    }
    if (index >= object->count) {
        return NULL;
    }
    return object->vertices + index;
}

// FUNCTION: LEGOLAND 0x0044de20
LEGO_EXPORT float GetZSkew(BinVFile *file, BinVObject *object, Vertex *vertex) {
    return vertex->depth * vertex->depth / ((file->field_14 + file->field_14 - 1.0f) * vertex->depth - vertex->field_c * file->field_14);
}

// FUNCTION: LEGOLAND 0x0044de50
LEGO_EXPORT float GetUnitDepth(float near_z, float far_z) {
    float scale = 49152.0f / (near_z - far_z);
    float a = scale * (2.0 - far_z) + 8192.0;
    float b = scale * (1.0 - far_z) + 8192.0;

    return a - b;
}
