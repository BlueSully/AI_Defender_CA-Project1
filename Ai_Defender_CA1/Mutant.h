#pragma once

#include "MathFunction.h"
#include "AiStates.h"

class Mutant
{
private:
	sf::RectangleShape m_boundingBox;//Bounding Box for Mutant
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;

	const float MAX_VELOCITY = 25;

	AiState m_state;

public:
	Mutant();
	Mutant(sf::Vector2f position, sf::Vector2f velocity);
	~Mutant();

	sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f value);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f value);

	sf::Vector2f seek(sf::Vector2f playerPostion);
	void swarm();

	void update(sf::Time deltaTime, sf::Vector2f playerPostion);

	void render(sf::RenderWindow & renderer);
};

