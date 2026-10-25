#pragma once

#include "image_sprite.h"
#include "render.h"

/* One row of the appraisal report (RunAppraisal); 19 ints. */
struct AppraisalRow {
    /* 0x00 */ int page; /* report page the row is on */
    /* 0x04 */ int x; /* indent */
    /* 0x08 */ int type; /* check result (1 passed, 0 failed); -1 failed-check advice, -2 summary, -3 continuation line */
    /* 0x0c */ int rnd; /* rand() % 5, chooses the mark's picture */
    /* 0x10 */ char *text;
    /* 0x14 */ int arg;
    /* 0x18 */ int bar; /* draw a value bar */
    /* 0x1c */ int value;
    /* 0x20 */ int goal;
    /* 0x24 */ int max;
    /* 0x28 */ int nids; /* speech ids queued when the row is shown */
    /* 0x2c */ int ids[8];
};

struct LocFile;
struct IconNode;

unsigned int RenderAdvisorIcon(struct IconNode *param_1);
void UpdateAppraisalPageButtons(void);
struct TextureNode;
struct Image;
void BuildTextureNodeFromImage(struct Image *param_1, struct TextureNode *param_2);
void SetNextAdvisorAnimState(unsigned int param_1, unsigned int param_2);
void ResetReportFlags(void);
void ResetAppraisalStreak(void);
void StartAppraisalTimer(void);
void StopAppraisalTimer(void);
void ConfigureAppraisal(unsigned int param_1, const char *param_2);
unsigned int GetLoadedTextureCount(void);
void LoadTextureBitmaps(struct LocFile *param_1, const char *param_2);
unsigned int SetReportLegolandZoneGoal(unsigned int param_1, unsigned int param_2);
unsigned int SetReportAdventurerZoneGoal(unsigned int param_1, unsigned int param_2);
unsigned int SetReportCastleZoneGoal(unsigned int param_1, unsigned int param_2);
void SetReportWesternZoneGoal(unsigned int param_1, unsigned int param_2);
void SetReportCastleScoreGoal(unsigned int param_1, unsigned int param_2);
void SetReportDrivingSchoolScoreGoal(unsigned int param_1, unsigned int param_2);
void SetReportLogFlumeScoreGoal(unsigned int param_1, unsigned int param_2);
void SetReportBoatingSchoolScoreGoal(unsigned int param_1, unsigned int param_2);
void SetReportJungleCruiseScoreGoal(unsigned int param_1, unsigned int param_2);
void SetReportType1Or3ObjectCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportType1Or3ClassCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportLinkedObjectPercentGoal(unsigned int param_1, unsigned int param_2);
void SetReportType2ObjectCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportType2ClassCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportSceneryCoverageGoal(unsigned int param_1, unsigned int param_2);
void SetReportFoodStallObjectCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportFoodStallClassCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportType4ObjectCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportType4ClassCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportVisitorCountGoal(unsigned int param_1, unsigned int param_2);
void SetReportVisitorHappinessGoal(unsigned int param_1, unsigned int param_2);
void SetReportVisitorFullnessGoal(unsigned int param_1, unsigned int param_2);
void SetReportPowerSupplyGoal(unsigned int param_1, unsigned int param_2);
void SetReportWorkingRidesGoal(unsigned int param_1, unsigned int param_2);
void SetReportMapTileCountGoal(unsigned int param_1, unsigned int param_2);
void LoadAdvisorAnims(void);
void FreeAdvisorAnims(void);
int CheckAppraisalDue(void);
unsigned int SaveReport(void);
unsigned int LoadReport(void);
