#include "headers/ApplyGravity.h"
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#endif

#define REALG 6.67430e-11f
#define SIMULATEDG 6.67430e-2f

void ApplyGravity(cbody_t** BodysLoaded, const u16 SizeOfArray)
{
    for (int i = 0; i < SizeOfArray; i++) // Seleccionar ObjetoA
    {
        if (BodysLoaded[i] == NULL) continue; // Si la memoria no apunta a nada, ignorar
        cbody_t* ObjectA = BodysLoaded[i]; // Si la memoria apunta a un cuerpo, guardar el puntero en una variable
        for (int j = 0; j < SizeOfArray; j++) // Seleccionar ObjetoB
        {
            if (BodysLoaded[j] == NULL) continue; // Si la memoria no apunta a nada, ignorar
            if (BodysLoaded[j] == BodysLoaded[i]) continue; // Si la direccion de memoria de ObjetoB apunta a ObjetoA, ignorar
            cbody_t* ObjectB = BodysLoaded[j];  // Si la memoria apunta a un cuerpo, guardar el puntero en una variable
            float dx = ObjectB->x - ObjectA->x; // Calcular distancias
            float dy = ObjectB->y - ObjectA->y;
            float distSqr = dx*dx + dy*dy;
            float dist = sqrtf(distSqr);
            if (dist < 1e-3f) continue; // Si la distancia es demasiado pequeña, evitar errores de division por 0
            float force = SIMULATEDG * ObjectA->mass * ObjectB->mass / distSqr; // Calcular fuerza aplicada por el ObjetoB hacia ObjetoA
            float fx = force * dx / dist; // Aplicar aceleración en eje X e Y
            float fy = force * dy / dist;
            ObjectA->vx += fx / ObjectA->mass; // Sumar las fuerzas finales con la velocidad final
            ObjectA->vy += fy / ObjectA->mass;
        }
    }
}