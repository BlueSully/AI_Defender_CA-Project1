#pragma once
#include <SFML\Graphics.hpp>
#include "PlayerState.h"
#include <time.h>

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
	float timer;

	sf::RectangleShape m_boundingBox;
	PlayerStates m_playerState;
public:
	Player();
	~Player();

	void processInputs(sf::Event *evt);
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f pos);

	void boundaryResponse(sf::Vector2f worldSize);
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow &renderer);
	void render(sf::RenderWindow &renderer, float scale);
};

