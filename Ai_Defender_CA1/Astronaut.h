#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include "MathFunction.h"

#define _USE_MATH_DEFINES

enum AiState
{
	WANDER,
	FLEE
};

class Astronaut
{
private:
	sf::RectangleShape m_boundingBox;
	sf::CircleShape m_fleeCircle;
	sf::CircleShape targetCircle;
	sf::Vector2f m_velocity;
	sf::Vector2f m_steering;
	sf::Vector2f m_fleeFromPos;

	float m_wanderAngle;
	float m_wanderDelayTimer;

	const float MAX_FORCE = 5.4f;
	const float MAX_VELOCITY = 10;
	const float CIRCLEDISTANCE = 6;
	const float CIRCLE_RADIUS = 8;
	const float ANGLECHANGE = 1;

	AiState state;
	bool m_wander;
	bool m_flee;
public:
	Astronaut();
	~Astronaut();

	//Get Method
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f value);
	void setFleeingTarget(sf::Vector2f);
	void setState(std::string str);

	//Methods
	void init(sf::Vector2f position, sf::Vector2f veclocity);
	sf::Vector2f wander();
	sf::Vector2f flee();

	void fleeCollisionCheck(sf::Vector2f value);
	void boundaryResponse(sf::Vector2f worldSize);
	void update(sf::Time elapsedTime);
	void render(sf::RenderWindow & renderer);
};

