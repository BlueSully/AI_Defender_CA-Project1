#include "Game.h"
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

	m_abductor1 = new Abductor(sf::Vector2f(0, 300), sf::Vector2f(m_windowScreen->getSize()));
	m_abductor2 = new Abductor(sf::Vector2f(300, 300), sf::Vector2f(m_windowScreen->getSize()));

	m_abductor1->setWorldRectangle(m_worldBackground[0].getPosition(), m_worldSize);
	m_abductor2->setWorldRectangle(m_worldBackground[0].getPosition(), m_worldSize);

	m_playerShip.setPosition(m_camera->getView().getCenter());

	m_camera->setTargetPlayer(&m_playerShip);


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

	m_abductor1->update(elapsedTime, m_playerShip.getBoundingBox());
	m_abductor1->flock(m_abductor2->getBoundingBox());
}

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));

	//render Scene
	renderer.setView(m_camera->getView());

	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}

	m_playerShip.render(renderer);

	m_abductor1->render(renderer);
	m_abductor2->render(renderer);
	 
	renderer.display();
}
