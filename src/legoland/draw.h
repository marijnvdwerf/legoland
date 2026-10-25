#pragma once

#include "legoland.h"

#include <windows.h>

union RectPoints {
    RECT rect;
    POINT pt[2];
};

struct VideoArg {
    /* 0x00 */ int pitch;
    /* 0x04 */ int width;
    /* 0x08 */ int height;
    /* 0x0c */ void *bits;
    /* 0x10 */ struct IDirectDrawSurface *surface; /* GetSprite: the locked sprite surface */
    /* 0x14 */ int format; /* 1: 8-bit, 2: 16-bit */
};
struct Sprite;

struct AviFrame {
    /* 0x00 */ unsigned char pad_0[4];
    /* 0x04 */ int width;
    /* 0x08 */ int height;
    /* 0x0c */ unsigned char pad_c[0x1c];
    /* 0x28 */ unsigned short pixels[1];
};

LEGO_EXPORT int InitHostSystemGPU(void);
LEGO_EXPORT void KillHostSystemGPU(void);
LEGO_EXPORT int InitScreen(void);
LEGO_EXPORT unsigned int SetPointer(unsigned int param_1);
LEGO_EXPORT void PushRenderingStatusAndLockVideoSurface(void);
LEGO_EXPORT void PushRenderingStatusAndUnlockVideoSurface(void);
LEGO_EXPORT void PopRenderingStatus(void);
LEGO_EXPORT void PrintBackground(int x, int y);
LEGO_EXPORT int GetVideoSurface(struct VideoArg *arg);
LEGO_EXPORT void SetOverridePalette(unsigned short *param_1);
LEGO_EXPORT void SetOverrideFrame(unsigned int param_1);
LEGO_EXPORT unsigned short *GetOverridePalette(void);
LEGO_EXPORT unsigned int GetOverrideFrame(void);
LEGO_EXPORT void ClearSpriteOverrides(void);
LEGO_EXPORT void ZBufferHelper(unsigned int *param_1, int *param_2, int *param_3, void *param_4);
LEGO_EXPORT void ClearOverrideFrame(void);
LEGO_EXPORT void ClearOverridePalette(void);
void DrawMovieFrameDoubled(struct AviFrame *frame);
void DrawAviFrame(struct AviFrame *param_1, int param_2, int param_3);
void LoadWatchSprite(int a, int b);
void UnloadWatchSprite(void);
LEGO_EXPORT void CommitCliprectToHardware(void);
LEGO_EXPORT int RenderingComplete(void);
LEGO_EXPORT void PushSetTarget(struct Sprite *sprite);
LEGO_EXPORT void PopTarget(void);
LEGO_EXPORT int RecreateSprite(struct Sprite *sprite);
void SetBriefingFile(const char *param_1);
LEGO_EXPORT int CheckHostSystemGPU(void);
int BlitFrameToWindow(void);
void __fastcall SoftPrint_BltFast(struct Sprite *sprite, RECT *rect, int *off);
LEGO_EXPORT void SoftPrint_Clear(void);
LEGO_EXPORT void SoftPrint_XBltFast(struct Sprite *sprite, RECT *a, RECT *b, unsigned int param_4);
void DrawWatchSprite(void);
int SetDisplayModeAndDetectPixelFormat(void);

struct DrawLLS {
    short frame;
    unsigned short delay;
    int width;
    int height;
    unsigned int field_c;
    short frame_count;
    short loop_delay;
    unsigned int flags;
};

struct DrawLLSFrame {
    unsigned int size;
    unsigned int pixel_count;
    unsigned int run_bytes;
    unsigned int field_c;
    unsigned short pixels[1];
};

struct Point;
void DrawLLSSpriteMousePick(struct DrawLLS *lls, RECT *clip, struct Point *pos);
void BlitLLS16PickClipLeftRight(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16PickClipLeft(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16PickClipRight(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16Pick(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16ClipLeftRight(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16ClipLeft(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16ClipRight(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void BlitLLS16(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip);
void DrawLLSSpriteTinted(struct DrawLLS *lls, RECT *clip, struct Point *pos);
void BlitLLS16MaskedPick(unsigned short *dst, unsigned short *src, unsigned char *runs, unsigned int *mask, int h, int stride, int skip, int left, int width, int flags, unsigned short *cursor);
void SoftPrint_IndexedLLS(void);
void SoftPrint_IndexedLLSMasked(void);
void BlitLLS16HalfBright(void);
