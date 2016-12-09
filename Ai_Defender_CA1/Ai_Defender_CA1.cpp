#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"

int main()
{
	sf::VideoMode screen = sf::VideoMode::getDesktopMode();

	sf::RenderWindow m_window(sf::VideoMode(800, 600, 32), "AI Defender Project", sf::Style::Default);

	Game m_game(m_window);

	while (!m_game.isGameRunning())
	{
		m_game.update();
		m_game.render(m_window);
	}

	return 0;
}