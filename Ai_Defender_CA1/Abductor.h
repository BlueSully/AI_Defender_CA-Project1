#pragma once

#include "MathHelper.h"
#include <vector>
#include "AiStates.h"

#include <iostream>


using namespace std;

using namespace std;

class Abductor
{
private :

	sf::RectangleShape m_boundingBox;//Bounding Box for Abductor
	sf::RectangleShape m_attackRangeBox;//Bounding box for Attack Range
	sf::RectangleShape m_worldBounds;//World data
	sf::RectangleShape m_screenBounds;//ScreenData

	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_abducteePosition;
	sf::Vector2f directionVector;

	sf::Vertex patrolLine[2];

	float m_speed;
	float m_timeTillNextAttack;
	int m_attackRange;
	int m_currentId;

	bool m_canAttack;

	AiState state;
public:
	Abductor();
	Abductor(sf::Vector2f position, sf::Vector2f windowSize, int id, float speed);
	~Abductor();

	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	sf::RectangleShape getBoundingBox();
	sf::Vector2f getVelocity();
	sf::Vector2f getDirection();

	void setWorldRectangle(sf::Vector2f postion, sf::Vector2f size);
	void setPosition(sf::Vector2f value);
	void setAbductorTarget(sf::Vector2f target);
	void setState(AiState value);
	void setSpeed(float value);
	void setDirection(int value);
	void setSize(sf::Vector2f value);
	void setColour(sf::Color colour);

	void update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox);

	void flock(vector<Abductor *> * abductors);

	sf::Vector2f alignment(vector<Abductor *> * abductors);
	sf::Vector2f seperation(vector<Abductor *> * abductors);
	sf::Vector2f cohesion(vector<Abductor *> * abductors);
	sf::Vector2f seek(sf::Vector2f v);

	void patrol(sf::Time deltaTime);

	void attack(sf::RectangleShape target);

	void boundaryResponse();

	bool rectCollision(sf::RectangleShape rectA, sf::RectangleShape rectB);
	void render(sf::RenderWindow & renderer);
};

