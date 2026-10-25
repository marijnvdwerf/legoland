#pragma once

void __stdcall AVIFileInit(void);
int __stdcall AVIFileOpenA(void **file, const char *name, int mode, void *handler);
int __stdcall AVIFileInfoA(void *file, void *info, int size);
int __stdcall AVIFileGetStream(void *file, void **stream, unsigned int type, int n);
int __stdcall AVIStreamInfoA(void *stream, void *info, int size);
unsigned int __stdcall AVIStreamAddRef(void *stream);
unsigned int __stdcall AVIFileRelease(void *file);
void *__stdcall AVIStreamGetFrameOpen(void *stream, void *format);
void *__stdcall AVIStreamGetFrame(void *getframe, int position);
int __stdcall AVIStreamGetFrameClose(void *frame);
unsigned int __stdcall AVIStreamRelease(void *stream);
void __stdcall AVIFileExit(void);
int __stdcall AVIStreamReadFormat(void *stream, int pos, void *format, int *size);
int __stdcall AVIStreamStart(void *stream);
int __stdcall AVIStreamLength(void *stream);
int __stdcall acmStreamOpen(void **stream, void *driver, void *src_format, void *dst_format, void *filter, unsigned int callback, unsigned int instance, unsigned int flags);
int __stdcall acmStreamSize(void *stream, unsigned int input_size, unsigned int *output_size, unsigned int flags);
void __stdcall acmStreamClose(void *stream, unsigned int flags);
int __stdcall acmStreamPrepareHeader(void *has, void *pash, unsigned int fdwPrepare);
int __stdcall acmStreamConvert(void *has, void *pash, unsigned int fdwConvert);
void __stdcall acmStreamUnprepareHeader(void *has, void *hdr, unsigned int flags);
int __stdcall AVIStreamRead(void *stream, int start, int samples, void *buffer, int buffer_size, int *bytes, int *samples_out);
#ifdef __LP64__
/* 64-bit Linux: the SDK's DWORD is 32 bits there, and unsigned long 64 */
int __stdcall EnumPrintersA(unsigned int flags, char *name, unsigned int level, unsigned char *buf, unsigned int cb, unsigned int *needed, unsigned int *returned);
#else
int __stdcall EnumPrintersA(unsigned long flags, char *name, unsigned long level, unsigned char *buf, unsigned long cb, unsigned long *needed, unsigned long *returned);
#endif
