#include "Game.h"
#include <iostream>

Game::Game() : m_isGameRunning(true), numOfScreens(5)
{

}

Game::Game(sf::RenderWindow & window) : m_isGameRunning(true), numOfScreens(9)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_windowScreen = &window;

	m_worldSize = sf::Vector2f(static_cast<float>(m_windowScreen->getSize().x * numOfScreens), static_cast<float>(m_windowScreen->getSize().y));

	m_camera = new Camera(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(m_windowScreen->getSize()), false, true);

	m_playerShip.setPosition(m_camera->getView().getCenter());
	m_camera->setTargetPlayer(&m_playerShip);

	sizeX = (m_windowScreen->getSize().x * numOfScreens / m_windowScreen->getSize().x);

	for (size_t i = 0; i < sizeX; i++)
	{
		sf::RectangleShape temp(sf::Vector2f(m_windowScreen->getSize()));
		temp.setPosition(static_cast<float>(-m_worldSize.x / 2 + (i * temp.getSize().x)), static_cast<float>(m_windowScreen->getSize().y - 50));

		if (i == 0 || i == sizeX - 1)//furthest right and furthest left must be the same Value or screen warping looks sloppy
		{
			temp.setFillColor(sf::Color(255, 0, 0));
		}
		else
		{
			temp.setFillColor(sf::Color(0, rand() % 235 + 10, rand() % 235 + 10));
		}

		m_testBackground.push_back(temp);
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

void Game::cameraWorldWrapping()
{
	if ((m_camera->getView().getCenter().x - m_camera->getView().getSize().x / 2) < -m_worldSize.x / 2) // if camera can't move more left warp everything on screen to right side of world
	{
		std::cout << "Warp to Right" << std::endl;
		m_playerShip.setPosition(sf::Vector2f((m_worldSize.x / 2) - m_camera->getView().getSize().x / 2, m_playerShip.getPosition().y));
	}
	else if ((m_camera->getView().getCenter().x + m_camera->getView().getSize().x / 2) > m_worldSize.x / 2) // if camera can't move more right warp everything on screen to left side of world
	{
		std::cout << "Warp to Left" << std::endl;
		m_playerShip.setPosition(sf::Vector2f((-m_worldSize.x / 2) + m_camera->getView().getSize().x / 2, m_playerShip.getPosition().y));
	}
}

void Game::update()
{
	sf::Time elapsedTime = m_clock.restart();
	getInput();
	
	cameraWorldWrapping();
	m_camera->Update(m_worldSize);
	m_playerShip.boundaryResponse(m_worldSize);
	m_playerShip.update(elapsedTime);
}

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));

	//render Scene
	renderer.setView(m_camera->getView());
	for (size_t i = 0; i < sizeX; i++)
	{
		renderer.draw(m_testBackground[i]);
	}
	m_playerShip.render(renderer);

	renderer.display();
}
