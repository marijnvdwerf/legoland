#pragma once

struct NerpsArg;
struct RewardArg;

struct Vec4 {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
};

/* Canonical objective-event node (~0x44 bytes), allocated by FUN_00468910.
 * objectives.c owns the allocator and treats it as ObjectiveEvent; nerps.c
 * previously viewed the same object as EventNode/EventNodeVec. Unified here so
 * the boundary casts disappear. */
struct ObjectiveEvent {
    struct ObjectiveEvent *next;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int type;
    unsigned char flags_10;
    unsigned char pad_11[0x14 - 0x11];
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    struct Vec4 vec_28;
    int sort_key;
    unsigned int timestamp;
    unsigned int field_40;
};

void FUN_00468810(char *name);
void FUN_00468830(void);
void FUN_00468840(void);
void FUN_00468860(int index, signed char value);
unsigned char FUN_00468890(int index, unsigned char value);
char FUN_004688c0(int index);
void FUN_004688e0(void);
void FUN_004688f0(int index, unsigned char param_2);
struct ObjectiveEvent *FUN_00468910(unsigned int type, int sort_key);
void FUN_00468940(struct ObjectiveEvent *node);
void FUN_00468970(struct ObjectiveEvent *node);
void FUN_004689a0(void);
unsigned int FUN_004689f0(char *param_1, char *param_2, int param_3);
void FUN_00468b00(struct ObjectiveEvent *node);
void FUN_00468b40(struct ObjectiveEvent *node, unsigned int param_2, unsigned int param_3);
struct ObjectiveEvent *FUN_00468bb0(const char *format, ...);
void FUN_00468c00(void);
void FUN_00468d00(void);
int FUN_00468d10(void);
int FUN_00468d30(struct NerpsArg *arg);
void FUN_00468d80(struct NerpsArg *object, unsigned int a, int b);
void FUN_00468e40(struct NerpsArg *arg, unsigned int class_id, int count, int sum);
void FUN_00468ea0(struct NerpsArg *arg, unsigned int class_id, int count, int sum);
void FUN_00468f40(struct NerpsArg *arg, unsigned int class_id, int count);
void FUN_00468f80(struct NerpsArg *object, int count);
void FUN_004690c0(struct NerpsArg *arg, int count);
void FUN_00469100(struct NerpsArg *object, int a, unsigned int b);
void FUN_004691e0(struct NerpsArg *arg, int param_2, unsigned int param_3);
void FUN_00469260(struct NerpsArg *arg, unsigned int class_id, int sum, int count);
void FUN_00469310(struct NerpsArg *object, unsigned int param_2, int count);
void FUN_00469350(struct NerpsArg *object, int count);
void FUN_00469390(struct NerpsArg *object);
unsigned int FUN_00469400(void);
void FUN_00469bd0(unsigned int a, void *b);
int FUN_00469c40(struct ObjectiveEvent *event);
void FUN_00469ab0(struct NerpsArg *object);
int FUN_00469ae0(struct RewardArg *arg);
int FUN_00469b00(struct RewardArg *arg);
void FUN_00469900(struct NerpsArg *object, unsigned int a, unsigned int b);
