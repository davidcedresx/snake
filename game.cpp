# include <game.hpp>
# include <color.hpp>
# include <cstdlib>
# include <string>

Game::Game() :
	m_lives(3),
	m_score(0),
	m_direction(Direction::Right),
	m_snake()
{
	m_board.fill(Slot::Empty);
	m_board[8*16+7] = Slot::Body;
	m_board[8*16+8] = Slot::Body;
	m_board[8*16+9] = Slot::Body;
	m_snake.emplace_back(7, 8);
	m_snake.emplace_back(8, 8);
	m_snake.emplace_back(9, 8);
	makeFruit();
}

void Game::reset()
{
	Game new_game;
	std::swap(*this, new_game);
}

bool Game::step()
{

	auto wrongMove = [] (Direction a, Direction b)
	{
		return (a == Direction::Up and b == Direction::Down)
			or (a == Direction::Down and b == Direction::Up)
			or (a == Direction::Left and b == Direction::Right)
			or (a == Direction::Right and b == Direction::Left);
	};

	auto new_direction = readDirectionFromKeyboard();
	if (new_direction == Direction::None or wrongMove(new_direction, m_direction))
		new_direction = m_direction;

	unsigned x, y;
	std::tie(x, y) = m_snake.back();

	switch (new_direction)
	{
		case Direction::Up:
		--y;
		y %= 16;
		break;
		case Direction::Down:
		++y;
		y %= 16;
		break;
		case Direction::Left:
		--x;
		x %= 16;
		break;
		case Direction::Right:
		++x;
		x %= 16;
		break;
	}

	m_direction = new_direction;
	return moveSnake(x, y);
}

bool Game::moveSnake(unsigned x, unsigned y)
{

	switch(m_board[y*16+x])
	{
		case Slot::Body:
		{
			if (m_lives == 0)
			{
				std::clog << clr::red << "game over \n";
				throw -1;
			}

			--m_lives;
			std::clog << clr::red << "life lost \n";

			m_snake.emplace_back(x, y);
			m_board[y*16+x] = Slot::Body;

			auto back = m_snake.front();
			m_board[std::get<1>(back)*16 + std::get<0>(back)] = Slot::Empty;
			m_snake.erase(m_snake.begin());
		}
		break;

		case Slot::Fruit:
		m_snake.emplace_back(x, y);
		m_board[y*16+x] = Slot::Body;
		makeFruit();
		m_score += 1;
		return true;
		break;

		case Slot::Empty:
		{
			m_snake.emplace_back(x, y);
			m_board[y*16+x] = Slot::Body;

			auto back = m_snake.front();
			m_board[std::get<1>(back)*16 + std::get<0>(back)] = Slot::Empty;
			m_snake.erase(m_snake.begin());
		}
		break;
	}
	return false;
}

void Game::draw(sf::RenderWindow & window, sf::Font & font)
{
	for (unsigned row = 0; row < 16; ++row)
		for (unsigned col = 0; col < 16; ++col)
		{
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(24, 24));

			Slot slot = m_board[row*16+col];
			switch (slot)
			{
				case Slot::Empty:
				rectangle.setFillColor(sf::Color(52, 73, 94));
				break;
				case Slot::Body:
				rectangle.setFillColor(sf::Color(46, 204, 113));
				break;
				case Slot::Fruit:
				rectangle.setFillColor(sf::Color(231, 76, 60));
				break;
			}

			rectangle.setPosition(col*24, row*24);
			window.draw(rectangle);
		}

	sf::Text text;
	text.setCharacterSize(14);
	text.setFont(font);
	text.setPosition(4, 4);

	text.setString(std::to_string(m_score) + " points");
	window.draw(text);

	text.move(0, 18);
	text.setString(std::to_string(m_lives) + " lives");
	window.draw(text);
}

void Game::makeFruit()
{
	bool set = false;
	while (!set)
	{
		auto x = rand()%16;
		auto y = rand()%16;
		Slot & slot = m_board[y*16+x];
		if (slot == Slot::Empty)
		{
			slot = Slot::Fruit;
			set = true;
			std::clog << clr::green << "fruit appeared at (" << x << ", " << y << ")\n";
		}
	}
}

Game::Direction Game::readDirectionFromKeyboard() const
{
	Game::Direction moveNextTo = Direction::None;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)
		or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		moveNextTo = Direction::Up;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
		or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		moveNextTo = Direction::Down;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)
		or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		moveNextTo = Direction::Left;

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
		or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		moveNextTo = Direction::Right;

	return moveNextTo;
}	