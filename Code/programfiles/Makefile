collisionTree.o: collisionTree.cpp collisionTree.hpp
	clang++ -c collisionTree.cpp -std=c++14

collision.o: collision.cpp
	clang++ -c collision.cpp -Wall -I/usr/include/SDL2 -std=c++14

test: collision.o collisionTree.o
	clang++ -o test  collision.o collisionTree.o -lSDL2 -lSDL2_image -lSDL2_ttf -std=c++14