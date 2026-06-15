#pragma once

void AVIFileInit(void);
int AVIFileOpenA(void **file, const char *name, int mode, void *handler);
int AVIFileInfoA(void *file, void *info, int size);
int AVIFileGetStream(void *file, void **stream, unsigned int type, int n);
int AVIStreamInfoA(void *stream, void *info, int size);
void AVIStreamAddRef(void *stream);
void AVIFileRelease(void *file);
void *AVIStreamGetFrameOpen(void *stream, void *format);
void *AVIStreamGetFrame(void *getframe, int position);
void AVIStreamGetFrameClose(void *frame);
void AVIStreamRelease(void *stream);
void AVIFileExit(void);
void AVIStreamReadFormat(void *stream, int pos, void *format, int *size);
int AVIStreamStart(void *stream);
int AVIStreamLength(void *stream);
int __stdcall acmStreamOpen(void **stream, void *driver, void *src_format, void *dst_format, void *filter, unsigned int callback, unsigned int instance, unsigned int flags);
void __stdcall acmStreamSize(void *stream, unsigned int input_size, unsigned int *output_size, unsigned int flags);
void __stdcall acmStreamClose(void *stream, unsigned int flags);
