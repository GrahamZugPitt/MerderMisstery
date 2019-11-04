#CPP_FILES = $(wildcard *.cpp)
#OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))

#FILES TO COMPILE
START_FILES = murder.o
SOURCE_FILES = bmp_edit worldObjects chat npc gameloop menuloop main_helper Player main
#OUTPUT
OUT_NAMES = $(patsubst %.cpp,%.o,$(START_FILES))
OBJDIR := Code/programfiles/
NPC_DIR := Code/NPC_Gen/
NPC_HEADERS = Code/NPC_Gen/bmp_edit.hpp Code/NPC_Gen/npc.hpp

OBJ_FILES = $(patsubst %,%.o, $(SOURCE_FILES))

#COMPILER
CC = clang++

#COMPILER FLAGS
COMPILER_FLAGS = -Wall -I/usr/include/SDL2

#LINKER FLAGS (SUPPORT FOR WINDOWS AND MAC)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

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
	rm *.o ./Art/NPCs/*m.bmp

farnan:
	make
	./murder.o farnan

rebuild:
	make clean
	make

run:
	make
	./murder.o
