#CPP_FILES = $(wildcard *.cpp)
#OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))

#FILES TO COMPILE
START_FILES = Basic\ Movement/main.cpp Basic\ Movement/Player.cpp Basic\ Movement/Player.hpp

#OUTPUT
OUT_NAMES = $(patsubst %.cpp,%.o,$(START_FILES))

#COMPILER
CC = clang++

#COMPILER FLAGS
COMPILER_FLAGS = -Wall -I/usr/include/SDL2

#LINKER FLAGS (SUPPORT FOR WINDOWS AND MAC)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_ttf

#TARGET
all : start

start : $(START_FILES)
	$(CC) $(START_FILES) -w $(LINKER_FLAGS)

sdl2_bmp.o: sdl2_bmp.cpp
	$(CC) -c $^ $(COMPILER_FLAGS)

npc.o: npc.cpp
	$(CC) -c $^ $(COMPILER_FLAGS)

murder: npc.o sdl2_bmp.o
	$(CC) -o $@ $^ $(COMPILER_FLAGS) $(LINKER_FLAGS)

clean:
	rm *.o start
