CXX=clang++
LIBS=-l SDL2 -lSDL2_ttf

test: src/*
	$(CXX) src/*.cpp $(LIBS) -std=c++17 -o bin/test
