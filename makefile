CXX = clang++ -std=c++11 -g
BINARY = snake
SFML_LINKS = -lsfml-window -lsfml-system -lsfml-graphics

all: snake

snake: game.o main.o
	$(CXX) $^ -o $@ $(SFML_LINKS)

game.o: game.cpp game.hpp
	$(CXX) -c $< -I.

main.o: main.cpp
	$(CXX) -c $< -I.

clean:
	$(RM) game.o main.o $(BINARY)
	
