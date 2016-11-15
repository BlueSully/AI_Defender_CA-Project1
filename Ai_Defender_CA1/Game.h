#pragma once
#include <SFML/Graphics.hpp>
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

