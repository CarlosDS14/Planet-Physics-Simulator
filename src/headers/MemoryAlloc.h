// TODO: Make better memory allocation for every custom datatype (example: cbody_t, GUI_t, TTF_Pair, etc)
#ifndef MEMORYALLOC_H
#define MEMORYALLOC_H
#include <stdlib.h>
#include "structs.h"

#define DEFAULT_OOMK_LIMIT_MB 512

extern u64 BytesAlloc;

void* TrackMalloc(size_t bytes);
void TrackFree(void* mem);

/*

// Idk, i think i will not use this functions

#define FreeMemory(mem) free(mem)

TTF_Pair* MallocTTF(char* KeyValue, const char* file, int ptsize);
cbody_t* MallocCBody(float x, float y, float vx, float vy, float radius, float mass, u8 r, u8 g, u8 b, u8 a, u8 BitfieldConfigs);
GUI_t* MallocUI(void); // Work in progress, not defined
void FreeTTFPair(TTF_Pair* pair); // Special case for freeing TTF_Pair heap blocks

*/

#endif