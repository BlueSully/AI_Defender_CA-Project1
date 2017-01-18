#include "Projectile.h"

Projectile::Projectile(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl, bool isEnemyBullet) :
	m_size(sf::Vector2f(40, 5)),
	m_alive(true),
	m_speed(2.5f),
	m_isLaser(true),
	m_timer(0)
{
	float offset = 20.0f;//so lasers leave from centre of ship
	m_ttl = ttl;
	m_projectileBoundBox.setFillColor(sf::Color::White);
	m_projectileBoundBox.setSize(m_size);
	m_projectileBoundBox.setPosition(sf::Vector2f(playerPos.x, playerPos.y + offset));
	m_isEnemyBullet = isEnemyBullet;
	if (isLeft == true)
	{
		m_velocity = sf::Vector2f(-m_speed, 0);
	}
	else
	{
		m_velocity = sf::Vector2f(m_speed, 0);
	}
}
Projectile::Projectile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl)
	:m_size(sf::Vector2f(24, 12)),
	m_alive(true),
	m_speed(0.01f),
	MAX_SPEED(.5f),
	m_isLaser(false),
	m_timer(0),
	m_rotation(atan2(m_velocity.x, m_velocity.y) * (180 / 3.141592654))
{
	m_ttl = ttl;
	m_projectileBoundBox.setFillColor(sf::Color::Yellow);
	m_projectileBoundBox.setSize(m_size);
	m_projectileBoundBox.setPosition(nestPos);
	m_playerBox= playerPos;
	if (!m_missileText.loadFromFile("Resources/Missile.png"))
	{
		std::cout << "File failed to load. Check folder location is correct" << std::endl;
	}
	m_missileSprite.setTexture(m_missileText);
	m_missileSprite.setTextureRect(sf::IntRect(0, 0, 24, 12));
}
Projectile::~Projectile()
{
}
bool Projectile::Update(sf::Time deltaTime) //laser
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
	/*collisionCheck();*/
	}
	return m_alive;
}
bool Projectile::UpdateMissile(sf::Time deltaTime,sf::RectangleShape playerBox)//missile
{
	m_playerBox = playerBox;
	m_timer += deltaTime.asSeconds();
	m_missileSprite.setPosition(m_projectileBoundBox.getPosition());

	if (m_acceleration.x <= MAX_SPEED)
	{
		m_acceleration.x += m_speed;
		m_acceleration.y += m_speed;
	}
	if (m_timer >= m_ttl)
	{
		m_timer = 0;
		m_alive = false;
	}
		if (trackPlayer())
		{ }
		else if (!trackPlayer() && m_alive == true)
		{
			m_alive = false;
		}
	return m_alive;
}
void Projectile::Render(sf::RenderWindow &renderer)
{
	if (m_alive) 
	{
		if (m_isLaser)
		{
			renderer.draw(m_projectileBoundBox);
		}
		else
		{
			renderer.draw(m_projectileBoundBox);
			renderer.draw(m_missileSprite);
		}

	}

}
bool Projectile::trackPlayer()
{
	m_velocity = sf::Vector2f(0,0);
	if(!CollisionHelper::RectangleCollision(m_projectileBoundBox.getPosition(), m_projectileBoundBox.getSize(), m_playerBox.getPosition(), m_playerBox.getSize()))
	{
		sf::Vector2f pos;
		sf::Vector2f vel = sf::Vector2f(m_playerBox.getPosition().x - m_projectileBoundBox.getPosition().x, m_playerBox.getPosition().y - m_projectileBoundBox.getPosition().y);
		m_velocity = vel;
		normalise(m_velocity);
		m_velocity = sf::Vector2f(m_velocity.x * m_acceleration.x, m_velocity.y * m_acceleration.y);
		pos = m_projectileBoundBox.getPosition() + m_velocity;
		m_projectileBoundBox.setPosition(pos);
		rotateMissile();
		return true;
	}
	else
		return false;
}
void Projectile::rotateMissile()
{
	if (sqrt((m_velocity.x*m_velocity.x) + (m_velocity.y*m_velocity.y)) >= 0)
	{
		m_rotation = atan2(m_velocity.x, -m_velocity.y) * (180 / 3.141592654);
		m_missileSprite.setRotation(m_rotation - 90);
	}
}

void Projectile::normalise(sf::Vector2f& vec)
{
	float length = sqrt((vec.x*vec.x) + (vec.y*vec.y));
	vec = sf::Vector2f(vec.x / length, vec.y / length);
}

sf::CircleShape Projectile::getRadius()
{
	return m_damageRadius;
}
sf::Vector2f Projectile::getPosition()const
{
	return m_projectileBoundBox.getPosition();
}

sf::Vector2f Projectile::getSize() const
{
	return m_projectileBoundBox.getSize();
}
void Projectile::setAlive(bool value)
{
	m_alive = value;
}
bool Projectile::isAlive() const
{
	return m_alive;
}

//void Projectile::collisionCheck()
//{
//	if (m_isEnemyBullet)
//	{
//		if (CollisionHelper::RectangleCollision(m_projectileBoundBox.getPosition(), m_projectileBoundBox.getSize(), m_playerBox.getPosition(), m_playerBox.getSize()))
//		{
//			m_alive = false;
//
//		}
//	}
//}




