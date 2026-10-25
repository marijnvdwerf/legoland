#pragma once

struct QueueTable;
struct QueueNode;

struct Queue {
    struct QueueTable *count;
    struct QueueNode *head;
    struct QueueNode *tail;
};

unsigned int CountQueueNodesBetween(struct QueueNode *start, struct QueueNode *stop);
void SaveQueue(struct QueueNode *start, struct Queue *queue);
struct QueueNode *GetNthNextQueueNode(struct QueueNode *node, int n);
void LoadQueueNodes(struct QueueNode *start, struct Queue *queue);

struct PathPair {
    /* 0x00 */ int a;
    /* 0x04 */ int b;
};

struct PathTable {
    /* 0x00 */ int count;
    /* 0x04 */ struct PathPair *pairs;
};

struct QueueTable *BuildQueueTable(struct PathTable *param_1);

struct RideQueueEntry {
    /* 0x00 */ struct RideQueueEntry *next;
    /* 0x04 */ struct RideQueueEntry *field_4;
    /* 0x08 */ unsigned short id;
    /* 0x0a */ unsigned char pad_a[0x2];
    /* 0x0c */ int x;
    /* 0x10 */ int y;
    /* 0x14 */ unsigned char field_14;
    /* 0x15 */ unsigned char field_15;
    /* 0x16 */ unsigned char pad_16[0x2];
    /* 0x18 */ struct RideQueueEntry *field_18;
    /* 0x1c */ unsigned char field_1c;
    /* 0x1d */ unsigned char field_1d;
    /* 0x1e */ unsigned char pad_1e[0x2];
};

unsigned int IsQueueFull(struct Queue *queue);
unsigned int QueueHasNodes(struct Queue *queue);
int IsQueueHeadAtFront(struct Queue *queue);
void FreeAllQueueNodes(struct Queue *queue);
void QueueUnlinkHead(struct Queue *queue);
struct RideNode;
void LeaveQueueHead(struct Queue *queue, struct RideNode **out);
void StepQueueRiders(struct Queue *queue, unsigned int param_2, unsigned int param_3);
void FreeIfNotNull(void *param_1);
struct Bloke;
void StartQueueTableBackward(struct QueueTable *param_1, struct Bloke *slot);
void StartQueueTableForward(struct QueueTable *param_1, struct Bloke *slot);
struct QueueTable *GetBlokeQueueTable(struct Bloke *slot);
void MoveBlokeAlongQueueTable(struct QueueTable *table, int x, int y, struct Bloke *bloke);
void *FindQueueEntryAtTile(unsigned int a, unsigned int b);
struct RideQueueEntry *FindRoadEntryAt(int x, int y);
struct RideQueueEntry *FindQueueEntryById(unsigned short param_1);
void PlaceRoadTiles(int x, int y, int param_3, int param_4);
void JoinQueue(struct Queue *queue, struct RideNode *mid);

extern unsigned short RoadPieceTileTable[15][4];
