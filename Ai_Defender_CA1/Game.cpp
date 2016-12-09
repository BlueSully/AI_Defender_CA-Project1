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

	m_astronaut.init(sf::Vector2f(200, (m_windowScreen->getSize().y - m_astronaut.getSize().y) - 300), sf::Vector2f(rand() % 100 - 100, rand() % 100 - 100));

	m_playerShip.setPosition(m_camera->getView().getCenter());
	m_camera->setTargetPlayer(&m_playerShip);

	sizeX = (m_windowScreen->getSize().x * numOfScreens / (m_windowScreen->getSize().x));//gets the size of each of the screens

	for (size_t i = 0; i < sizeX; i++)
	{
		sf::RectangleShape temp(sf::Vector2f(m_windowScreen->getSize().x, rand() % 100 + 50));

		if (i == 0 || i == (sizeX - 1))//furthest right and furthest left must be the same Value or screen warping looks sloppy
		{
			temp.setFillColor(sf::Color(255, 0, 0));
			temp.setSize(sf::Vector2f(temp.getSize().x, 200));
		}
		else
		{
			temp.setFillColor(sf::Color(rand() % 235 + 10, rand() % 235 + 10, rand() % 235 + 10));
		}

		temp.setPosition(static_cast<float>((-m_worldSize.x / 2) + (i * temp.getSize().x)), (m_windowScreen->getSize().y - temp.getSize().y));
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

//used to create a seamless transition from side to side as the player travels
void Game::cameraWorldWrapping()
{
	if ((m_camera->getView().getCenter().x - m_camera->getView().getSize().x / 2) < (m_testBackground.front().getPosition().x)) // if camera can't move more left warp everything on screen to right side of world
	{
		std::cout << "Warp to Right" << std::endl;
		m_playerShip.setPosition(sf::Vector2f(m_testBackground.back().getPosition().x + m_testBackground.back().getSize().x - m_camera->getView().getSize().x / 2, m_playerShip.getPosition().y));//Move player to center of screen on right side of the world
	}
	else if ((m_camera->getView().getCenter().x + m_camera->getView().getSize().x / 2) > (m_testBackground.back().getPosition().x + m_testBackground.back().getSize().x)) // if camera can't move more right warp everything on screen to left side of world
	{
		std::cout << "Warp to Left" << std::endl;
		m_playerShip.setPosition(sf::Vector2f((m_testBackground.front().getPosition().x + m_camera->getView().getSize().x / 2), m_playerShip.getPosition().y));
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
	debugTime += elapsedTime.asSeconds();

	m_astronaut.boundaryResponse(m_worldSize);
	//if (debugTime > 1) 
	//{
		m_astronaut.update(elapsedTime);
		//debugTime = 0;
	//}
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
	m_astronaut.render(renderer);
	m_playerShip.render(renderer);

	renderer.display();
}
