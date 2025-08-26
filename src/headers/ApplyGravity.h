#ifndef APPLYGRAVITY_H
#define APPLYGRAVITY_H
#include "structs.h"

void ApplyGravity(cbody_t** BodysLoaded);

void ApplyLensEffect(cbody_t** BodysLoaded, Photon_t** Photons);

#endif // APPLYGRAVITY_H