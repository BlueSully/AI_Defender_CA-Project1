#include "Game.h"
#include <iostream>

Game::Game() : m_isGameRunning(true)
{

}

Game::Game(sf::RenderWindow & window) : m_isGameRunning(true)
{
	m_windowScreen = &window;
}

Game::~Game()
{

}

bool Game::isGameRunning() const
{
	return false;
}

void Game::getInput()
{

	//sf::Event event;// Events handling closing of Program
	while (m_windowScreen->pollEvent(m_event))
	{

		if (m_event.type == sf::Event::Closed)
		{
			m_windowScreen->close();
		}
		else if (m_event.KeyPressed == sf::Keyboard::Escape)
		{
			m_windowScreen->close();
		}

		if (m_event.type == sf::Event::KeyPressed || m_event.type == sf::Event::KeyReleased)
		{
			m_playerShip.processInputs(&m_event);
		}	
	}
}

void Game::update()
{
	sf::Time elapsedTime = m_clock.restart();
	getInput();

	m_playerShip.boundaryResponse(m_windowScreen);
	m_playerShip.update(elapsedTime);
}

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));
	m_playerShip.render(renderer);
	renderer.display();
}
