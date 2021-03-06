# what to compile
OBJS = src/main.cpp src/pipera.cpp

# what to compile with
CC = g++

# compile flags
COMPILER_FLAGS = -O -Wall -std=c++11 `sdl-config --cflags` -lSDL_image -DUSING_SDL1
COMPILER_FLAGS2 = -O -Wall -std=c++11 `sdl2-config --cflags` -lSDL2_image

# what to link
LINKER_FLAGS = `sdl-config --libs` -lSDL_ttf
LINKER_FLAGS2 = `sdl2-config --libs` -lSDL2_ttf

# output name
OBJ_NAME = pipera_demo

default: sdl1

sdl2: $(OBJS)
	@echo "Building with SDL2"
	$(CC) $(OBJS) $(COMPILER_FLAGS2) $(LINKER_FLAGS2) -o bin/$(OBJ_NAME)
	./bin/$(OBJ_NAME)

sdl1: $(OBJS)
	@echo "Building with SDL1.2"
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o bin/$(OBJ_NAME)
	./bin/$(OBJ_NAME)

# fresh start
clean:
	rm -f bin/$(OBJ_NAME) bin/*.o