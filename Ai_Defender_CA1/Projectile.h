#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <iostream>
class Projectile
{
private:

	sf::Vector2f m_velocity;
	sf::RectangleShape m_playerBox;
	sf::Vector2f m_size;
	sf::RectangleShape m_projectileBoundBox;
	bool m_alive;
	int m_ttl;
	float timer = 0;
	bool m_isLaser;
	float m_speed;
	

public:

	Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl);
	Projectile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl);
	~Projectile();

	bool Update(sf::Time deltaTime);

	bool Update(sf::Time deltaTime, sf::RectangleShape);

	void Render(sf::RenderWindow & renderer);

	bool trackPlayer();


};


