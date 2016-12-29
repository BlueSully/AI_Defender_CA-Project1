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
	}
}

void ProjectileManager::addLaser(bool isLeft, sf::Vector2f playerPos, float playerXVelocity, int ttl)
{

}

void ProjectileManager::addMissile(sf::Vector2f playerPos, sf::Vector2f nestPos, int ttl)
{

}

void ProjectileManager::Render(sf::RenderWindow & renderer)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles.at(i).Render(renderer);
	}
}





