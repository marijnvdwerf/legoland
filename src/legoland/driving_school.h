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
void IncrementObjectCount();
void RenderCursor();
int GetBlokeNum();
int GetBlokePtr();
