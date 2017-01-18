#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <iostream>
#include "MathFunction.h"

class Projectile
{
private:

	sf::Vector2f m_velocity;



	sf::RectangleShape m_playerBox;
	sf::Vector2f m_size;
	sf::RectangleShape m_projectileBoundBox;
	bool m_alive;
	sf::Vector2f m_acceleration;
	int m_ttl;
	float m_timer;
	bool m_isLaser;
	float m_speed;
	float MAX_SPEED;
	float m_rotation;
	sf::CircleShape m_damageRadius;
	int m_colourCount;
	bool m_isEnemyBullet;

	sf::Texture m_missileText;
	sf::Sprite m_missileSprite;

	

public:

	Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl, bool isEnemyBullet);
	Projectile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl);
	~Projectile();


	bool Update(sf::Time deltaTime, sf::RectangleShape);

	bool UpdateMissile(sf::Time deltaTime, sf::RectangleShape playerBox);

	void Render(sf::RenderWindow & renderer);

	bool trackPlayer();

	void rotateMissile();

	void normalise(sf::Vector2f & vec);

	void explosionAnimation();

	sf::CircleShape getRadius();
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	void setAlive(bool value);
	bool isAlive() const;
	void collisionCheck();

	




};


