#pragma once

/* K&R cross-TU prototypes for the driving-school bloke AI (FUN_00405bd0),
   mirroring boating_school.h. The owning TUs keep their own (sometimes typed)
   declarations; these unspecified-parameter forms let the AI loop pass blokes
   as raw int + byte offsets the way the original codegen expects. */
void FUN_00402c10();
void FUN_00414440();
int FUN_00401c40(short param_1);
int FUN_00401ae0();
char CalcMoveLine();
void NewDirForAction();
void BlokeSitAnim();
void BlokeSetFrame();
void KillAllSamplesFromSource();
void RenderCursor();
int GetBlokeNum();
int GetBlokePtr();

void FUN_00405370();
void FUN_00405460();
void FUN_00405570();
void FUN_00405630();
void FUN_00405740();
void FUN_004058a0();
void FUN_00405940();
unsigned int * FUN_00405ad0(struct DSCarLayer *arg1, unsigned short arg2);
void FUN_00405b10();
void FUN_00405bd0();
int FUN_00405e70(void);
int FUN_00406050(void);
int FUN_00406070(void);