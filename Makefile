
COMPILER = g++
OBJS = $(wildcard source/*.cpp)
APPNAME = Pacman

ifeq ($(OS),Windows_NT)
FLAGS=-L libs/raylib/lib/ -L libs/glew/lib -Wall -lraylibWin64 -lglew32 -lraylib -lopengl32  -lgdi32 -g -Wno-unused-function -I libs/glew/include -I libs/raylib/include
else
FLAGS=-L libs/raylib/lib/ -Wall -lraylib -lm -lpthread -ldl -lrt -lX11 -lGL -lGLEW -g -Wno-unused-function
endif

all:
	$(COMPILER) -std=c++14 $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"