#pragma once

#include "MathFunction.h"
#include "AiStates.h"

using namespace std;

class Astronaut
{
private:
	sf::RectangleShape m_boundingBox;

	sf::Vector2f m_velocity;
	sf::Vector2f m_steering;
	sf::Vector2f * m_fleeTarget;
	sf::Vector2f * m_followTarget;
	sf::Vector2f m_followSize;

	float m_wanderAngle;
	int m_abductorId;
	int m_fleeingDirection;
	float m_wanderDelayTimer;

	const float MAX_FORCE = 6.4f;
	const float MAX_VELOCITY = 10;
	const float CIRCLEDISTANCE = 6;
	const float CIRCLE_RADIUS = 8;
	const float ANGLECHANGE = 1000;

	AiState m_state;
	bool m_beingAbducted;
	bool m_falling;
public:
	Astronaut();
	Astronaut(sf::Vector2f position, sf::Vector2f veclocity);
	~Astronaut();

	//Get Method
	sf::Vector2f getSize();
	sf::Vector2f getPosition();	
	void setFleeTarget(sf::Vector2f * value);
	sf::Vector2f getVelocity();
	AiState getState() const;
	void setPosition(sf::Vector2f value);
	void setState(AiState value);
	void setBeingAbducted(bool value);
	bool getBeingAbducted() const;
	int getAbuctorId() const;
	void setAbductorId(int value);
	void setFollowTarget(sf::Vector2f & value, sf::Vector2f size);

	//Methods
	sf::Vector2f wander();
	sf::Vector2f flee();

	void boundaryResponse(sf::Vector2f worldSize);
	void update(sf::Time elapsedTime);
	void render(sf::RenderWindow & renderer);
};

