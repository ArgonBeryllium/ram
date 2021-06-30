CXX=clang++
LIBS=-l SDL2 -lSDL2_ttf
FLAGS=-std=c++17 -g -O3

test: src/*
	$(CXX) src/*.cpp $(LIBS) $(FLAGS) -o bin/test
