#pragma once

#include "legoland.h"

struct Point;

struct Point;

int WorkerSuggestNextMove(int *a, int *b, int *out);
LEGO_EXPORT int SuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out);
LEGO_EXPORT int PTPSuggestNextMove(struct Point *pos, struct Point *goal, struct Point *out);
LEGO_EXPORT void AddPathSquare(struct Point *pos);
LEGO_EXPORT void RemovePathSquare(struct Point *pos);
void UpdateConnectedNodes(struct Point *pos);
void ClearEntrance1(void);
void InitEntrance1Point(void);
void FreeBestNodeList(void);
struct Point *GetEntrance1Point(void);
unsigned int SaveBestNodeList(void);
int LoadBestNodeList(void);
