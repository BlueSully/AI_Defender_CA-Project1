#pragma once
//#include <SFML\Graphics.hpp>
#include <SFML\Graphics.hpp>
class Player
{
public:
	Player();
	~Player();

	void update();
	void render(sf::RenderWindow &renderer);
};

