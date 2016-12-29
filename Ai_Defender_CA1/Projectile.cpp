#include "Projectile.h"

Projectile::Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity,int ttl)
{
	m_ttl = ttl;
	m_projectileBoundBox.setPosition(playerPos);
	m_alive = true;
	//set to laser sprite
	int speed = 10;
	m_isLaser = true;

	if (isLeft == true)
	{
		m_velocity = sf::Vector2f(playerXVelocity - speed, 0);
	}
	else
	{
		m_velocity = sf::Vector2f(playerXVelocity + speed, 0);
	}

}
Projectile::Projectile(sf::Vector2f playerPos, sf::Vector2f nestPos, int ttl)
{
	m_ttl = ttl;
	m_projectileBoundBox.setPosition(nestPos);
	//set to missile sprite 
	m_alive = true;
	int speed = 5;
	m_isLaser = false;

}
Projectile::~Projectile()
{
}
bool Projectile::Update(sf::Time deltaTime)
{
	
	timer += deltaTime.asSeconds();
	if (timer >= m_ttl)
	{
		timer = 0;
		m_alive = false;
	}
	sf::Vector2f pos = (m_projectileBoundBox.getPosition() + m_velocity);
	m_projectileBoundBox.setPosition(pos);

	return m_alive;
}
void Projectile::Render(sf::RenderWindow &renderer)
{
	renderer.draw(m_projectileBoundBox);
}


