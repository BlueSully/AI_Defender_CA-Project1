#include "Projectile.h"

Projectile::Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity,int ttl) : 
m_size(sf::Vector2f(7,5))
{
	m_ttl = ttl;
	m_projectileBoundBox.setFillColor(sf::Color::White);
	m_projectileBoundBox.setSize(m_size);
	m_projectileBoundBox.setPosition(playerPos);
	m_alive = true;
	//set to laser sprite
	m_speed = 1.0f;
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
	m_speed = 0.05f;
	m_isLaser = false;
	m_playerBox= playerPos;

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
	if (m_isLaser) 
	{

	sf::Vector2f pos = (m_projectileBoundBox.getPosition() + m_velocity);
	m_projectileBoundBox.setPosition(pos);
	}
	return m_alive;
}
bool Projectile::Update(sf::Time deltaTime,sf::RectangleShape playerBox)
{
	m_playerBox = playerBox;

	timer += deltaTime.asSeconds();
	if (timer >= m_ttl)
	{
		timer = 0;
		m_alive = false;
	}
	if (!m_isLaser)
	{
		if (!trackPlayer())
		{
			m_alive = false;
		}

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

	if (!(m_projectileBoundBox.getPosition().x < m_playerBox.getPosition().x + m_playerBox.getSize().x
		&& m_projectileBoundBox.getPosition().x > m_playerBox.getPosition().x
		&& m_projectileBoundBox.getPosition().y < m_playerBox.getPosition().y + m_playerBox.getSize().y
		&& m_projectileBoundBox.getPosition().y > m_playerBox.getPosition().y))
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




