#pragma once
#include "Player.h"
#include "Camera.h"
#include "Abductor.h"
#include <vector>
#include <SFML\Graphics.hpp>

using namespace std;

class Game
{
private:
	Player m_playerShip;
	vector<Abductor *> m_abductors;

	sf::RenderWindow * m_windowScreen;
	Camera * m_camera;
	sf::View m_minimapView;

	sf::Event m_event;
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

