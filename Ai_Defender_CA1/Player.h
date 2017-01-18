#pragma once
#include <SFML\Graphics.hpp>
#include "PlayerState.h"
#include <time.h>
#include "MathFunction.h"
#include "ProjectileManager.h"

class Player
{
private:
	
	sf::Vector2f m_velocity;
	sf::Vector2f m_size;
	sf::Vector2f m_speed;
	sf::Vector2f m_worldSize;
	const float m_MAXHORIZONTALACCLERATION;
	const float m_MAXVERTICALACCLERATION;
	bool m_smartBombReady;
	bool m_jumpReady;
	int m_smartBombNum;
	int m_resetTime;
	float m_timer;
	bool m_isLeft;

	float m_orientation;
	sf::RectangleShape m_boundingBox;
	PlayerStates m_playerState;
	ProjectileManager projMan;
	sf::Texture m_playerText;
	sf::Sprite m_playerSprite;
public:
	Player();
	~Player();

	void processInputs(sf::Event *evt);
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	vector<Projectile> getProjList() const;
	void setPosition(sf::Vector2f pos);
	sf::RectangleShape getBoundingBox() const;

	float getNewOrientation(float currentOrientation, sf::Vector2f velocity);
	void boundaryResponse(sf::Vector2f worldSize);
	void activateSmartBomb();
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow &renderer);
	void renderRadar(sf::RenderWindow & renderer);
	void setLaserAlive(bool value, int index);
	void render(sf::RenderWindow &renderer, float scale);
};

