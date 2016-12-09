#include "Camera.h"

Camera::Camera() : m_xAxisLocked(false), m_yAxisLocked(false)
{
}

Camera::~Camera()
{
}

Camera::Camera(sf::Vector2f startPosition, sf::Vector2f ViewportSize) 
{
	m_view = sf::View(sf::FloatRect(startPosition.x, startPosition.y, ViewportSize.x, ViewportSize.y));
	m_viewSize = ViewportSize;
}

Camera::Camera(sf::Vector2f startPosition, sf::Vector2f ViewportSize, bool xAxisLock, bool yAxisLock) : m_xAxisLocked(xAxisLock), m_yAxisLocked(yAxisLock)
{
	m_view = sf::View(sf::FloatRect(startPosition.x, startPosition.y, ViewportSize.x, ViewportSize.y));
	m_viewSize = ViewportSize;
}

void Camera::setTargetPlayer(Player * player)
{
	m_targetPlayer = player;
}

void Camera::setViewport(sf::FloatRect viewport)
{
	m_view.setViewport(viewport);
}

void Camera::xAxisLock(bool value) 
{
	m_xAxisLocked = value;
}

void Camera::yAxisLock(bool value) 
{
	m_yAxisLocked = value;
}

void Camera::Update(sf::Vector2f worldbounds)
{
	UpdateView(worldbounds);
}

void Camera::UpdateView(sf::Vector2f worldbounds)
{
	if (!m_xAxisLocked)
	{
		if (m_targetPlayer->getPosition().x > m_view.getCenter().x)
		{
			m_view.setCenter(m_targetPlayer->getPosition().x, m_view.getCenter().y);
		}
		else if (m_targetPlayer->getPosition().x < m_view.getCenter().x)
		{
			m_view.setCenter(m_targetPlayer->getPosition().x, m_view.getCenter().y);
		}
	}
	//if (!m_yAxisLocked)
	//{
	//	if (m_targetPlayer->getPosition().y > m_view.getCenter().y)
	//	{
	//		m_view.setCenter(m_view.getCenter().x, m_targetPlayer->getPosition().y);
	//	}
	//	else if (m_targetPlayer->getPosition().y < m_view.getCenter().y)
	//	{
	//		m_view.setCenter(m_view.getCenter().x, m_targetPlayer->getPosition().y);
	//	}
	//}
}

sf::View Camera::getView() const
{
	return m_view;
}
