CPP_FILES = $(wildcard *.cpp)
OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))
CC = clang++
COMPILER_FLAGS = -Wall -I/usr/include/SDL2
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all: murder

sdl2_bmp.o: sdl2_bmp.cpp
	$(CC) -c $^ $(COMPILER_FLAGS)

npc.o: npc.cpp
	$(CC) -c $^ $(COMPILER_FLAGS)

murder: npc.o sdl2_bmp.o
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)
	
clean:
	rm *.o murder
