#include "Projectile.h"

Projectile::Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl) :
	m_size(sf::Vector2f(40, 5))
{
	float offset = 15.0f;//so lasers leave from centre of ship
	m_ttl = ttl;
	m_projectileBoundBox.setFillColor(sf::Color::White);
	m_projectileBoundBox.setSize(m_size);
	m_projectileBoundBox.setPosition(sf::Vector2f(playerPos.x, playerPos.y + offset));
	m_alive = true;
	//set to laser sprite
	m_speed = 2.5f;
	m_timer = 0;
	m_isLaser = true;
	if (isLeft == true)
	{
		m_velocity = sf::Vector2f(-m_speed, 0);
	}
	else
	{
		m_velocity = sf::Vector2f(m_speed, 0);
	}
}
Projectile::Projectile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl):m_size(sf::Vector2f(10, 10))
{
	m_ttl = ttl;
	m_projectileBoundBox.setFillColor(sf::Color::Yellow);
	m_projectileBoundBox.setSize(m_size);
	m_projectileBoundBox.setPosition(nestPos);
	//set to missile sprite 
	m_alive = true;
	m_speed = 1.0f;
	m_isLaser = false;
	m_playerBox= playerPos;
	m_timer = 0;

}
Projectile::~Projectile()
{
}
bool Projectile::Update(sf::Time deltaTime)
{
	m_timer += deltaTime.asSeconds();
	if (m_timer >= m_ttl)
	{
		m_timer = 0;
		m_alive = false;
	}
	if (m_isLaser) 
	{
		m_colourCount++;
		m_projectileBoundBox.setFillColor(sf::Color(1 * m_colourCount, 1.5f * m_colourCount, 2.0f * m_colourCount));
		if (m_colourCount > 170)
		{
			m_colourCount = 0;
		}
	sf::Vector2f pos = (m_projectileBoundBox.getPosition() + m_velocity);
	m_projectileBoundBox.setPosition(pos);
	}
	return m_alive;
}
bool Projectile::Update(sf::Time deltaTime,sf::RectangleShape playerBox)
{
	m_playerBox = playerBox;
	m_timer += deltaTime.asSeconds();
	if (m_timer >= m_ttl)
	{
		m_timer = 0;
		m_alive = false;
	}
		if (trackPlayer())
		{
		}
		else
		{
			m_alive = false;
		}
	return m_alive;
}
void Projectile::Render(sf::RenderWindow &renderer)
{
	if(m_alive)
	renderer.draw(m_projectileBoundBox);
}
bool Projectile::trackPlayer()
{
	m_velocity = sf::Vector2f(0,0);

	if(CollisionHelper::RectangleCollision(m_projectileBoundBox.getPosition(), m_projectileBoundBox.getSize(), m_playerBox.getPosition(), m_playerBox.getSize()))
	{
		sf::Vector2f pos;
		if (m_projectileBoundBox.getPosition().x < m_playerBox.getPosition().x)
		{
			m_velocity.x += m_speed;
			pos = (m_projectileBoundBox.getPosition() + m_velocity);
		}
		else if (m_projectileBoundBox.getPosition().x > m_playerBox.getPosition().x)
		{
			m_velocity.x -= m_speed;
			pos = (m_projectileBoundBox.getPosition() + m_velocity);
		}
		if (m_projectileBoundBox.getPosition().y < m_playerBox.getPosition().y)
		{
			m_velocity.y += m_speed;
			pos = (m_projectileBoundBox.getPosition() + m_velocity);
		}
		else if (m_projectileBoundBox.getPosition().y > m_playerBox.getPosition().y)
		{
			m_velocity.y -= m_speed;
			pos = (m_projectileBoundBox.getPosition() + m_velocity);
		}

		m_projectileBoundBox.setPosition(pos);
		return true;
	}
	else
		return false;
}




