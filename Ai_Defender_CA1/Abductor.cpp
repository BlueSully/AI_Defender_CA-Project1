#include "Abductor.h"

Abductor::Abductor() : m_attackRange(17), m_flockRange(5), speed(20), state(PATROL), m_enemyInRange(false), m_canAttack(true)
{
	m_boundingBox.setPosition(sf::Vector2f(0, 300));
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	
	m_flockRangeBox.setFillColor(sf::Color(0, 0, 125, 175));
	m_flockRangeBox.setRadius(static_cast<float>(32 * m_flockRange));
	m_flockRangeBox.setOrigin(sf::Vector2f(-m_flockRangeBox.getRadius() / 2, -m_flockRangeBox.getRadius() / 2));

	m_velocity = getDirection();
}

Abductor::Abductor(sf::Vector2f position, sf::Vector2f windowSize) : m_attackRange(17), m_flockRange(5), speed(35), state(PATROL), m_enemyInRange(false), m_canAttack(true)
{
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));

	m_flockRangeBox.setFillColor(sf::Color(0, 0, 125, 175));
	m_flockRangeBox.setRadius(static_cast<float>(32 * m_flockRange));
	m_flockRangeBox.setOrigin(sf::Vector2f(m_flockRangeBox.getRadius() / 2, m_flockRangeBox.getRadius() / 2));

	m_velocity = getDirection();
}


Abductor::~Abductor()
{

}

sf::Vector2f Abductor::getSize()
{
	return m_boundingBox.getSize();
}

sf::Vector2f Abductor::getPosition()
{
	return m_boundingBox.getPosition();
}

sf::RectangleShape Abductor::getBoundingBox()
{
	return m_boundingBox;
}

void Abductor::setWorldRectangle(sf::Vector2f postion, sf::Vector2f size)
{
	m_worldBounds.setPosition(postion);
	m_worldBounds.setSize(size);
}

void Abductor::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

void Abductor::setAbductorTarget(sf::Vector2f target)
{
	m_abducteePosition = target;
}

void Abductor::setState(AiState value)
{
	state = value;
}

void Abductor::flock(sf::RectangleShape target)
{
	//m_flockRangeBox
	//if (rectCollision(m_flockRangeBox, target)) //Check if value is inside of wander radius;
	//{
	//	std::cout << "Flocking" << std::endl;

	//}
}

void Abductor::patrol(sf::Time deltaTime)
{
	patrolLine[0].position = sf::Vector2f(m_worldBounds.getPosition().x, 300);
	patrolLine[1].position = sf::Vector2f(m_worldBounds.getPosition().x + m_worldBounds.getSize().x, 300);
	
	if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) < static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity = sf::Vector2f(m_velocity.x, speed);
	}
	else if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) > static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity = sf::Vector2f(m_velocity.x, -speed);
	}
	else 
	{
		m_velocity = sf::Vector2f(m_velocity.x, 0);
	}

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
}

void Abductor::attack(sf::RectangleShape target)
{
	if (rectCollision(m_attackRangeBox, target)) //Check if value is inside of wander radius;
	{
		if (m_enemyInRange == false)
		{
			//Enemy in Range 
			m_enemyInRange = true;
			//std::cout << "In Range " << std::endl;
		}
	}
	else 
	{
		m_enemyInRange = false;
	}
}

void Abductor::update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox)
{
	switch (state)
	{
	case ABDUCT:

		break;
	case PATROL:
		patrol(deltaTime);
		break;
	default:
		break;
	}

	//Move Radius with Abductor as the abductor moves
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	m_flockRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(m_flockRangeBox.getRadius() - (m_boundingBox.getSize().x / 2), m_flockRangeBox.getRadius() - (m_boundingBox.getSize().x / 2))); 
	
	attack(playerBoundingBox);// Attack player if in range
}

sf::Vector2f Abductor::getDirection()
{
	srand(time(NULL));
	int direction = rand() % 2;
	sf::Vector2f directionVector = sf::Vector2f(speed, 0);

	/*switch (direction)
	{
	case 0:
		directionVector = sf::Vector2f(speed, 0);
		break;
	case 1:
		directionVector = sf::Vector2f(-speed, 0);
		break;
	}*/

	return directionVector;
}

bool Abductor::rectCollision(sf::RectangleShape rectA, sf::RectangleShape rectB)
{
	float aLeft = rectA.getPosition().x;
	float aTop = rectA.getPosition().y;
	float aRight = rectA.getPosition().x + rectA.getSize().x;
	float aBottom = rectA.getPosition().y + rectA.getSize().y;

	float bLeft = rectB.getPosition().x;
	float bTop = rectB.getPosition().y;
	float bRight = rectB.getPosition().x + rectB.getSize().x;
	float bBottom = rectB.getPosition().y + rectB.getSize().y;

	return (aLeft <= bRight &&
			bLeft <= aRight &&
			aTop <= bBottom &&
			bTop <= aBottom);
}

void Abductor::render(sf::RenderWindow & renderer)
{
	//renderer.draw(patrolLine, 2, sf::Lines);//Debug Patrol Line
	renderer.draw(m_boundingBox);
	renderer.draw(m_flockRangeBox);
	//renderer.draw(m_attackRangeBox);
}