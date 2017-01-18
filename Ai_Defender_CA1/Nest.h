#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include "MathFunction.h"
#include "AiStates.h"
#include "ProjectileManager.h"

class Nest
{
private:
	sf::RectangleShape m_boundingBox;
	sf::CircleShape m_fleeCircle;
	sf::CircleShape targetCircle;
	sf::Vector2f m_velocity;
	sf::Vector2f m_steering;
	sf::Vector2f m_fleeFromPos;
	sf::RectangleShape m_worldBounds;//World data
	ProjectileManager projMan;
	float m_wanderAngle;
	float m_wanderDelayTimer=0;

	const float MAX_FORCE = 6.4f;
	const float MAX_VELOCITY = 10;
	const float CIRCLEDISTANCE = 6;
	const float CIRCLE_RADIUS = 8;
	const float ANGLECHANGE = 1;


	const int MAX_ABDUCTORS = 20;
	sf::Texture m_nestText;
	sf::Sprite m_nestSprite;




	AiState state;
	bool m_wander;
	bool m_flee;
	int swap = 0;
	int m_missileNum;
	int m_abductorNum;
	float m_timer;
	float m_abductorTimer;
	float m_missileTimer;

public:
	Nest(sf::Vector2f position, sf::Vector2f veclocity);
	~Nest();

	//Get Method
	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f value);
	void setWorldRectangle(sf::Vector2f postion, sf::Vector2f size);
	void setColour(sf::Color colour);
	void setFleeingTarget(sf::Vector2f);
	void setState(std::string str);

	void fleeCollisionCheck(sf::RectangleShape value, sf::Time elapsedTime);

	sf::Vector2f wander();
	sf::Vector2f flee();

	void update(sf::Time elapsedTime, sf::RectangleShape value);

	void animate(sf::Time elapsedTime);
	void spawnAbductor(sf::Time elapsedTime);
	void fireMissiles(sf::RectangleShape value, sf::Time elapsedTime);
	void boundaryResponse();

	void render(sf::RenderWindow & renderer);
	void renderRadar(sf::RenderWindow & renderer);
};

