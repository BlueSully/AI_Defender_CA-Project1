#pragma once
#include "Player.h"
class Camera
{
private:
	Player * m_targetPlayer;
	sf::Vector2f m_viewSize;
	bool m_xAxisLocked;
	bool m_yAxisLocked;
	sf::View m_view;
public:
	Camera();
	Camera(sf::Vector2f startPosition, sf::Vector2f ViewportSize);
	Camera(sf::Vector2f startPosition, sf::Vector2f ViewportSize, bool xAxisLock, bool yAxisLock);
	~Camera();

	void setTargetPlayer(Player * player);
	void xAxisLock(bool value);
	void yAxisLock(bool value);
	void Update(sf::Vector2f worldbounds);
	sf::View getView() const;

private:
	void UpdateView(sf::Vector2f worldbounds);
};

