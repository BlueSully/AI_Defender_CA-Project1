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

	int centerId = m_numOfScreens / 2;
	sf::Vector2f centerOfWorld = sf::Vector2f(m_worldBackground[centerId].getPosition().x, 0);

	m_camera = new Camera(centerOfWorld, static_cast<sf::Vector2f>(m_windowScreen->getSize()), false, true);

	m_playerShip.setPosition(m_camera->getView().getCenter());

	m_camera->setTargetPlayer(&m_playerShip);

	const int minPosition = static_cast<const int>(m_worldBackground[0].getPosition().x);
	const int maxPosition = static_cast<const int>(m_worldBackground[m_numOfScreens - 1].getPosition().x);

	for (int i = 0; i < 5; i++)
	{
		//Getting a random value between each side of the world
		float locationX = minPosition + (rand() * (float)(maxPosition - minPosition) / RAND_MAX);

		sf::Vector2f positon = sf::Vector2f(locationX, (float)m_windowScreen->getSize().y);
		m_astronauts.push_back(new Astronaut(positon, sf::Vector2f(0, 0)));
	}

	for (size_t i = 0; i < 5; i++)
	{
		float locationX = minPosition + (rand() * (float)(maxPosition - minPosition) / RAND_MAX);

		m_abductors.push_back(new Abductor(sf::Vector2f(locationX, 300), sf::Vector2f(m_windowScreen->getSize()), (int)i, 32));
		m_abductors[i]->setWorldRectangle(m_worldBackground[0].getPosition(), m_worldSize);
		m_abductors[i]->setColour(sf::Color(0, rand() % 235 + 10, rand() % 235 + 10));
	}
	
	for (size_t i = 0; i < 1; i++)
	{
		m_mutants.push_back(new Mutant(sf::Vector2f(250, 200), sf::Vector2f(0, 0)));
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

//Function to Manage astronauts within gameworld
void Game::manageHumans(sf::Time elapsedTime)
{
	float fleeingDistance = 300;
	for (int i = 0; i < m_astronauts.size(); i++)
	{
		bool canWander = false;
		std::map<int, float> alienDist;//humanDist : Get all distances to nearest humans
		if (m_astronauts[i]->getState() != GRABBED)
		{
			m_astronauts[i]->boundaryResponse(m_worldSize);

			for (int j = 0; j < m_abductors.size(); j++)
			{
				float distance = VectorHelper::distanceBetweenTwoVectors(m_abductors[j]->getPosition(), m_astronauts[i]->getPosition());

				alienDist[j] = distance;
			}

			if (alienDist.size() > 1)
			{
				std::pair<int, float> min = *min_element(alienDist.begin(), alienDist.end(), compareValues());

				if (min.second < fleeingDistance)
				{
					m_astronauts[i]->setState(FLEE);
					m_astronauts[i]->setFleeTarget(&m_abductors[min.first]->getPosition());
				}
			}

			//Check distances from aliens if any are within range don't go back to wandering
			for (int i = 0; i < alienDist.size(); i++)
			{
				if (alienDist[i] > fleeingDistance)
				{
					canWander = true;
				}
				else
				{
					canWander = false;
					break;
				}
			}

			//if distance from pursuer is greater that the fleeing distance go back wandering
			if (canWander)
			{
				m_astronauts[i]->setAbductorId(-1);
				m_astronauts[i]->setBeingAbducted(false);
				m_astronauts[i]->setState(WANDER);
			}

		}
		m_astronauts[i]->update(elapsedTime);
	}
}

void Game::manageMutants(sf::Time elapsedTime)
{
	for (size_t i = 0; i < m_mutants.size(); i++)
	{
		m_mutants[i]->update(elapsedTime, m_playerShip.getPosition());
	}
}

//Function to Manage abductors within gameworld
void Game::manageAbductors(sf::Time elapsedTime)
{
	for (int i = 0; i < m_abductors.size(); i++)
	{
		std::map<int, float> humanDist;//humanDist : Get all distances to nearest humans

		if (m_abductors[i]->getState() != ABDUCTING) 
		{
			for (int j = 0; j < m_astronauts.size(); j++)//Look for nearest unmarked human
			{
				float distance = VectorHelper::distanceBetweenTwoVectors(m_abductors[i]->getPosition(), m_astronauts[j]->getPosition());

				if (m_astronauts[j]->getBeingAbducted() == false)
					humanDist[j] = distance;
			}
		}

		if (humanDist.size() >= 1 && m_abductors[i]->getState() != ABDUCTING)
		{
			std::pair<int, float> min = *min_element(humanDist.begin(), humanDist.end(), compareValues());

			if (min.second < 500 && !(m_astronauts[min.first]->getBeingAbducted()))//Marking unmarked human as targeted
			{
				m_abductors[i]->setState(ABDUCTING);
				m_abductors[i]->setAbducteeId(min.first);
				m_astronauts[min.first]->setBeingAbducted(true);
				m_astronauts[min.first]->setAbductorId(i);
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

	//manageAbductors(elapsedTime);
	manageMutants(elapsedTime);
	//manageHumans(elapsedTime);
}

//used to create a seamless transition from side to side as the player travels
void Game::cameraWorldWrapping()
{
	if ((m_camera->getView().getCenter().x - m_camera->getView().getSize().x / 2) < (m_worldBackground.front().getPosition().x)) // if camera can't move more left warp everything on screen to right side of world
	{
		//std::cout << "Warp to Right" << std::endl;
		m_playerShip.setPosition(sf::Vector2f(m_worldBackground.back().getPosition().x + m_worldBackground.back().getSize().x - m_camera->getView().getSize().x / 2, m_playerShip.getPosition().y));//Move player to center of screen on right side of the world
	}
	else if ((m_camera->getView().getCenter().x + m_camera->getView().getSize().x / 2) > (m_worldBackground.back().getPosition().x + m_worldBackground.back().getSize().x)) // if camera can't move more right warp everything on screen to left side of world
	{
		//std::cout << "Warp to Left" << std::endl;
		m_playerShip.setPosition(sf::Vector2f((m_worldBackground.front().getPosition().x + m_camera->getView().getSize().x / 2), m_playerShip.getPosition().y));
	}
}

void Game::warpingOtherEntities()
{
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

	/*for (size_t i = 0; i < m_astronauts.size(); i++)
	{
		m_astronauts[i]->render(renderer);
	}

	for (size_t i = 0; i < m_abductors.size(); i++)
	{
		m_abductors[i]->render(renderer);
	}*/

	for (size_t i = 0; i < m_mutants.size(); i++)
	{
		m_mutants[i]->render(renderer);
	}

	//Render mini-map
	renderer.setView(m_camera->getRadar());

	for (size_t i = 0; i < m_worldBackground.size(); i++)
	{
		renderer.draw(m_worldBackground[i]);
	}

	m_playerShip.render(renderer);

	//for (size_t i = 0; i < m_astronauts.size(); i++)
	//{
	//	m_astronauts[i]->render(renderer);
	//}

	//for (size_t i = 0; i < m_abductors.size(); i++)
	//{
	//	m_abductors[i]->render(renderer);
	//}

	for (size_t i = 0; i < m_mutants.size(); i++)
	{
		m_mutants[i]->render(renderer);
	}

	renderer.display();
}
