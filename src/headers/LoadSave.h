#ifndef LOADSAVE_H
#define LOADSAVE_H
#include "structs.h"

void default_bodies(cbody_t** BodysLoaded);
int load_bodies(cbody_t** BodysLoaded, const u16 SizeOfArray, const char* filename);

#endif // LOADSAVE_H