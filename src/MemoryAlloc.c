#include "headers/MemoryAlloc.h"

#define MAX_MALLOC_TRIES 3

u64 BytesAlloc = 0;

void* TrackMalloc(size_t size)
{
    void* mem = NULL; // Create null memory pointer
    for (int i = 0; i < MAX_MALLOC_TRIES; i++) // Retry malloc if necesary
    {
        mem = malloc(size + sizeof(size_t));
        if (mem != NULL) break;
    }
    if (!mem) return NULL; // If memory still NULL, return NULL;

    *((size_t*)mem) = size; // Save the size like a header
    BytesAlloc += size + sizeof(size_t); // Save the memory size to BytesAlloc
    return mem + sizeof(size_t); // Save the user-pointer
}

void TrackFree(void* mem)
{
    if (!mem) return; // If pointer equals null, dont do anything

    void* realptr = mem - sizeof(size_t); // Get the real pointer
    size_t size = *((size_t*)realptr); // Get the size of the memory allocated
    
    BytesAlloc -= size + sizeof(size_t); // Free the memory size from BytesAlloc
    free(realptr); // free the real pointer
}

/*

TTF_Pair* MallocTTF(char* KeyValue, const char* file, int ptsize)
{
    TTF_Font* font = TTF_OpenFont(file, ptsize);
    if (font == NULL) return NULL;
    if (strlen(KeyValue) > 50)
    {
        TTF_CloseFont(font);
        return NULL;
    }
    TTF_Pair* pair = malloc(sizeof(TTF_Pair));
    strcpy(pair->FontKey, KeyValue);
    pair->FontValue = font;
    return pair;
}

cbody_t* MallocCBody(float x, float y, float vx, float vy, float radius, float mass, u8 r, u8 g, u8 b, u8 a, u8 BitfieldConfigs)
{
    cbody_t* celestialbody = malloc(sizeof(cbody_t));
    celestialbody->x = x;
    celestialbody->x = y;
    celestialbody->vx = vx;
    celestialbody->vx = vy;
    celestialbody->radius = radius;
    celestialbody->mass = mass;
    celestialbody->ColorRed = r;
    celestialbody->ColorGreen = g;
    celestialbody->ColorBlue = b;
    celestialbody->AlphaChannel = a;
    celestialbody->Bitfield = BitfieldConfigs;
    return celestialbody;
}

GUI_t* MallocUI(void)
{
    // TODO: Allocate memory for user interface, currently returning NULL
    return NULL;
}

void FreeTTFPair(TTF_Pair* pair)
{
    free(pair->FontValue);
    free(pair);
    BytesAlloc -= sizeof(TTF_Pair);
}

*/