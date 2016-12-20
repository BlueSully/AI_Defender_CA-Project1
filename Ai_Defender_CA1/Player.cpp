#include "Player.h"
#include <iostream>

Player::Player()
: m_velocity(sf::Vector2f(0, 0)),
  m_size(sf::Vector2f(50, 30)),
  m_speed(sf::Vector2f(0, 0)),
  m_MAXHORIZONTALACCLERATION(1000),
  m_MAXVERTICALACCLERATION(500)
{
	m_boundingBox.setFillColor(sf::Color::Green);
	m_boundingBox.setPosition(sf::Vector2f(375, 285));
	m_boundingBox.setSize(m_size);
	m_playerState = PlayerStates::IDLE;
	m_speed = sf::Vector2f(m_MAXHORIZONTALACCLERATION / 2, m_MAXVERTICALACCLERATION / 2);
	srand(time(NULL));
	m_jumpReady = true;
	m_smartBombReady = true;
	m_smartBombNum = 3;
}

Player::~Player()
{

}

void Player::processInputs(sf::Event *evt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_playerState = PlayerStates::MOVING_LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_playerState = PlayerStates::MOVING_RIGHT;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_playerState = PlayerStates::MOVING_UP;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_playerState = PlayerStates::MOVING_DOWN;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
		&&!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_playerState = PlayerStates::IDLE;
	}

	// Ability keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		std::cout << "Fire Laser" << std::endl;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && m_jumpReady)
	{
		int worldX = -(m_worldSize.x / 2);
		int worldY = m_worldSize.y;
		setPosition(sf::Vector2f(rand() % ((int)m_worldSize.x + 1) + (worldX), rand() % (worldY - 50) + 50));
		m_jumpReady = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && m_smartBombReady && m_smartBombNum > 0)
	{
		activateSmartBomb();
	}
}

sf::Vector2f Player::getPosition() const
{
	return m_boundingBox.getPosition();
}

void Player::setPosition(sf::Vector2f pos)
{
	m_boundingBox.setPosition(pos);
}

void Player::boundaryResponse(sf::Vector2f worldSize)
{
	m_worldSize = sf::Vector2f(worldSize.x, worldSize.y);

	if(getPosition().y + m_size.y < m_size.y)
	{
		setPosition(sf::Vector2f(getPosition().x, 0));
		m_velocity.y = 0;
	}
	else if (getPosition().y + m_size.y > m_worldSize.y)
	{
		setPosition(sf::Vector2f(getPosition().x, m_worldSize.y - m_size.y));
		m_velocity.y = 0;
	}
}
void Player::activateSmartBomb()
{
	m_smartBombReady = false;
	std::cout << "smrtBmb" << std::endl;
	m_smartBombNum--;
	//add kill all once Enemies added
}

void Player::update(sf::Time deltaTime)
{
#pragma region moveStates
	if (m_playerState == PlayerStates::MOVING_RIGHT && m_velocity.x < m_MAXHORIZONTALACCLERATION)
	{
		m_velocity.x += m_speed.x * deltaTime.asSeconds();
	}
	else if (m_playerState == PlayerStates::MOVING_LEFT && m_velocity.x > -m_MAXHORIZONTALACCLERATION)
	{
		m_velocity.x -= m_speed.x * deltaTime.asSeconds();
	}
	if (m_playerState == PlayerStates::MOVING_UP && m_velocity.y > -m_MAXVERTICALACCLERATION)
	{
		m_velocity.y -= m_speed.y * deltaTime.asSeconds();
	}
	else if (m_playerState == PlayerStates::MOVING_DOWN && m_velocity.y < m_MAXVERTICALACCLERATION)
	{
		m_velocity.y += m_speed.y * deltaTime.asSeconds();
	}
	if (m_playerState == PlayerStates::IDLE) 
	{
		if ((m_velocity.x > 0 && m_velocity.x < 0.1f) || (m_velocity.x < 0 && m_velocity.x > -0.1f))
		{
			m_velocity.x = 0;
		}
		else if (m_velocity.x > 0) 
		{
			m_velocity.x -= m_speed.x * deltaTime.asSeconds();
		}
		else if (m_velocity.x < 0) 
		{
			m_velocity.x += m_speed.x * deltaTime.asSeconds();
		}

		if ((m_velocity.y > 0 && m_velocity.y < 0.1f) || (m_velocity.y < 0 && m_velocity.y > -0.1f))
		{
			m_velocity.y = 0;
		}
		else if (m_velocity.y > 0)
		{
			m_velocity.y -= m_speed.y * deltaTime.asSeconds();
		}
		else if (m_velocity.y < 0)
		{
			m_velocity.y += m_speed.y * deltaTime.asSeconds();
		}
	}
#pragma endregion

	if (m_smartBombReady == false)
	{
		timer += deltaTime.asSeconds();
		std::cout << "time : " << timer << std::endl;
		if (timer >= 6)
		{
			timer = 0;
			m_smartBombReady = true;
		}

	}
	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
}

void Player::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
}

void Player::render(sf::RenderWindow &renderer, float scale)
{
	sf::RectangleShape drawRect = m_boundingBox;
	drawRect.setScale(sf::Vector2f(scale, scale));
	renderer.draw(drawRect);
}