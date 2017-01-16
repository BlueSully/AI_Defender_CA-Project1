#include "Game.h"

Game::Game() : m_isGameRunning(true), m_numOfScreens(5)
{

}

Game::Game(sf::RenderWindow & window) : m_isGameRunning(true), m_numOfScreens(9)
{
	srand(static_cast<unsigned int>(time(NULL)));
	m_windowScreen = &window;

	m_worldSize = sf::Vector2f(static_cast<float>(m_windowScreen->getSize().x * m_numOfScreens), static_cast<float>(m_windowScreen->getSize().y));

	m_screenWidth = (m_windowScreen->getSize().x * m_numOfScreens / m_windowScreen->getSize().x);

	for (size_t i = 0; i < 4; i++)
	{
		m_astronauts.push_back(new Astronaut(sf::Vector2f(100 * i, m_windowScreen->getSize().y), sf::Vector2f(0, 0)));
	}

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
		m_abductors.push_back(new Abductor(sf::Vector2f(300 * i, 300), sf::Vector2f(m_windowScreen->getSize()), (int)i, 32));
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

//Function to Manage astronauts within gameworld
void Game::manageHumans(sf::Time elapsedTime)
{
	float fleeingDistance = 350;
	for (size_t i = 0; i < m_astronauts.size(); i++)
	{
		if (m_astronauts[i]->getState() != GRABBED)
		{
			m_astronauts[i]->boundaryResponse(m_worldSize);

			//Check if id isn't out of range
			if (m_astronauts[i]->getAbuctorId() >= 0) 
			{
				//if distance from pursuer is greater that the fleeing distance go back wandering
				float distance = VectorHelper::distanceBetweenTwoVectors(m_abductors[m_astronauts[i]->getAbuctorId()]->getPosition(), m_astronauts[i]->getPosition());

				if (distance <= fleeingDistance && m_astronauts[i]->getState() != FLEE)
				{
					m_astronauts[i]->setState(FLEE);			
					m_astronauts[i]->setFleeingTarget(&m_abductors[i]->getPosition());

					if (m_abductors[m_astronauts[i]->getAbuctorId()]->getPosition().x > m_astronauts[i]->getPosition().x)
					{
						m_astronauts[i]->setFleeDirection(0);
					}
					else if (m_abductors[m_astronauts[i]->getAbuctorId()]->getPosition().x < m_astronauts[i]->getPosition().x)
					{
						m_astronauts[i]->setFleeDirection(1);
					}
				}
				else if (distance > fleeingDistance && m_astronauts[i]->getState() != WANDER)
				{
					m_astronauts[i]->setAbductorId(-1);
					m_astronauts[i]->setBeingAbducted(false);
					m_astronauts[i]->setState(WANDER);
				}
			}
		}
		m_astronauts[i]->update(elapsedTime);
	}
	cout << "veloc: " << m_astronauts[0]->getVelocity().x << ' ' << m_astronauts[0]->getVelocity().y << endl;
}

//Function to Manage abductors within gameworld
void Game::manageAbductors(sf::Time elapsedTime)
{
	for (size_t i = 0; i < 2; i++)
	{
		std::map<int, float> humanDist;//humanDist : Get all distances to nearest humans

		for (size_t j = 0; j < m_astronauts.size(); j++)//Look for nearest unmarked human
		{
			float distance = VectorHelper::distanceBetweenTwoVectors(m_abductors[i]->getPosition(), m_astronauts[j]->getPosition());
			
			humanDist[j] = distance;
		}

		if (humanDist.size() > 1 && m_abductors[i]->getState() != ABDUCTING)
		{
			std::pair<int, float> min = *min_element(humanDist.begin(), humanDist.end(), compareValues());

			if (min.second < 300 && !(m_astronauts[min.first]->getBeingAbducted()))//Marking unmarked human as targeted
			{
				m_abductors[i]->setState(ABDUCTING);
				m_abductors[i]->setAbducteeId(min.first);
				m_astronauts[min.first]->setBeingAbducted(true);
				m_astronauts[humanDist.begin()->first]->setAbductorId(i);
			}
		}

		if (m_abductors[i]->getState() != ABDUCTING)
		{
			m_abductors[i]->flock(&m_abductors);//flock if not grabbing human
		}
		else if (m_abductors[i]->getState() == ABDUCTING)
		{
			if (m_abductors[i]->getAbucteeId() >= 0 && m_abductors[i]->getAbucteeId() < m_astronauts.size()) //Making sure the id isn't a junk value or out of bounds
			{
				int id = m_abductors[i]->getAbucteeId();
				m_abductors[i]->abduct(elapsedTime, &m_astronauts[id]->getPosition());
				if (CollisionHelper::RectangleCollision(m_abductors[i]->getPosition(), m_abductors[i]->getSize(), m_astronauts[id]->getPosition(), m_astronauts[id]->getSize()))
				{
					m_abductors[i]->setGrabbedAstronaut(true);
					m_astronauts[id]->setFollowTarget((m_abductors[i]->getPosition()), m_abductors[i]->getSize());
					m_astronauts[id]->setState(GRABBED);
				}
			}
		}

		m_abductors[i]->update(elapsedTime, m_playerShip.getBoundingBox());
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

	manageAbductors(elapsedTime);
	manageHumans(elapsedTime);
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

void Game::render(sf::RenderWindow &renderer)
{
	renderer.clear(sf::Color(0, 0, 0, 255));

	//render Scene
	renderer.setView(m_camera->getView());

	m_playerShip.render(renderer);

	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}

	for (size_t i = 0; i < m_astronauts.size(); i++)
	{
		m_astronauts[i]->render(renderer);
	}

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->render(renderer);
	}

	//Render mini-map
	renderer.setView(m_camera->getRadar());
	
	m_playerShip.render(renderer);

	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}

	for (size_t i = 0; i < m_astronauts.size(); i++)
	{
		m_astronauts[i]->render(renderer);
	}

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->render(renderer);
	}

	renderer.display();
}
