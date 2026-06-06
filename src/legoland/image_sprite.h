#pragma once

/* struct Sprite / struct Image are forward-declared only: every prototype below
 * uses them through a pointer. Their full layouts live in image_sprite.c, and
 * several callers (mapscreen.c, timer.c, ...) keep their own divergent
 * struct Sprite views, so the full definition is deliberately NOT moved here. */
struct Sprite;
struct Image;

unsigned int CreateSourceImage(unsigned int a, unsigned int b);
int KillImage(struct Image *image);
void ReloadImageBitmapAndBuildSprites(struct Image *image);
void FreeBitmapResources(struct Image *image);

struct Sprite *CreateFunctionBasedSprite(unsigned int source, unsigned short a, unsigned short b);
unsigned int LoadSprite(const char *name, int flags);
void MakeSprite(unsigned int sprite);
short ReferenceSprite(struct Sprite *sprite);
int KillSprite(unsigned int sprite);

void HideLayer(void *layer, unsigned int flag);

void FUN_00498120(void);
void FUN_00498630(const char *param_1);
int FUN_004988c0(void);
void FUN_00498920(void);
int FUN_00498b00(void);
int FUN_00498cf0(void);
