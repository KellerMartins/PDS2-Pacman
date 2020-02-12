COMPILER = g++
OBJS = $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
APPNAME = Pacman

FLAGS= -std=c++14 -L third_party/raylib/lib/ -Wall -Wno-unused-function -I third_party/raylib/include -I third_party/glad/include

ifeq ($(OS),Windows_NT)
LIBS = -lraylibWin64 -lopengl32 -lgdi32 -static source/icon.res
release: FLAGS += -mwindows
else
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif

all:
	$(COMPILER) -g $(OBJS) $(FLAGS) $(LIBS) -o $(APPNAME)
	@echo "Build Finished!"

release:
	$(COMPILER) -O3 -DNDEBUG $(OBJS) $(FLAGS) $(LIBS) -o $(APPNAME)
	@echo "Build Finished!"

wasm:
	-mkdir html5
	emcc -O3 -DNDEBUG $(OBJS) $(FLAGS) -s USE_GLFW=3 -s NO_EXIT_RUNTIME=0 -s WASM=1 -Wall --emrun --preload-file assets -s ALLOW_MEMORY_GROWTH=1 third_party/raylib/lib/libraylib.bc --shell-file source/index.html -o html5/pacman.html
ifeq ($(OS),Windows_NT)
	ren html5\pacman.html index.html
else
	mv html5/pacman.html html5/index.html
endif

clean:
ifeq ($(OS),Windows_NT)
	-del /q $(APPNAME).exe
	-del html5\index.html
	-del html5\pacman.js
	-del html5\pacman.data
	-del html5\pacman.wasm
	-rmdir html5
else
	-rm -f $(APPNAME)
	-rm -f html5/index.html
	-rm -f html5/pacman.js
	-rm -f html5/pacman.data
	-rm -f html5/pacman.wasm
	-rmdir html5
endif