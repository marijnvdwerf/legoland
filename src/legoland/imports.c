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
void DirectDrawCreate(void) { STUB(); }

// STUB: LEGOLAND 0x0049d31a
void Ordinal_1(void) { STUB(); }

// STUB: LEGOLAND 0x0049d320
void DirectInputCreateA(void) { STUB(); }

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
int __stdcall acmStreamOpen(void *phas, void *had, void *pwfxSrc, void *pwfxDst, void *pwfltr, unsigned int dwCallback, unsigned int dwInstance, unsigned int fdwOpen) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e3c4
void __stdcall acmStreamUnprepareHeader(void *has, void *hdr, unsigned int flags) { STUB(); }

// STUB: LEGOLAND 0x0049e3ca
void __stdcall acmStreamConvert(void *has, void *pash, unsigned int fdwConvert) { STUB(); }

// STUB: LEGOLAND 0x0049e3d0
void __stdcall acmStreamPrepareHeader(void *has, void *pash, unsigned int fdwPrepare) { STUB(); }

// STUB: LEGOLAND 0x0049e3d6
void AVIFileExit(void) { STUB(); }

// STUB: LEGOLAND 0x0049e3dc
void AVIFileRelease(void *file) { STUB(); }

// STUB: LEGOLAND 0x0049e3e2
void AVIStreamRelease(void *stream) { STUB(); }

// STUB: LEGOLAND 0x0049e3e8
void AVIStreamAddRef(void *stream) { STUB(); }

// STUB: LEGOLAND 0x0049e3ee
int AVIStreamInfoA(void *stream, void *info, int size) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e3f4
int AVIFileGetStream(void *file, void **stream, unsigned int type, int n) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e3fa
int AVIFileInfoA(void *file, void *info, int size) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e400
int AVIFileOpenA(void **file, const char *name, int mode, void *handler) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e406
void AVIFileInit(void) { STUB(); }

// STUB: LEGOLAND 0x0049e40c
void AVIStreamGetFrameClose(void *frame) { STUB(); }

// STUB: LEGOLAND 0x0049e412
void *AVIStreamGetFrameOpen(void *stream, void *format) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e418
void *AVIStreamGetFrame(void *getframe, int position) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e41e
void AVIStreamReadFormat(void *stream, int pos, void *format, int *size) { STUB(); }

// STUB: LEGOLAND 0x0049e424
int AVIStreamLength(void *stream) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e42a
int AVIStreamStart(void *stream) { STUB(); return 0; }

// STUB: LEGOLAND 0x0049e430
void AVIStreamRead(void *stream, int start, int samples, void *buffer, int buffer_size, int *bytes, int *samples_out) { STUB(); }

// STUB: LEGOLAND 0x0049e442
void EnumPrintersA(void) { STUB(); }

// STUB: LEGOLAND 0x0049e448
void FUN_0049e448(void) { STUB(); }
