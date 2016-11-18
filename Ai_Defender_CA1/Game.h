#pragma once

#include "Player.h"

class Game
{
private:

public:
	Game();
	~Game();

	void update();
	void render(sf::RenderWindow &renderer);
};

