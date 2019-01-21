# include <SFML/Graphics.hpp>
# include <color.hpp>
# include <game.hpp>

int main()
{
	// window setup
	sf::RenderWindow window(sf::VideoMode(16*24, 16*24), "Snake");
	window.setFramerateLimit(60);

	// font setup
	sf::Font font;
	if (!font.loadFromFile("font/8-BIT WONDER.TTF"))
		throw -1;

	// difficulty settings
	float difficulty = 0.f;
	float fps_hard = 15.f;
	float fps_easy =  10.f;
	short snacks_to_go_pro = 10;

	Game game;
	while (window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		if (game.step())
		{
			// increase difficulty
			if (difficulty != 1.f)
				difficulty += difficulty / snacks_to_go_pro;
		}

		game.draw(window, font);
		window.display();

		float sleep_time = 1/((fps_hard-fps_easy)*difficulty+fps_easy);
		sf::sleep(sf::seconds(sleep_time));
	}

	return 0;
}
