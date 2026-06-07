#include <windows.h>

#include "legoland.h"
#include "globals.h"
#include "debug_alloc.h"

extern void FUN_004661d0(const char *msg);
extern int CheckHostSystemGPU(void);

struct LegoConfig {
    unsigned char gap_0[0x40];
    unsigned int field_40;
};

// FUNCTION: LEGOLAND 0x0047f870
void FUN_0047f870(void) {}

// FUNCTION: LEGOLAND 0x0047f880
int FUN_0047f880(void) { STUB(); }

// FUNCTION: LEGOLAND 0x0047fc20
unsigned int mystrlen(const char *s) {
    if (s == NULL) {
        return 0;
    }
    return strlen(s);
}

// FUNCTION: LEGOLAND 0x0047fc40
char *FUN_0047fc40(const char *haystack, const char *needle) { STUB(); }

// FUNCTION: LEGOLAND 0x0047fd10
int FUN_0047fd10(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HANDLE mutex;
    SECURITY_ATTRIBUTES sa;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = FALSE;
    // STRING: LEGOLAND 0x004bcdb0
    mutex = CreateMutexA(&sa, TRUE, "LegolandGameMutex");
    if (mutex == NULL) {
        // STRING: LEGOLAND 0x004bcd98
        DBPrintf("Couldn't create Mutex\n");
        return 0;
    }

    if (WaitForSingleObject(mutex, 0) == WAIT_TIMEOUT) {
        // STRING: LEGOLAND 0x004bcd7c
        DBPrintf("Program already running.\n");
        CloseHandle(mutex);
        return 0;
    }

    // STRING: LEGOLAND 0x004bcd70
    if (FUN_0047fc40(lpCmdLine, "WINDEBUG")) {
        DAT_004b9ca4 = FUN_004661d0;
        DAT_00667d6c = 1;
    // STRING: LEGOLAND 0x004bcd6c
    } else if (FUN_0047fc40(lpCmdLine, "BLT")) {
        DAT_004b9ca4 = FUN_004661d0;
    }

    // STRING: LEGOLAND 0x004bcd60
    if (FUN_0047fc40(lpCmdLine, "-nointro")) {
        lpConfig->field_40 = 1;
    } else {
        lpConfig->field_40 = 0;
    }

    // STRING: LEGOLAND 0x004bcd54
    DAT_004bf774 = FUN_0047fc40(lpCmdLine, "-nomusic") ? 0 : 1;
    g_hInstance = hInstance;
    DAT_0066920c = nCmdShow;

    if (CheckHostSystemGPU() == 0) {
        return 0;
    }
    return FUN_0047f880();
}
