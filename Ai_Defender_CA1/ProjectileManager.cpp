#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::Update(sf::Time deltaTime)
{
	for (int i = 0; i < m_projectiles.size(); i++) 
	{
		if (m_projectiles.at(i).Update(deltaTime));
		else if(!m_projectiles.at(i).Update(deltaTime) || m_projectiles[i].getAlive() == false)
		{
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

void ProjectileManager::UpdateMissile(sf::Time deltaTime, sf::RectangleShape playerBox)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles.at(i).UpdateMissile(deltaTime, playerBox));
		else if (!m_projectiles.at(i).UpdateMissile(deltaTime, playerBox) )
		{
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

void ProjectileManager::addLaser(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl,bool isEnemyBullet)
{
	m_projectiles.push_back(Projectile(isLeft, playerPos, playerXVelocity, ttl, isEnemyBullet));
}

void ProjectileManager::addMissile(sf::RectangleShape playerPos, sf::Vector2f nestPos, int ttl)
{
	m_projectiles.push_back(Projectile(playerPos, nestPos, ttl));
}

void ProjectileManager::setAlive(bool value,int index)
{
	m_projectiles[index].setAlive(value);
}

float ProjectileManager::getProjNumber()
{
	return m_projectiles.size();
}
void ProjectileManager::Render(sf::RenderWindow & renderer)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles.at(i).Render(renderer);
	}
}


std::vector<Projectile> ProjectileManager::getProjList() const
{
	return m_projectiles;
}





