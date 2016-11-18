#pragma once

#include "Player.h"

class Game
{
private:
	Player m_playerShip;
	sf::RenderWindow * m_windowScreen;
	sf::Event m_event;
	bool m_isGameRunning;

	sf::Clock m_clock;
	sf::Time m_deltatime;

public:
	Game();
	Game(sf::RenderWindow &window);
	~Game();

	bool isGameRunning() const;
	void getInput();
	void update();
	void render(sf::RenderWindow &renderer);
};

