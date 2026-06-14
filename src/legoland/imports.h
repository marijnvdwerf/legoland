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
