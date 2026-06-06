#pragma once

struct LLS {
    short frame;
    unsigned short delay;
    unsigned char pad_4[0x10 - 0x4];
    short frame_count;
    short loop_delay;
    unsigned int flags;
};

int LLIDB_FindElement(const char *name, unsigned int *out, int zero);
unsigned int ElemID(const char *name);
void LLIDB_ClearOnLevel(void);
void *LLIDB_LoadData(void *head);
void LLIDB_UnLoadData(unsigned int handle);

void LLSStop(unsigned int handle);
void LLSSetFrame(struct LLS *lls, int index);

unsigned int SaveGameRead(void *buffer, unsigned int count);
unsigned int SaveGameWrite(void *buffer, unsigned int count);
