#include "Game.h"
#include <iostream>

Game::Game() : m_isGameRunning(true)
{

}

Game::Game(sf::RenderWindow & window) : m_isGameRunning(true)
{
	m_windowScreen = &window;
	m_worldSize = sf::Vector2f(m_windowScreen->getSize().x * 9, m_windowScreen->getSize().y);
	viewport = new sf::View(sf::FloatRect(0, 0, m_windowScreen->getSize().x, m_windowScreen->getSize().y));
	
	
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			sf::RectangleShape temp(sf::Vector2f(i * 50, j * 50));
			if (j % 2 == 0)
			{
				temp.setFillColor(sf::Color::Green);
			}
			else
			{
				temp.setFillColor(sf::Color::Red);
			}
			m_testBackground.push_back(temp);
		}		
	}
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
	//viewport->setCenter(m_playerShip.getPosition());
	m_windowScreen->setView(*viewport);
	m_playerShip.boundaryResponse(m_windowScreen);
	m_playerShip.update(elapsedTime);

}

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));
	
	for (size_t i = 0; i < (10 * 10); i++)
	{
		renderer.draw(m_testBackground[i]);
	}
	m_playerShip.render(renderer);

	renderer.display();
}
