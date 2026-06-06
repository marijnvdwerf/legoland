#include "legoland.h"

#include "timer.h"
#include "saveload.h"
#include "llidb.h"

extern unsigned int *DAT_00669200;
extern unsigned int DAT_00669204;

// FUNCTION: LEGOLAND 0x0047d790
void BeginMeasuredBlock(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d7e0
void FUN_0047d7e0(void) {
    unsigned int buffer;

    SaveGameRead(&buffer, 4);
}

// FUNCTION: LEGOLAND 0x0047d800
void EndMeasuredBlock(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d880
void FindeIneList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d8c0
unsigned int GeteListPtr(int idx) {
    if (idx == -1) {
        return 0;
    }
    return DAT_00669200[idx];
}

// FUNCTION: LEGOLAND 0x0047d8e0
void SaveGame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047e980
void LoadGame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047f760
void UnloadSaveGameMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047f810
void FUN_0047f810(void) {
    DAT_00669204 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x0047f820
unsigned int FUN_0047f820(void) {
    return GetGameTimer() - DAT_00669204;
}

// FUNCTION: LEGOLAND 0x0047f830
unsigned int FUN_0047f830(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0047f840
int FUN_0047f840(void) {
    return 1;
}

// FUNCTION: LEGOLAND 0x0047f850
void FUN_0047f850(void) {
    return;
}
