CXX = g++
FLAGS = -O3 -Wall -g -std=c++2a
BIN = bin/*.o
FILES = main.cpp src/Window/Window.cpp src/BUS/BUS.cpp src/CPU/Instructions.cpp src/CPU/AddressingModes.cpp src/CPU/CPU.cpp src/RAM/RAM.cpp
INCLUDE_DIRS = -Ivendor/glad/include -Ivendor/imgui
LINKER_FLAGS = -lm -lGL -ldl -lglfw -lfmt
OUT = APP
build:
	$(CXX) $(FLAGS) $(INCLUDE_DIRS) $(BIN) $(FILES) $(LINKER_FLAGS) -o $(OUT)
build_bin:
	$(CXX) $(FLAGS) -c $(INCLUDE_DIRS) vendor/imgui/*.cpp vendor/imgui/backends/imgui_impl_glfw.cpp vendor/imgui/backends/imgui_impl_opengl3.cpp glad/src/glad.c $(LINKER_FLAGS) && mkdir bin && mv *.o bin
clean:
	rm -rf bin