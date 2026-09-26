#pragma once

void __stdcall AVIFileInit(void);
int __stdcall AVIFileOpenA(void **file, const char *name, int mode, void *handler);
int __stdcall AVIFileInfoA(void *file, void *info, int size);
int __stdcall AVIFileGetStream(void *file, void **stream, unsigned int type, int n);
int __stdcall AVIStreamInfoA(void *stream, void *info, int size);
void __stdcall AVIStreamAddRef(void *stream);
void __stdcall AVIFileRelease(void *file);
void *__stdcall AVIStreamGetFrameOpen(void *stream, void *format);
void *__stdcall AVIStreamGetFrame(void *getframe, int position);
void __stdcall AVIStreamGetFrameClose(void *frame);
void __stdcall AVIStreamRelease(void *stream);
void __stdcall AVIFileExit(void);
void __stdcall AVIStreamReadFormat(void *stream, int pos, void *format, int *size);
int __stdcall AVIStreamStart(void *stream);
int __stdcall AVIStreamLength(void *stream);
int __stdcall acmStreamOpen(void **stream, void *driver, void *src_format, void *dst_format, void *filter, unsigned int callback, unsigned int instance, unsigned int flags);
void __stdcall acmStreamSize(void *stream, unsigned int input_size, unsigned int *output_size, unsigned int flags);
void __stdcall acmStreamClose(void *stream, unsigned int flags);
void __stdcall acmStreamPrepareHeader(void *has, void *pash, unsigned int fdwPrepare);
void __stdcall acmStreamConvert(void *has, void *pash, unsigned int fdwConvert);
void __stdcall acmStreamUnprepareHeader(void *has, void *hdr, unsigned int flags);
void __stdcall AVIStreamRead(void *stream, int start, int samples, void *buffer, int buffer_size, int *bytes, int *samples_out);
