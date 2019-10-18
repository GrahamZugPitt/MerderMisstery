#CPP_FILES = $(wildcard *.cpp)
#OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))

#FILES TO COMPILE
START_FILES = Basic_Movement/main.cpp Basic_Movement/Player.cpp Basic_Movement/Player.hpp
SOURCE_FILES = bmp_edit npc main Player
#OUTPUT
OUT_NAMES = $(patsubst %.cpp,%.o,$(START_FILES))
OBJDIR := Basic_Movement
NPC_DIR := NPC_Gen
NPC_HEADERS = NPC_Gen/bmp_edit.hpp NPC_Gen/npc.hpp

OBJ_FILES = $(patsubst %,%.o, $(SOURCE_FILES))

#COMPILER
CC = g++

#COMPILER FLAGS
COMPILER_FLAGS = -Wall -I/usr/include/SDL2

#LINKER FLAGS (SUPPORT FOR WINDOWS AND MAC)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf #-F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf
UNAME := $(shell uname)
ifeq ($(OS),Windows_NT)
	# Windows specific stuff can go here
else
	#not windows
	ifeq ($(UNAME),Linux)
		LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
	else 
		LINKER_FLAGS = -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf
	endif
endif
#TARGET
all: murder.o

start: $(START_FILES)
	$(CC) $(START_FILES) -w $(LINKER_FLAGS)

%.o: $(OBJDIR)/%.cpp
	$(CC) -c -o $@ $< $(COMPILER_FLAGS) -std=c++14


%.o: $(NPC_DIR)/%.cpp $(NPC_HEADERS)
	$(CC) -c -o $@ $< $(COMPILER_FLAGS) -std=c++14

murder.o: $(OBJ_FILES)#bmp_edit.o npc.o main.o Player.o#$(START_FILES)  
	$(CC) -o $@ $^ $(LINKER_FLAGS) -std=c++14

clean:
	rm *.o
