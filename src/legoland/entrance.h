#pragma once

// Per-TU header for entrance.c — canonical declarations for the park entrance helpers.

struct EntranceParam {
    unsigned char pad_0[0xc];
    unsigned int field_c;
};

void FUN_0042d970(void);
void FUN_0042d9c0(void);
void FUN_0042de50(void);
void FUN_0042def0(struct EntranceParam *param);
void FUN_0042df70(void);
void FUN_0042dfa0(void);
