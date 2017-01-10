#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include "AiStates.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

class Abductor
{
private :
	sf::RectangleShape m_boundingBox;//Bounding Box for Abductor
	sf::RectangleShape m_attackRangeBox;//Bounding box for Attack Range
	sf::CircleShape m_flockRangeBox;//Bounding box for Flocking Range
	sf::RectangleShape m_worldBounds;//World data
	sf::RectangleShape m_screenBounds;//ScreenData

	sf::Vector2f m_velocity;
	sf::Vector2f m_abducteePosition;

	sf::Vertex patrolLine[2];

	float speed;
	int m_attackRange;
	int m_flockRange;

	bool m_enemyInRange;
	bool m_canAttack;

	AiState state;
public:
	Abductor();
	Abductor(sf::Vector2f position, sf::Vector2f windowSize);
	~Abductor();

	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	sf::RectangleShape getBoundingBox();

	void setWorldRectangle(sf::Vector2f postion, sf::Vector2f size);
	void setPosition(sf::Vector2f value);
	void setAbductorTarget(sf::Vector2f target);
	void setState(AiState value);

	void update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox);
	sf::Vector2f getDirection();

	void flock(Abductor target, std::vector<Abductor> abductors);
	void patrol(sf::Time deltaTime);

	sf::Vector2f computeAlignment(std::vector<Abductor> abductors);


	void attack(sf::RectangleShape target);

	bool rectCollision(sf::RectangleShape rectA, sf::RectangleShape rectB);
	void render(sf::RenderWindow & renderer);
};

