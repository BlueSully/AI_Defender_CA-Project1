#pragma once
#include "Player.h"
#include "Astronaut.h"
#include "Camera.h"
#include "Abductor.h"
#include "Nest.h"
#include <iostream>
#include <vector>
#include <SFML\Graphics.hpp>

using namespace std;

class Game
{
private:
	//Astronaut m_astronaut;
	Player m_playerShip;
	vector<Abductor *> m_abductors;
	vector<Nest *> m_nests;

	sf::RenderWindow * m_windowScreen;
	Camera * m_camera;
	sf::View m_minimapView;

	sf::Event m_event;
	float debugTime;
	bool m_isGameRunning;
	int m_numOfScreens;
	int m_screenWidth;

	sf::Clock m_clock;
	sf::Time m_deltatime;
	sf::View *viewport;
	sf::Vector2f m_worldSize;
	std::vector<sf::RectangleShape> m_worldBackground;

public:
	Game();
	Game(sf::RenderWindow &window);
	~Game();

	//Get Methods and Set methods
	bool isGameRunning() const;
	void getInput();

	//Methods
	void cameraWorldWrapping();
	void update();
	void render(sf::RenderWindow &renderer);
};

