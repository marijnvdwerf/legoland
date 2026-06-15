#pragma once

struct NerpsListNode {
    struct NerpsListNode *field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
};

struct SortNode {
    struct SortNode *next;
    int val;
};

struct StringHolder;
struct Vec4;

void FUN_0046b240(unsigned int param_1);
unsigned int FUN_0046b280(void);
void FUN_0046b2d0(void);
struct NerpsListNode *FUN_0046b4f0(unsigned int param_1);
void FUN_0046b590(struct SortNode *node);
void FUN_0046b650(const char *src, struct StringHolder *holder);
unsigned int FUN_0046b700(void);
unsigned int FUN_0046b760(void);
void FUN_0046b790(unsigned int param_1, unsigned int param_2);
void FUN_0046b7f0(unsigned int param_1);
void FUN_0046b850(unsigned int param_1);
void FUN_0046b9c0(unsigned int param_1);
void FUN_0046ba30(unsigned int param_1);
void FUN_0046bad0(unsigned int param_1, unsigned int param_2, unsigned int *param_3);
void FUN_0046bb10(unsigned int param_1, unsigned int param_2);
void FUN_0046bb80(unsigned int param_1, unsigned int param_2);
void FUN_0046bbb0(unsigned int param_1, unsigned int param_2);
void FUN_0046bc40(void);
void FUN_0046bc60(void);
void FUN_0046bcb0(unsigned int param_1, unsigned int param_2);
void FUN_0046bd10(unsigned int param_1);
void FUN_0046bd40(unsigned int param_1);
void FUN_0046bda0(unsigned int *param_1);
void FUN_0046bdd0(unsigned char param_1, unsigned int param_2, unsigned int param_3);
void FUN_0046be00(unsigned char param_1, unsigned int param_2, unsigned int *param_3);
void FUN_0046be40(unsigned char param_1, unsigned int param_2, unsigned int param_3, struct Vec4 *param_4);
void FUN_0046bef0(unsigned char param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
void FUN_0046c030(unsigned char param_1, unsigned int param_2, unsigned int param_3);
void FUN_0046c090(unsigned char param_1, unsigned int param_2);
void FUN_0046c0c0(unsigned char param_1, unsigned int param_2, unsigned int param_3);
void FUN_0046c120(unsigned char param_1, unsigned int param_2);
void FUN_0046c290(unsigned char param_1, unsigned int param_2);
void FUN_0046c2f0(unsigned char param_1, unsigned int param_2);
void FUN_0046c320(unsigned char param_1, unsigned int param_2);
void FUN_0046c390(unsigned char param_1, unsigned int param_2, unsigned int param_3);
void FUN_0046c3c0(unsigned char param_1, unsigned int param_2);
void FUN_0046c420(unsigned char param_1, unsigned int param_2, unsigned int param_3);
void FUN_0046c5c0(void);
unsigned int FUN_0046cb20(void);
void FUN_0046ce00(void);
