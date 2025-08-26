#ifndef LOADSAVE_H
#define LOADSAVE_H
#include "structs.h"

void default_bodies(cbody_t** BodysLoaded);
int load_bodies(cbody_t** BodysLoaded, const u16 SizeOfArray, const char* filename, const u8 FileVersionRequired);
cbody_t* LoadBody(FILE* file, int locateddatapos);

#endif // LOADSAVE_H