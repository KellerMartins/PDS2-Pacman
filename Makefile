
COMPILER = g++
OBJS = $(wildcard source/*.cpp)
APPNAME = Pacman

FLAGS= -std=c++14 -L libs/raylib/lib/ -Wall -Wno-unused-function -I libs/raylib/include

ifeq ($(OS),Windows_NT)
FLAGS +=-L libs/glew/lib -lraylibWin64 -lglew32 -lopengl32  -lgdi32 -I libs/glew/include 
else
FLAGS += -lraylib -lm -lpthread -ldl -lrt -lX11 -lGL -lGLEW
endif

all:
	$(COMPILER) -g -D_DEBUG $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"

release:
	$(COMPILER) -O3 $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"