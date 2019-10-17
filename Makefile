#CPP_FILES = $(wildcard *.cpp)
#OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))

#FILES TO COMPILE
START_FILES = Basic\ Movement/main.cpp Basic\ Movement/Player.cpp Basic\ Movement/Player.hpp

#OUTPUT
OUT_NAMES = $(patsubst %.cpp,%.o,$(START_FILES))

#COMPILER
CC = g++

#COMPILER FLAGS
COMPILER_FLAGS = -Wall -I/usr/include/SDL2

#LINKER FLAGS (SUPPORT FOR WINDOWS AND MAC)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf #-F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf

#TARGET
all: start run

start: $(START_FILES)
	$(CC) $(START_FILES) -w $(LINKER_FLAGS)

npc.o: NPC_Gen/npc.cpp NPC_Gen/bmp_edit.hpp
	$(CC) -c $^ $(COMPILER_FLAGS) -std=c++11

bmp_edit.o: NPC_Gen/bmp_edit.cpp
	$(CC) -c $^ $(COMPILER_FLAGS) -std=c++11

npc: bmp_edit.o npc.o
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -std=c++11

run: a.out
	./a.out

clean:
	rm *.o start npc
