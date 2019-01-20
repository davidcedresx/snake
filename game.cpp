# include <game.hpp>
# include <color.hpp>
# include <cstdlib>

Game::Game() :
	m_lives(3),
	m_score(0),
	m_direction(Direction::Right),
	m_snake()
{
	m_board.fill(Slot::Empty);
	m_board[16*64+31] = Slot::Body;
	m_board[16*64+32] = Slot::Body;
	m_board[16*64+33] = Slot::Body;
	m_snake.emplace_back(31, 16);
	m_snake.emplace_back(32, 16);
	m_snake.emplace_back(33, 16);
	makeFruit();
}

void Game::reset()
{
	Game new_game;
	std::swap(*this, new_game);
}

bool Game::step()
{

	auto goesBackwards = [] (Direction a, Direction b)
	{
		return a == Direction::Up and b == Direction::Down 
			or a == Direction::Down and b == Direction::Up
			or a == Direction::Left and b == Direction::Right
			or a == Direction::Right and b == Direction::Left;
	};

	Direction new_direction = readDirectionFromKeyboard();
	if (new_direction == Direction::None)
		new_direction = m_direction;
	else if (goesBackwards(new_direction, m_direction))
		new_direction = m_direction;

	unsigned x, y;
	std::tie(x, y) = m_snake.back();

	switch (new_direction)
	{
		case Direction::Up:
		--y;
		y %= 32;
		break;
		case Direction::Down:
		++y;
		y %= 32;
		break;
		case Direction::Left:
		--x;
		x %= 64;
		break;
		case Direction::Right:
		++x;
		x %= 64;
		break;
	}

	m_direction = new_direction;
	return moveSnake(x, y);
}

bool Game::moveSnake(unsigned x, unsigned y)
{

	switch(m_board[y*64+x])
	{
		case Slot::Body:
		reset();
		break;

		case Slot::Fruit:
		m_snake.emplace_back(x, y);
		m_board[y*64+x] = Slot::Body;
		makeFruit();
		m_score += 10;
		return true;
		break;

		case Slot::Empty:
		m_snake.emplace_back(x, y);
		m_board[y*64+x] = Slot::Body;

		auto back = m_snake.front();
		m_board[std::get<1>(back)*64 + std::get<0>(back)] = Slot::Empty;
		m_snake.erase(m_snake.begin());
		break;
	}
	return false;
}

void Game::draw(sf::RenderWindow & window)
{
	for (unsigned row = 0; row < 32; ++row)
		for (unsigned col = 0; col < 64; ++col)
		{
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(15, 15));

			Slot slot = m_board[row*64+col];
			switch (slot)
			{
				case Slot::Empty:
				rectangle.setFillColor(sf::Color::Black);
				break;
				case Slot::Body:
				rectangle.setFillColor(sf::Color::White);
				break;
				case Slot::Fruit:
				rectangle.setFillColor(sf::Color::Red);
				break;
			}

			rectangle.setPosition(col*15, row*15);
			window.draw(rectangle);
		}
}

void Game::makeFruit()
{
	bool set = false;
	while (!set)
	{
		auto x = rand()%64;
		auto y = rand()%32;
		Slot & slot = m_board[y*64+x];
		if (slot == Slot::Empty)
		{
			slot = Slot::Fruit;
			set = true;
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