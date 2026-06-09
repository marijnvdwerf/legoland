#pragma once

struct Point {
    /* 0x00 */ int x;
    /* 0x04 */ int y;
};

void FUN_00480840(struct Point *src, struct Point *dst, int dir);
void FUN_004808d0(int *src, int *dst, int dir);
