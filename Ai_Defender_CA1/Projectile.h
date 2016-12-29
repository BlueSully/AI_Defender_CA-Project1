#pragma once
#include "Player.h"
class Projectile
{
private:

	sf::Vector2f m_velocity;
	sf::Vector2f m_size;
	sf::RectangleShape m_projectileBoundBox;
	bool m_alive;
	int m_ttl;
	float timer;
	bool m_isLaser;

public:
	Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl);
	Projectile(sf::Vector2f playerPos, sf::Vector2f nestPos, int ttl);
	~Projectile();

	bool Update(sf::Time deltaTime);

	void Render(sf::RenderWindow & renderer);


};
