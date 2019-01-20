# pragma once

# include <SFML/Graphics.hpp>

# include <vector>
# include <tuple>

class Game
{
// enums
public:
	enum class Slot { Empty, Body, Fruit };
	enum class Direction { None, Up, Down, Left, Right };

// methods
public:
	Game();
	Game(const Game &) = default;
	~Game() = default;

	void reset();
	bool step();
	bool moveSnake(unsigned x, unsigned y);

	void draw(sf::RenderWindow & window);

private:
	void makeFruit();
	Direction readDirectionFromKeyboard() const;

// attributes
private:
	std::array <Slot, 64*32> m_board;
	unsigned m_lives;
	unsigned m_score;
	Direction m_direction = Direction::Right;
	std::vector<std::pair<unsigned, unsigned>> m_snake;
};
