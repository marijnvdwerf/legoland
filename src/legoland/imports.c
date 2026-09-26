#include "legoland.h"

#include "imports.h"

// STUB: LEGOLAND 0x0049d050
void GetUserNameA(void) { STUB(); }

// STUB: LEGOLAND 0x0049d056
void SetCurrentDirectoryA(void) { STUB(); }

// STUB: LEGOLAND 0x0049d05c
void ReadFile(void) { STUB(); }

// STUB: LEGOLAND 0x0049d062
void CloseHandle(void) { STUB(); }

// STUB: LEGOLAND 0x0049d068
void GetFileSize(void) { STUB(); }

// STUB: LEGOLAND 0x0049d314
int __stdcall DirectDrawCreate(void *guid, void *ddraw, void *unknown) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049d31a
int __stdcall DirectSoundCreate(void *guid, void *dsound, void *unknown) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049d320
int __stdcall DirectInputCreateA(void *hinst, unsigned int version, void *out, void *outer) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e3a0
void VerQueryValueA(void) { STUB(); }

// STUB: LEGOLAND 0x0049e3a6
void GetFileVersionInfoA(void) { STUB(); }

// STUB: LEGOLAND 0x0049e3ac
void GetFileVersionInfoSizeA(void) { STUB(); }

// STUB: LEGOLAND 0x0049e3b2
void __stdcall acmStreamClose(void *has, unsigned int flags) { STUB(); }

// STUB: LEGOLAND 0x0049e3b8
void __stdcall acmStreamSize(void *has, unsigned int cbInput, unsigned int *pdwOutputBytes, unsigned int fdwSize) { STUB(); }

// STUB: LEGOLAND 0x0049e3be
int __stdcall acmStreamOpen(void *phas, void *had, void *pwfxSrc, void *pwfxDst, void *pwfltr, unsigned int dwCallback, unsigned int dwInstance, unsigned int fdwOpen) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e3c4
void __stdcall acmStreamUnprepareHeader(void *has, void *hdr, unsigned int flags) { STUB(); }

// STUB: LEGOLAND 0x0049e3ca
void __stdcall acmStreamConvert(void *has, void *pash, unsigned int fdwConvert) { STUB(); }

// STUB: LEGOLAND 0x0049e3d0
void __stdcall acmStreamPrepareHeader(void *has, void *pash, unsigned int fdwPrepare) { STUB(); }

// STUB: LEGOLAND 0x0049e3d6
void __stdcall AVIFileExit(void) { STUB(); }

// STUB: LEGOLAND 0x0049e3dc
void __stdcall AVIFileRelease(void *file) { STUB(); }

// STUB: LEGOLAND 0x0049e3e2
void __stdcall AVIStreamRelease(void *stream) { STUB(); }

// STUB: LEGOLAND 0x0049e3e8
void __stdcall AVIStreamAddRef(void *stream) { STUB(); }

// STUB: LEGOLAND 0x0049e3ee
int __stdcall AVIStreamInfoA(void *stream, void *info, int size) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e3f4
int __stdcall AVIFileGetStream(void *file, void **stream, unsigned int type, int n) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e3fa
int __stdcall AVIFileInfoA(void *file, void *info, int size) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e400
int __stdcall AVIFileOpenA(void **file, const char *name, int mode, void *handler) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e406
void __stdcall AVIFileInit(void) { STUB(); }

// STUB: LEGOLAND 0x0049e40c
void __stdcall AVIStreamGetFrameClose(void *frame) { STUB(); }

// STUB: LEGOLAND 0x0049e412
void *__stdcall AVIStreamGetFrameOpen(void *stream, void *format) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e418
void *__stdcall AVIStreamGetFrame(void *getframe, int position) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e41e
void __stdcall AVIStreamReadFormat(void *stream, int pos, void *format, int *size) { STUB(); }

// STUB: LEGOLAND 0x0049e424
int __stdcall AVIStreamLength(void *stream) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e42a
int __stdcall AVIStreamStart(void *stream) {
    STUB();
    return 0;
}

// STUB: LEGOLAND 0x0049e430
void __stdcall AVIStreamRead(void *stream, int start, int samples, void *buffer, int buffer_size, int *bytes, int *samples_out) { STUB(); }

// STUB: LEGOLAND 0x0049e442
void EnumPrintersA(void) { STUB(); }

// STUB: LEGOLAND 0x0049e448
void FUN_0049e448(void) { STUB(); }
