#pragma once

struct Sample;
struct SampleDef;

struct Sample *CreatePlayableSample(unsigned int def);
int PlaySample(struct Sample *sample, unsigned int looping, unsigned int oneshot);
void PauseSingleSample(struct Sample *sample);
void FUN_00492850(void);
int ResumeSinglyPausedSample(struct Sample *sample);
void FUN_00492980(void);
void FUN_00492990(void);
int SetSampleFade(struct Sample *sample, unsigned int fade);
void KillPlayableSample(struct Sample *sample);
void DeleteSampleDef(struct SampleDef *def);
int KillSoundSampleSystem(void);
int FUN_00495b00(void);
