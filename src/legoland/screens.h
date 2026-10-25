#pragma once

void GetProductVersion(char *param_1);
// The game's own __ftol at 0x00458930 (compiler float->int conversion helper).
int _ftol();
void ResetEditState(void);
void StartLevelGame(void);
void UnloadMap(void);
void SetMapLoaded(unsigned int param_1);
void MarkLevelComplete(void);
void RunGameApplication(void);
void SetLevelEndScreenFile(int param0, const char *param1);
void EndLevel(unsigned int a1);
void RenderFrontEndScreen(unsigned char param_1);
void GameProc(void);
void RenderMapScreenFrame(void);
