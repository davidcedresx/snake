# pragma once

# include <SFML/Graphics.hpp>

# include <vector>
# include <tuple>

class Game
{
public:
	enum class Slot { Empty, Body, Fruit };
	enum class Direction { None, Up, Down, Left, Right };

public:
	Game();
	Game(const Game &) = default;
	~Game() = default;

	// runs the game, one step at a time, returns if the snake catched a fruit
	bool step();
	// draws the board and the snake
	void draw(sf::RenderWindow & window, sf::Font & font);

private:
	// resets the game state
	void reset();
	// returns true if the snake catched a fruit
	bool moveSnake(unsigned x, unsigned y);
	// puts a new fruit in the board
	void makeFruit();
	// check for keystrokes and returns directions
	Direction readDirectionFromKeyboard() const;

private:
	std::array <Slot, 16*16> m_board;
	unsigned m_lives;
	unsigned m_score;
	Direction m_direction = Direction::Right;
	std::vector<std::pair<unsigned, unsigned>> m_snake;
};
