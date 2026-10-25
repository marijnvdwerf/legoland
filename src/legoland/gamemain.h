#pragma once

struct GameMainNode;
struct Point;
struct ResFile;

/* A level script command line: the command name and its arguments. */
struct CommandArgs {
    char *name;
    char *arg1;
    char *arg2;
    char *arg3;
};

int IsInMapBounds(int a, int b);
void AddToClosedList(struct GameMainNode *node);
void InsertOpenListSorted(struct GameMainNode *node);
struct GameMainNode *FindClosedPathNode(struct Point *ctx);
struct GameMainNode *FindOpenPathNode(struct Point *arg);
struct GameMainNode *GetPathNodeAt(struct Point *pos, int *result);
void RemoveFromClosedList(struct GameMainNode *ctx);
void RemoveFromOpenList(struct GameMainNode *param_1);

void RemoveObjectAtPoint(struct Point *p);
void FindMapPathAStar(int x, int y, int a, int b);
int FindStringNoCase(const char *param_1, const void *param_2, int param_3);
struct ScriptCommand;
int ParseScriptFile(const char *name, struct ScriptCommand *commands, int count, int flags);
int ParseScriptResFile(struct ResFile *file, struct ScriptCommand *commands, int count, int flags);
void ResetLevelScriptState(void);
void SetScriptSectionType(char *param_1, unsigned int param_2);
void SetScriptObjectiveKind(unsigned int param_1);
void SetScriptObjectivePurgeFlag(struct CommandArgs *param_1, unsigned int param_2);
int HasEnoughScriptArgs(void *param_1, unsigned int param_2, unsigned int param_3);
int IsScriptCommandAllowed(void *param_1, unsigned int param_2, unsigned int param_3);
void ParseRect(int *param_1, char **param_2, int param_3);
unsigned int CheckScriptCommand(void *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
void ParseIntPair(int *param_1, char **param_2, int param_3);
char *GetScriptArg1(struct CommandArgs *param_1, unsigned int param_2);
