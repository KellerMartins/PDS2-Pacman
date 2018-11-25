
COMPILER = g++
OBJS = $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
APPNAME = Pacman

FLAGS= -std=c++14 -L third_party/raylib/lib/ -Wall -Wno-unused-function -I third_party/raylib/include -I third_party/glad/include

ifeq ($(OS),Windows_NT)
FLAGS += -lraylibWin64 -lopengl32 -lgdi32 -static
release: FLAGS += -mwindows
else
FLAGS += -lraylib -lm -lpthread -ldl -lrt -lX11
endif

all:
	$(COMPILER) -g $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"

release:
	$(COMPILER) -O3 -DNDEBUG $(OBJS) $(FLAGS) -o $(APPNAME)
	@echo "Build Finished!"