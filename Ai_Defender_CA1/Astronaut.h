#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>
#include "MathFunction.h"

#define _USE_MATH_DEFINES
class Astronaut
{
private:
	sf::RectangleShape m_boundingBox;
	sf::CircleShape m_wanderDebugCircle;
	sf::Vector2f m_velocity;
	sf::Vector2f m_steering;
	sf::Vector2f m_directionVector;
	float m_wanderAngle;
	float m_orientation;
	float m_wanderDelayTimer;

	const float MAX_FORCE = 5.4f;
	const float MAX_VELOCITY = 10;
	const float CIRCLEDISTANCE = 50;
	const float CIRCLE_RADIUS = 8;
	const float ANGLECHANGE = 1;

	bool m_wander;
	bool m_flee;
public:
	Astronaut();
	~Astronaut();

	//Get Method
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f value);
	//Methods
	void init(sf::Vector2f position, sf::Vector2f veclocity);
	sf::Vector2f wander();
	sf::Vector2f wander2();
	void flee();

	void align(sf::Vector2f target);
	void face(sf::Vector2f target);
	float getNewOrientation(float currentOrientation, sf::Vector2f velocity);
	void boundaryResponse(sf::Vector2f worldSize);
	void update(sf::Time elapsedTime);
	void render(sf::RenderWindow & renderer);
};

