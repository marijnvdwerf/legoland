#pragma once

struct CostInfo;
struct ObjCountWrap;
struct EditObject;
struct BestNode;
struct InstancePos;
struct Bloke;

int GetObjCost(struct CostInfo *info);
unsigned int BasicObjectDCalcCursor(unsigned int param_1, unsigned int param_2);
void ClearObjectCounters(void);
unsigned int ObjCount(struct ObjCountWrap *wrap);
void FreeBlokeCounters(void);
void ClearBlokeCounters(unsigned int index, struct Bloke *bloke);
void FUN_00481170(void);
void SetEditObject(struct EditObject *obj);
struct BestNode *FUN_00481730(void);
struct BestNode *FUN_00481790(struct InstancePos *pos);
void FUN_00481b10(struct BestNode *node);
