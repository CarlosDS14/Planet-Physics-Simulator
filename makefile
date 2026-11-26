COMPILER = gcc
FLAGS = -g -Wall

SOURCE_FILES= src/MainExec.c src/LoadSave.c src/GeometryRender.c src/ApplyGravity.c src/MemoryAlloc.c src/globals.c
DEPENDENCIES = # Empty
INCLUDES = # Empty

LINK_LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

EXECNAME = PanetTester

all:
	$(COMPILER) $(SOURCE_FILES) $(DEPENDENCIES) $(INCLUDES) $(LINK_LIBS) -o $(EXECNAME) $(FLAGS)