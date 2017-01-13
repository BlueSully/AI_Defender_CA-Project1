#pragma once
#include "Projectile.h"

class ProjectileManager
{
private:
	std::vector<Projectile> m_projectiles;

public:
	ProjectileManager();
	~ProjectileManager();
	void Update(sf::Time deltaTime);
	void Update(sf::Time deltaTime, sf::RectangleShape);
	void Render(sf::RenderWindow & renderer);
	void addLaser(bool isLeft,sf::Vector2f playerPos, float playerXVelocity, int ttl);
	void addMissile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl);
};