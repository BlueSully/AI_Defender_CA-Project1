#pragma once

#include "MathFunction.h"
#include <vector>
#include "AiStates.h"

using namespace std;

class Abductor
{
private:

	sf::RectangleShape m_boundingBox;//Bounding Box for Abductor
	sf::RectangleShape m_attackRangeBox;//Bounding box for Attack Range
	sf::RectangleShape m_worldBounds;//World data
	sf::RectangleShape m_screenBounds;//ScreenData

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	sf::Vector2f * m_abducteePosition;
	sf::Vector2f directionVector;

	sf::Vertex patrolLine[2];

	float m_speed;
	float m_timeTillNextAttack;
	int m_attackRange;
	int m_abducteeId;

	bool m_canAttack;
	bool m_grabbedAstronaut;

	AiState m_state;
public:
	Abductor();
	Abductor(sf::Vector2f position, sf::Vector2f windowSize, int id, float speed);
	~Abductor();

	sf::Vector2f getSize() const;
	sf::Vector2f & getPosition();
	sf::RectangleShape getBoundingBox() const;
	AiState getState() const;
	sf::Vector2f getVelocity() const;
	sf::Vector2f getDirection() const;
	int getAbucteeId() const;
	void setAbducteeId(int astronnautId);
	bool getGrabbedAstronaut() const;
	void setGrabbedAstronaut(bool value);

	void setWorldRectangle(sf::Vector2f postion, sf::Vector2f size);
	void setPosition(sf::Vector2f value);
	void setAbductorTarget(sf::Vector2f * target);
	void setState(AiState value);
	void setVelocity(sf::Vector2f value);
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

	void abduct(sf::Time deltaTime, sf::Vector2f * pos);
	void patrol(sf::Time deltaTime);

	void attack(sf::RectangleShape target);

	void boundaryResponse();

	void render(sf::RenderWindow & renderer);
};

