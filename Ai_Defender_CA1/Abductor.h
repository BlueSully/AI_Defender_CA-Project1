#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include "AiStates.h"

class Abductor
{
private :
	sf::RectangleShape m_boundingBox;
	sf::Vector2f m_velocity;
	sf::Vector2f m_steering;
	sf::Vector2f m_abducteePosition;

	AiState state;
public:
	Abductor();
	~Abductor();

	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f value);
	void setAbductorTarget(sf::Vector2f target);
	void setState(AiState value);

	void update();
	void flock();
	void patrol();

	void attack(sf::Vector2f);

	void render(sf::RenderWindow & renderer);
};

