#pragma once

#include "legoland.h"
#include "movie_pool.h"

struct IconNode;
struct MovieHandle;

struct InterfaceListNode {
    /* 0x00 */ struct InterfaceListNode *next;
    /* 0x04 */ void *data;
    /* 0x08 */ int flag;
};

struct TrackElemPair {
    /* 0x00 */ char *track_name;
    /* 0x04 */ char *elem_name;
};

struct InterfaceResearchNode {
    /* 0x00 */ struct InterfaceResearchNode *next;
    /* 0x04 */ void *data;
    /* 0x08 */ int field_8;
};

struct InterfacePanel {
    /* 0x00 */ short group;
    /* 0x02 */ unsigned char pad_2[0x04 - 0x02];
    /* 0x04 */ int field_4;
    /* 0x08 */ struct IconNode *icon;
    /* 0x0c */ int content_left;
    /* 0x10 */ int content_top;
    /* 0x14 */ int field_14;
    /* 0x18 */ int content_bottom;
    /* 0x1c */ int clip_left;
    /* 0x20 */ int clip_top;
    /* 0x24 */ int clip_right;
    /* 0x28 */ int clip_bottom;
};

void SetDefaultMouseHandlers(void);
void UpdateScriptEndIcon(void *a);
int SaveObjectiveIcons(void);
int LoadObjectiveIcons(void);
LEGO_EXPORT int InitGameInterface(int a);
void UnloadGameInterface(void);
unsigned char InterfaceBriefingClicked(void *context, unsigned int flags, const char *a, const char *b);
unsigned char InterfaceAdvisorClicked(unsigned int a, unsigned char flags);
unsigned char SetEditObjectIconHandler(struct IconNode *a, unsigned int flags);
unsigned char ScriptEndIconHandler(unsigned int a, unsigned int flags);
unsigned char InterfaceQueryClicked(unsigned int a, unsigned int flags);
unsigned char InterfaceEraserClicked(unsigned int a, unsigned int flags);
unsigned char LegolandThemeIconHandler(struct IconNode *param_1, unsigned char flags);
unsigned char AdventurersThemeIconHandler(struct IconNode *param_1, unsigned char flags);
unsigned char CastleThemeIconHandler(struct IconNode *param_1, unsigned char flags);
unsigned char WesternThemeIconHandler(struct IconNode *param_1, unsigned char flags);
unsigned char ToggleMapScreen(unsigned int a, unsigned char flags);
unsigned char InterfaceOptionsClicked(unsigned int a, unsigned int flags, unsigned int c, unsigned int d);
LEGO_EXPORT void DelObjectList(void);
LEGO_EXPORT int MakeUpObjectList(int param_1, int param_2, int param_3, int param_4);
LEGO_EXPORT unsigned int ObjectLinkedList(unsigned int *entry);
void ClearInterfaceSlotValues(void);
void ClearButtonFlashStates(void);
void DrawFlashingButtons(void);
void SetButtonFlashMask(int mask, unsigned int value);
void SetObjectiveIconShown(int index, int value);
void ShowProfileObjectiveIcons(void);
int GetPerformanceTime(void);
int OpenMovieAudio(struct MovieHandle *handle);
int StartMovieAudio(struct MovieHandle *handle);
int StopMovieAudio(void);
int UpdateAviAudioBuffer(unsigned int param_1, int param_2);
int PlayMovie(char *filename, unsigned int param_2, int param_3);
LEGO_EXPORT void DisableSidePanelIcons(void);
LEGO_EXPORT void EnableSidePanelIcons(void);
LEGO_EXPORT void ListChildrenBar(struct InterfaceListNode *node, int group, short x, short y);
LEGO_EXPORT void CloseChildrenBar(struct InterfaceListNode *node, int group, short x, short y);
LEGO_EXPORT void RedrawObjectList(struct InterfacePanel *panel, int param_2, int delta);
void DeselectThemeIcon(void);
LEGO_EXPORT unsigned int TestMenu(unsigned int *entry);
char ListChildrenIconHandler(struct IconNode *param_1, unsigned char param_2);
char CloseChildrenIconHandler(struct IconNode *param_1, unsigned char param_2);
LEGO_EXPORT void UpdateMenu(void);
LEGO_EXPORT void Load_Interface_ControlIcons(void);
LEGO_EXPORT void Load_Interface_ThemeIcons(void);
LEGO_EXPORT void UnLoad_Interface_ControlIcons(void);
LEGO_EXPORT void UnLoad_Interface_ThemeIcons(void);
void RestoreBuildMenuAfterLoad(void);
void SelectMatchingBuildElement(void);
