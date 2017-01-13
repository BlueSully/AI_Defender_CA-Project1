﻿#include "Game.h"
#include <iostream>

Game::Game() : m_isGameRunning(true), m_numOfScreens(5)
{

}

Game::Game(sf::RenderWindow & window) : m_isGameRunning(true), m_numOfScreens(9)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_windowScreen = &window;

	m_worldSize = sf::Vector2f(static_cast<float>(m_windowScreen->getSize().x * m_numOfScreens), static_cast<float>(m_windowScreen->getSize().y));

	m_screenWidth = (m_windowScreen->getSize().x * m_numOfScreens / m_windowScreen->getSize().x);

	m_astronaut.init(sf::Vector2f(100, (m_windowScreen->getSize().y - m_astronaut.getSize().y) - 10), sf::Vector2f(0, 0));

	for (size_t i = 0; i < m_screenWidth; i++)
	{
		sf::RectangleShape screenBackground(sf::Vector2f(m_windowScreen->getSize()));
		screenBackground.setPosition(static_cast<float>(-m_worldSize.x / 2 + (i * screenBackground.getSize().x)), static_cast<float>(m_windowScreen->getSize().y - 50));

		if (i == 0 || i == m_screenWidth - 1)//furthest right and furthest left must be the same Value or screen warping looks sloppy
		{
			screenBackground.setFillColor(sf::Color(255, 0, 0));
		}
		else
		{
			screenBackground.setFillColor(sf::Color(0, rand() % 235 + 10, rand() % 235 + 10));
		}

		m_worldBackground.push_back(screenBackground);
	}

	m_camera = new Camera(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(m_windowScreen->getSize()), false, true);

	m_playerShip.setPosition(m_camera->getView().getCenter());

	m_camera->setTargetPlayer(&m_playerShip);

	for (size_t i = 0; i < 4; i++)
	{
		m_abductors.push_back(new Abductor(sf::Vector2f(300 * i, 300), sf::Vector2f(m_windowScreen->getSize()), (int)i, 35));
		m_abductors[i]->setWorldRectangle(m_worldBackground[0].getPosition(), m_worldSize);
	}
	m_abductors[1]->setColour(sf::Color(0, 150, 0));
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

//used to create a seamless transition from side to side as the player travels
void Game::cameraWorldWrapping()
{
	if ((m_camera->getView().getCenter().x - m_camera->getView().getSize().x / 2) < (m_worldBackground.front().getPosition().x)) // if camera can't move more left warp everything on screen to right side of world
	{
		std::cout << "Warp to Right" << std::endl;
		m_playerShip.setPosition(sf::Vector2f(m_worldBackground.back().getPosition().x + m_worldBackground.back().getSize().x - m_camera->getView().getSize().x / 2, m_playerShip.getPosition().y));//Move player to center of screen on right side of the world
	}
	else if ((m_camera->getView().getCenter().x + m_camera->getView().getSize().x / 2) > (m_worldBackground.back().getPosition().x + m_worldBackground.back().getSize().x)) // if camera can't move more right warp everything on screen to left side of world
	{
		std::cout << "Warp to Left" << std::endl;
		m_playerShip.setPosition(sf::Vector2f((m_worldBackground.front().getPosition().x + m_camera->getView().getSize().x / 2), m_playerShip.getPosition().y));
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

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->update(elapsedTime, m_playerShip.getBoundingBox());
		
		//if (i + 1 < m_abductors.size()) 
		//{
		//	//Check for Other Abductors within range
		//	if (VectorHelper::distanceBetweenTwoVectors(m_abductors[i]->getPosition(), m_abductors[i + 1]->getPosition()) < 150)
		//	{
				m_abductors[i]->flock(&m_abductors);
			//}
		//}
	}
	cout << "Velo: " << m_abductors[0]->getVelocity().x << ' ' << m_abductors[0]->getVelocity().y << endl;
}

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));

	renderer.clear(sf::Color(0, 0, 0, 255));

	//render Scene
	renderer.setView(m_camera->getView());

	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}
	m_astronaut.render(renderer);
	m_playerShip.render(renderer);

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->render(renderer);
	}

	//Render mini-map
	renderer.setView(m_camera->getRadar());
	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}

	m_astronaut.render(renderer);
	m_playerShip.render(renderer);

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->render(renderer);
	}

	 
	renderer.display();
}
