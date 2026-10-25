#pragma once

struct CommandArgs;

int EnableObjectClass(char *arg);
unsigned int ScriptCmdEnable(struct CommandArgs *param_1, unsigned int param_2, unsigned int param_3);
int ScriptCmdGive(struct CommandArgs *arg, int argc);
int LoadLevelScript(const char *param_1);
