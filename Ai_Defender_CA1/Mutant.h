#pragma once

#include "MathFunction.h"
#include "AiStates.h"
#include "ProjectileManager.h"


//! Mutant Class.
/*!
	This is a very aggressive alien as it will once spawned actively search for the player and won't stop till it is destroyed.
	It will swarm with nearby mutants as it seeks for the player.
*/

class Mutant
{
private:
	sf::RectangleShape m_boundingBox;//Bounding Box for Mutant
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	int m_lives;

	const float MAX_VELOCITY = 192;
	bool m_alive;
	ProjectileManager projMan;
	AiState m_state;

	sf::Texture m_mutantTex;
	sf::Sprite m_mutantSprite;


public:
	Mutant();
	Mutant(sf::Vector2f position, sf::Vector2f velocity);
	~Mutant();
	
	sf::Vector2f getSize() const;

	std::vector<Projectile> getProjList() const;

	sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f value);

	int getLives() const;

	void setLives(int value);

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f value);

	bool isAlive() const;
	void setAlive(bool value);

	sf::Vector2f seek(sf::Vector2f playerPostion);
	void swarm(vector<Mutant *> mutantsList);

	void update(sf::Time deltaTime, sf::Vector2f playerPostion);

	void render(sf::RenderWindow & renderer);
	void renderRadar(sf::RenderWindow & renderer);
};

