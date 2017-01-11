#include "Abductor.h"

Abductor::Abductor() : m_attackRange(17), m_flockDistance(250), m_speed(20), state(PATROL), m_enemyInRange(false), m_canAttack(true)
{
	srand(time(NULL));
	m_boundingBox.setPosition(sf::Vector2f(0, 300));
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	
	m_velocity = getDirection();
}

Abductor::Abductor(sf::Vector2f position, sf::Vector2f windowSize, int direction) : m_attackRange(17), m_flockDistance(250), m_speed(35), state(PATROL), m_enemyInRange(false), m_canAttack(true)
{
	srand(time(NULL));
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	setDirection(direction);
	m_velocity = getDirection();
}

Abductor::~Abductor()
{

}

sf::Vector2f Abductor::getSize()
{
	return m_boundingBox.getSize();
}

void Abductor::setSize(sf::Vector2f value)
{
	m_boundingBox.setSize(value);
}

void Abductor::setColour(sf::Color colour)
{
	m_boundingBox.setFillColor(colour);
}

sf::Vector2f Abductor::getPosition()
{
	return m_boundingBox.getPosition();
}

sf::RectangleShape Abductor::getBoundingBox()
{
	return m_boundingBox;
}

sf::Vector2f Abductor::getVelocity()
{
	return m_velocity;
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

void Abductor::setSpeed(float value)
{
	m_speed = value;
}

void Abductor::flock(vector<Abductor *> * abductors)
{
	sf::Vector2f alignmentVect = alignment(abductors);
	sf::Vector2f cohesionVect = cohesion(abductors);
	sf::Vector2f separationVect = seperation(abductors);

	this->m_velocity.x += alignmentVect.x * m_ALIGNMENTWEIGHT + cohesionVect.x * m_COHENSIONWEIGHT + separationVect.x * m_SEPERATIONWEIGHT;
	this->m_velocity.y += alignmentVect.y * m_ALIGNMENTWEIGHT + cohesionVect.y * m_COHENSIONWEIGHT + separationVect.y * m_SEPERATIONWEIGHT;

	if (VectorHelper::magnitude(this->m_velocity) != 0)
	{
		this->m_velocity = VectorHelper::normalise(this->m_velocity);
		this->m_velocity *= m_speed;
	}
}

sf::Vector2f Abductor::alignment(vector<Abductor *> * abductors)
//Alignment is a behavior that causes a particular agent to line up with agents close by.
{
	sf::Vector2f v = sf::Vector2f(0, 0);
	int neighborCount = 0;

	for (size_t i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
		if (distBetween < m_flockDistance)
		{
			v.x += (*abductors)[i]->m_velocity.x;
			v.y += (*abductors)[i]->m_velocity.y;
			neighborCount++;
		}
	}

	if (neighborCount == 0)
		return v;
	float length = VectorHelper::magnitude(v);

	if (length <= 0) 
	{
		v = sf::Vector2f(-1, 0);
	}

	v.x /= neighborCount;
	v.y /= neighborCount;
	v = VectorHelper::normalise(v);
	return v;
}

sf::Vector2f Abductor::seperation(vector<Abductor *> * abductors)
{
	sf::Vector2f v = sf::Vector2f(0, 0);
	int neighborCount = 0;

	for (size_t i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
			if (distBetween < m_flockDistance)
			{
				v.x += (*abductors)[i]->getPosition().x - getPosition().x;
				v.y += (*abductors)[i]->getPosition().y - getPosition().y;
				neighborCount++;
			}
	}

	if (neighborCount == 0)
		return v;

	v.x /= neighborCount;
	v.y /= neighborCount;
	v.x *= -1;
	v.y *= -1;
	v = VectorHelper::normalise(v);

	return v;
}

sf::Vector2f Abductor::cohesion(vector<Abductor *> * abductors)
//Cohesion is a behavior that causes agents to steer towards the "center of mass" - that is, the average position of the agents within a certain radius.
{
	sf::Vector2f v = sf::Vector2f(0, 0);
	int neighborCount = 0;

	for (size_t i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
			if (distBetween < m_flockDistance)
			{
				v.x += (*abductors)[i]->getPosition().x;
				v.y += (*abductors)[i]->getPosition().y;
				neighborCount++;
			}
	}

	if (neighborCount == 0)
		return v;

	v.x /= neighborCount;
	v.y /= neighborCount;
	// Need to set the steering to a negative to move away
	
	v = sf::Vector2f(v.x - getPosition().x, v.y - getPosition().y);
	v = VectorHelper::normalise(v);
	return v;
}

void Abductor::patrol(sf::Time deltaTime)
{
	patrolLine[0].position = sf::Vector2f(m_worldBounds.getPosition().x, 300);
	patrolLine[1].position = sf::Vector2f(m_worldBounds.getPosition().x + m_worldBounds.getSize().x, 300);
	
	if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) < static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity = sf::Vector2f(m_velocity.x, m_speed);
	}
	else if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) > static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity = sf::Vector2f(m_velocity.x, -m_speed);
	}
	else 
	{
		m_velocity = sf::Vector2f(m_velocity.x, 0);
	}

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
}

sf::Vector2f Abductor::computeAlignment(std::vector<Abductor> abductors)
{
	sf::Vector2f v;
	int neighborCount = 0;

	for (size_t i = 0; i < abductors.size(); i++)
	{
		if (this != &abductors[i])
		{
			/*if (myAgent.distanceFrom(agent) < 300)
			{
				v.x += agent.velocity.x;
				v.y += agent.velocity.y;
				neighborCount++;
			}*/
		}
	}

	if (neighborCount == 0)
		return v;

	v.x /= neighborCount;
	v.y /= neighborCount;
	//v.normalize(1);
	return v;
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
	attack(playerBoundingBox);// Attack player if in range
}

sf::Vector2f Abductor::getDirection()
{
	return directionVector;
}

void Abductor::setDirection(int value)
{
	switch (value)
	{
	case 0:
		directionVector = sf::Vector2f(m_speed, 0);
		break;
	case 1:
		directionVector = sf::Vector2f(-m_speed, 0);
		break;
	}
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
	//renderer.draw(m_attackRangeBox);
}