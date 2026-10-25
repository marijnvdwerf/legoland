#pragma once

#include "legoland.h"
#include "llidb.h"

struct JailCell {
    struct JailCell *next;
    unsigned short tile_id;
    unsigned char frame;
    unsigned char pad_7[0x8 - 0x7];
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
};

struct Element;
struct Ride;
struct Cursor;

/* Callback-table entry points for the western-town shops (General Store,
 * Sheriff, Jail Cells, Bank, Saloon), registered by the shops dispatcher
 * (ShopsGetInterfaces). */
void LoadGStoreMatteSpritesAndMoneySFX(struct Element *obj);
void KillGStoreMatteSpritesAndMoneySFX(void);
void GeneralStoreSetEditMode(void);
void RenderGeneralStore(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6);
void GeneralStoreUpdate(struct Element *param_1);
void LoadSherifshutMatteSpriteAndMoneySFX(struct Element *obj);
void KillSherifshutMatteSpriteAndMoneySFX(void);
void SheriffSetEditMode(void);
void RenderSheriff(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6);
void SheriffUpdate(struct Element *param_1);
void JailCellAddObject(struct Element *editObj, int *coords);
void JailCellRemoveObject(struct Element *editObj, TileId coords, struct Cursor *cursor);
void JailCellLoad(struct Element *obj);
void JailCellUnload(void);
void JailCellSetEditMode(void);
void RenderJailCell(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6);
void JailCellUpdate(Element *obj);
LEGO_EXPORT int SaveJailCells(struct Element *element);
LEGO_EXPORT int LoadJailCells(struct Element *element);
void LoadBankMatteSpriteAndMoneySFX(struct Element *obj);
void KillBankMatteSpriteAndMoneySFX(void);
void BankSetEditMode(void);
void RenderBank(struct Element *param_1, unsigned int param_2, unsigned int param_3, unsigned short *param_4, void *param_5, unsigned int param_6);
void BankUpdate(Element *obj);
void LoadSaloonMatteSpritesAndMoneySFX(struct Element *obj);
void KillSaloonMatteSpritesAndMoneySFX(void);
void SaloonSetEditMode(void);
void RenderSaloon(Element *obj, unsigned int param_2, unsigned int param_3, unsigned short *tile, void *param_5, unsigned int param_6);
void SaloonUpdate(Element *obj);
