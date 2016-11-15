#pragma once
#include "Game.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "AI Defender Project");

	Game game;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.KeyPressed == sf::Keyboard::Escape) 
				window.close();
		}

		game.update();
		game.render(window);
	}

	return 0;
}