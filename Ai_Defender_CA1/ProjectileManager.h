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
	void UpdateMissile(sf::Time deltaTime, sf::RectangleShape playerBox);
	void addLaser(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl, bool isEnemyBullet);
	float getProjNumber();
	void Render(sf::RenderWindow & renderer);
	std::vector<Projectile> getProjList() const;
	void addMissile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl);
	void setAlive(bool value, int index);
};