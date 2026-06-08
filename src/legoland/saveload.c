#include "legoland.h"

#include "timer.h"
#include "saveload.h"
#include "llidb.h"
#include "globals.h"

// FUNCTION: LEGOLAND 0x0047d790
LEGO_EXPORT void BeginMeasuredBlock(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d7e0
void FUN_0047d7e0(void) {
    unsigned int buffer;

    SaveGameRead(&buffer, 4);
}

// FUNCTION: LEGOLAND 0x0047d800
LEGO_EXPORT void EndMeasuredBlock(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d880
LEGO_EXPORT void FindeIneList(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047d8c0
LEGO_EXPORT unsigned int GeteListPtr(int idx) {
    if (idx == -1) {
        return 0;
    }
    return DAT_00669200[idx];
}

// FUNCTION: LEGOLAND 0x0047d8e0
LEGO_EXPORT int SaveGame(char *filename) { STUB(); }

// FUNCTION: LEGOLAND 0x0047e980
LEGO_EXPORT void LoadGame(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047f760
LEGO_EXPORT void UnloadSaveGameMap(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047f810
void FUN_0047f810(void) {
    DAT_00669204 = GetGameTimer();
}

// FUNCTION: LEGOLAND 0x0047f820
unsigned int FUN_0047f820(void) {
    return GetGameTimer() - DAT_00669204;
}

// FUNCTION: LEGOLAND 0x0047f830
unsigned int FUN_0047f830(const char *path) {
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
