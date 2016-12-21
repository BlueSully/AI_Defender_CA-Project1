#pragma once
#include "Player.h"
class Projectile
{
private:

	sf::Vector2f m_velocity;
	sf::Vector2f m_size;
	int m_speed;
	sf::RectangleShape m_projectileBoundBox;
	bool m_alive;
	int m_ttl;
	float timer;

public:
	Projectile(bool isLeft, sf::Vector2f userPos, float userrXVelocity, int ttl, int speed);
	~Projectile();

	bool Update(sf::Time deltaTime);

	void Render(sf::RenderWindow & renderer);


};
