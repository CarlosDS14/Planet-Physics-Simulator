#include <stdio.h>
#include <stdlib.h>
#include "headers/LoadSave.h"

int load_bodies(cbody_t** BodysLoaded, const u16 SizeOfArray, const char* filename)
{
    // Metadata of file
    const char* FileSignature = "PPSSAV";
    u8 FileSystemVer = 0;
    u16 ObjectsSaved = 0;
    // Vars
    FILE* SaveFile = fopen(filename, "rb");
    u32 ByteCounter = 0;
    u8 ByteFetched = 0;
    // Check file type
    while (1)
    {
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&ByteFetched, sizeof(u8), 1, SaveFile);
        if (ByteFetched == 0x1A) break;
        else if (ByteFetched == FileSignature[ByteCounter])
            ByteCounter++;
        else if (ByteFetched != FileSignature[ByteCounter])
            return 1;
    }
    // Get Metadata
    ByteCounter++;
    fseek(SaveFile, ByteCounter, SEEK_SET);
    fread(&FileSystemVer, sizeof(u8), 1, SaveFile);
    ByteCounter++;
    fseek(SaveFile, ByteCounter, SEEK_SET);
    fread(&ObjectsSaved, sizeof(u16), 1, SaveFile);
    printf("FileSystemVer: %d\n", FileSystemVer);
    printf("ObjectsSaved: %d\n", ObjectsSaved);
    ByteCounter += 2;

    for (u16 i = 0; i < ObjectsSaved; i++)
    {
        // Allocate memory for the bodies
        BodysLoaded[i] = malloc(sizeof(cbody_t));
        // Get X pos
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->x, sizeof(float), 1, SaveFile);
        // Get Y pos
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->y, sizeof(float), 1, SaveFile);
        // Get X velocity
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->vx, sizeof(float), 1, SaveFile);
        // Get Y velocity
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->vy, sizeof(float), 1, SaveFile);
        // Get radius
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->radius, sizeof(float), 1, SaveFile);
        // Get mass
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->mass, sizeof(float), 1, SaveFile);
        // Get red color
        ByteCounter += 4;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->ColorRed, sizeof(u8), 1, SaveFile);
        // Get green color
        ByteCounter++;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->ColorBlue, sizeof(u8), 1, SaveFile);
        // Get blue color
        ByteCounter++;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->ColorBlue, sizeof(u8), 1, SaveFile);
        // Get alpha channel
        ByteCounter++;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->AlphaChannel, sizeof(u8), 1, SaveFile);
        // Get bitfield configuration
        ByteCounter++;
        fseek(SaveFile, ByteCounter, SEEK_SET);
        fread(&BodysLoaded[i]->Bitfield, sizeof(u8), 1, SaveFile);
    }
    for (int i = 0; i < SizeOfArray; i++)
    {
        if (BodysLoaded[i] == NULL) continue;
        printf("Body %d\n", i);

    }

    fclose(SaveFile);
    return 0;
}

void default_bodies(cbody_t** BodysLoaded)
{
    // Test bodies
    BodysLoaded[0] = malloc(sizeof(cbody_t));
    BodysLoaded[1] = malloc(sizeof(cbody_t));
    BodysLoaded[2] = malloc(sizeof(cbody_t));
    BodysLoaded[3] = malloc(sizeof(cbody_t));

    BodysLoaded[0]->x = 500;                       // Position X and Y
    BodysLoaded[0]->y = 310;
    BodysLoaded[0]->vx = 0;                        // Initial velocity in X and Y
    BodysLoaded[0]->vy = -2.46f;
    BodysLoaded[0]->radius = 7.8f;
    BodysLoaded[0]->mass = 180;
    BodysLoaded[0]->ColorRed = 0xe8;               // Color red, green and blue
    BodysLoaded[0]->ColorGreen = 0x5b;
    BodysLoaded[0]->ColorBlue = 0x51;
    BodysLoaded[0]->AlphaChannel = 0xff;
    BodysLoaded[0]->Bitfield = 0b00000000;         // Extra settings

    BodysLoaded[1]->x = 400;                       // Position X and Y
    BodysLoaded[1]->y = 310;
    BodysLoaded[1]->vx = 0;                        // Initial velocity in X and Y
    BodysLoaded[1]->vy = 0;
    BodysLoaded[1]->radius = 40;
    BodysLoaded[1]->mass = 10000;
    BodysLoaded[1]->ColorRed = 0x5d;               // Color red, green and blue
    BodysLoaded[1]->ColorGreen = 0x6d;
    BodysLoaded[1]->ColorBlue = 0xe8;
    BodysLoaded[1]->AlphaChannel = 0xff;
    BodysLoaded[1]->Bitfield = 0b00000001;

    BodysLoaded[2]->x = 400;
    BodysLoaded[2]->y = 550;
    BodysLoaded[2]->vx = 1.58f;
    BodysLoaded[2]->vy = 0;
    BodysLoaded[2]->radius = 10.4f;
    BodysLoaded[2]->mass = 300;
    BodysLoaded[2]->ColorRed = 0x6f;
    BodysLoaded[2]->ColorGreen = 0xf2;
    BodysLoaded[2]->ColorBlue = 0xa2;
    BodysLoaded[2]->AlphaChannel = 0xff;
    BodysLoaded[2]->Bitfield = 0b0000000;

    BodysLoaded[3]->x = 400;
    BodysLoaded[3]->y = 569.0f;
    BodysLoaded[3]->vx = 2.65f;
    BodysLoaded[3]->vy = -0.05f;
    BodysLoaded[3]->radius = 3.6f;
    BodysLoaded[3]->mass = 40;
    BodysLoaded[3]->ColorRed = 0xff;
    BodysLoaded[3]->ColorGreen = 0xff;
    BodysLoaded[3]->ColorBlue = 0xff;
    BodysLoaded[3]->AlphaChannel = 0xff;
    BodysLoaded[3]->Bitfield = 0b00000000;
    return;
}