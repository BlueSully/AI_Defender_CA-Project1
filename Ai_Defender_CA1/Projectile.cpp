#include "Projectile.h"

Projectile::Projectile(bool isLeft, sf::Vector2f userPos, float userrXVelocity,int ttl, int speed)
{
	m_ttl = ttl;
	m_projectileBoundBox.setPosition(userPos);
	m_alive = true;

	if (isLeft == true)
	{
		m_velocity = sf::Vector2f(userrXVelocity - speed, 0);
	}
	else
	{
		m_velocity = sf::Vector2f(userrXVelocity + speed, 0);
	}

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


