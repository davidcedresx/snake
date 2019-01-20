# include <SFML/Graphics.hpp>
# include <color.hpp>
# include <game.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(32*20, 32*20), "Snake");
	window.setFramerateLimit(60);

	float delay_factor = 20.f;

	Game game;
	while (window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		if (game.step())
		{
			delay_factor = (60.0f+delay_factor)/4.f;
		}

		game.draw(window);
		window.display();
		sf::sleep(sf::seconds(1.f/delay_factor));
	}

	return 0;
}
