#include "Player.h"
#include <iostream>

Player::Player()
: m_velocity(sf::Vector2f(0, 0)),
  m_size(sf::Vector2f(50, 30)),
  m_speed(sf::Vector2f(0, 0)),
  m_MAXHORIZONTALACCLERATION(1000),
  m_MAXVERTICALACCLERATION(500),m_lives(300)
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
	m_smartBombFired = false;
	m_resetTime = 60;
	m_isLeft = false;
	if (!m_playerText.loadFromFile("Resources/Player.png"))
	{
		std::cout << "File failed to load. Check folder location is correct" << std::endl;
	}

	m_playerSprite.setTexture(m_playerText);
	m_playerSprite.setTextureRect(sf::IntRect(0, 0, 60, 24));
	m_playerSprite.setOrigin(m_playerSprite.getTextureRect().width / 2, m_playerSprite.getTextureRect().height / 2);

}

Player::~Player()
{

}

float Player::getNewOrientation(float currentOrientation, sf::Vector2f velocity)
{
	if (VectorHelper::magnitude(velocity) > 0)
	{
		return std::atan2(m_boundingBox.getPosition().y - (m_boundingBox.getPosition().y + velocity.y), 
						  m_boundingBox.getPosition().x - (m_boundingBox.getPosition().x + velocity.x));
	}
	else
	{
		return currentOrientation;
	}
}

void Player::processInputs(sf::Event *evt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_playerState = PlayerStates::MOVING_LEFT;
		m_isLeft = true;
		m_playerSprite.setScale(-1.f, 1.f);
		m_playerSprite.setTextureRect(sf::IntRect(60, 0, 60, 24));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_playerState = PlayerStates::MOVING_RIGHT;
		m_isLeft = false;
		m_playerSprite.setScale(1.f, 1.f);
		m_playerSprite.setTextureRect(sf::IntRect(60, 0, 60, 24));

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
		&&!sf::Keyboard::isKeyPressed(sf::Keyboard::Q) &&!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_playerState = PlayerStates::IDLE;
		m_playerSprite.setTextureRect(sf::IntRect(0, 0, 60, 24));
	}
	// Ability keys
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if(projMan.getProjNumber() <5)
		projMan.addLaser(m_isLeft, m_boundingBox.getPosition(), m_velocity.x, 3,false);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && m_jumpReady)
	{
		int worldX = static_cast<int>(-(m_worldSize.x / 2));
		int worldY = static_cast<int>(m_worldSize.y / 2);
		setPosition(sf::Vector2f(rand() % ((int)m_worldSize.x + 1) + (worldX), getPosition().y));
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
sf::Vector2f Player::getSize() const
{
	return m_boundingBox.getSize();
}

int Player::getLives() const
{
	return m_lives;
}
void Player::setLives(int value) 
{
	m_lives = value;
}

bool Player::getBomb() const
{
	return m_smartBombFired;
}
void Player::setBomb(bool value)
{
	m_smartBombFired = value;
}
std::vector<Projectile> Player::getProjList() const
{
	return projMan.getProjList();
}

void Player::setPosition(sf::Vector2f pos)
{
	m_boundingBox.setPosition(pos);
}

sf::RectangleShape Player::getBoundingBox() const
{
	return m_boundingBox;
}
void Player::boundaryResponse(sf::Vector2f worldSize)
{
	m_worldSize = sf::Vector2f(worldSize.x, worldSize.y);

	if (getPosition().y + m_size.y < (m_size.y + (m_worldSize.y / 10.0f)))//radar takes up the top 1/10 of the screen
	{
		setPosition(sf::Vector2f(getPosition().x, (m_worldSize.y / 10.0f)));
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
	//std::cout << "smrtBmb" << std::endl;
	m_smartBombNum--;
	m_smartBombFired = true;
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


	sf::Vector2f spritePos = sf::Vector2f(m_boundingBox.getPosition().x + (m_boundingBox.getSize().x / 2.0f), m_boundingBox.getPosition().y + (m_boundingBox.getSize().y / 2.0f));
	m_playerSprite.setPosition(spritePos);

	if (m_smartBombReady == false)
	{
		m_timer += deltaTime.asSeconds();

		if (m_timer >= m_resetTime)
		{
			m_timer = 0;
			m_smartBombReady = true;
		}
	}
	
	projMan.Update(deltaTime);
	m_velocity = VectorHelper::truncate(m_velocity, m_MAXHORIZONTALACCLERATION);

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
}
void Player::render(sf::RenderWindow & renderer)
{
	//renderer.draw(m_boundingBox);
	projMan.Render(renderer);
	renderer.draw(m_playerSprite);
}


void Player::renderRadar(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
	projMan.Render(renderer);
}

void Player::setLaserAlive(bool value, int index)
{
	projMan.setAlive(value, index);
}
void Player::render(sf::RenderWindow &renderer, float scale)
{
	sf::RectangleShape drawRect = m_boundingBox;
	drawRect.setScale(sf::Vector2f(scale, scale));
	renderer.draw(drawRect);
	renderer.draw(m_playerSprite);
}