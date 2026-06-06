#pragma once

// Per-TU header for castle_level.c — Castle Level interface hooks.

typedef void (*CastleVtblFn)(void);

struct CastleLevelInterface {
    unsigned char pad_0[0x8c];
    CastleVtblFn field_8c;
    CastleVtblFn field_90;
    CastleVtblFn field_94;
    CastleVtblFn field_98;
    CastleVtblFn field_9c;
    CastleVtblFn field_a0;
    CastleVtblFn field_a4;
    CastleVtblFn field_a8;
    CastleVtblFn field_ac;
    CastleVtblFn field_b0;
};

void FUN_00402ca0(void);
void FUN_00402ce0(void);
void FUN_00402d00(void);
void FUN_00402dc0(void);
void FUN_00402ff0(void);
void FUN_00403030(void);
void FUN_00403060(unsigned int param1, unsigned int param2);
void FUN_00403080(const char **name, struct CastleLevelInterface *ci);
