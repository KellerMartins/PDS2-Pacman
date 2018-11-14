
COMPILER = g++
OBJS = $(wildcard source/*.cpp) $(wildcard source/*/*.cpp)
APPNAME = Pacman

FLAGS= -std=c++14 -L third_party/raylib/lib/ -Wall -Wno-unused-function -I third_party/raylib/include

ifeq ($(OS),Windows_NT)
FLAGS +=-L third_party/glew/lib -lraylibWin64 -lglew32 -lopengl32  -lgdi32 -I third_party/glew/include 
else
FLAGS += -lraylib -lm -lpthread -ldl -lrt -lX11 -lGL -lGLEW
endif

all:
	$(COMPILER) -g $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"

release:
	$(COMPILER) -O3  -DNDEBUG $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"