#pragma once

#include <windows.h>

#include "legoland.h"

struct NewObjInfo;

/* what the info popup is about (0x7fdec0; passed by value to PopUpInfoSetUp) */
// 0x007fdec0
extern struct HoverInfo PopUpInfoHover;

LEGO_EXPORT void InitPopUpInfo(void);
LEGO_EXPORT void PopInfoSizeMayChange(void);
void ClearNewObjectsList(void);
void ShowNewObjectsInfo(void);
void AddNewObjectIcon(struct NewObjInfo *param_1);
LEGO_EXPORT int UnLoad_PopUpInfo(void);
void RemoveAllNewObjects(void);
LEGO_EXPORT void ResetInfoStruct(void);
unsigned int TryClosePopUp(void);
unsigned int HandlePopUpOkKey(void);
void ClearAdvicePriority(void);
void UpdateAdvicePriority(void);

LEGO_EXPORT void InfoPrintCent(int len, char *text, int font, RECT rc, int flag);
LEGO_EXPORT void DisableInfoPopUPIcons(void);
void RemoveNewObject(void *param);
unsigned char ClosePopUpEventHandler(void *param1, unsigned char param2, unsigned int param3, unsigned int param4);
unsigned char DeleteObjectEventHandler(void *param_1, unsigned char param_2);
unsigned char PopUpOkEventHandler(void *param_1, unsigned char flags);
unsigned char CBCloseEventHandler(void *param1, unsigned char param2);
unsigned char PrevIconEventHandler(void *arg0, unsigned char flags);
unsigned char NextIconEventHandler(void *arg1, unsigned char flags, unsigned int arg3, unsigned int arg4);
unsigned char AddGardenerEventHandler(void *param_1, unsigned char param_2);
unsigned char AddMechanicsEventHandler(void *param_1, unsigned char param_2);
unsigned char DeleteWorkOrderEventHandler(void *param_1, unsigned char param_2);
unsigned int TryPlayAdviceSpeech(unsigned int param);
LEGO_EXPORT void DrawPopUpInfo(void);
unsigned int PlayCantBuildAdvice(unsigned int param_1);
LEGO_EXPORT void PopUpInfoSetUp(struct HoverInfo info, unsigned int param_4, unsigned int param_5);
