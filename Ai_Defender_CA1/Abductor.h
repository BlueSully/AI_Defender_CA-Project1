#pragma once

#include "MathHelper.h"
#include "AiStates.h"

#include <iostream>


using namespace std;

class Abductor
{
private :
	const float m_ALIGNMENTWEIGHT = 10;
	const float m_SEPERATIONWEIGHT = 5;
	const float m_COHENSIONWEIGHT = 5;

	sf::RectangleShape m_boundingBox;//Bounding Box for Abductor
	sf::RectangleShape m_attackRangeBox;//Bounding box for Attack Range
	sf::RectangleShape m_worldBounds;//World data
	sf::RectangleShape m_screenBounds;//ScreenData

	sf::Vector2f m_velocity;
	sf::Vector2f m_abducteePosition;
	sf::Vector2f directionVector;

	sf::Vertex patrolLine[2];

	float m_speed;
	int m_attackRange;
	float m_flockDistance;
	float m_seperationDistance;

	bool m_enemyInRange;
	bool m_canAttack;

	AiState state;
public:
	Abductor();
	Abductor(sf::Vector2f position, sf::Vector2f windowSize, int direction);
	~Abductor();

	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	sf::RectangleShape getBoundingBox();
	sf::Vector2f getVelocity();

	void setWorldRectangle(sf::Vector2f postion, sf::Vector2f size);
	void setPosition(sf::Vector2f value);
	void setAbductorTarget(sf::Vector2f target);
	void setState(AiState value);
	void setSpeed(float value);
	void setSize(sf::Vector2f value);
	void setColour(sf::Color colour);

	void update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox);
	sf::Vector2f getDirection();
	void setDirection(int value);
	void flock(vector<Abductor *> * abductors);
	sf::Vector2f alignment(vector<Abductor *> * abductors);
	sf::Vector2f seperation(vector<Abductor *> * abductors);
	sf::Vector2f cohesion(vector<Abductor *> * abductors);

	void patrol(sf::Time deltaTime);

	void attack(sf::RectangleShape target);

	bool rectCollision(sf::RectangleShape rectA, sf::RectangleShape rectB);
	void render(sf::RenderWindow & renderer);
};

