#include "Abductor.h"

Abductor::Abductor()
	: m_attackRange(17),
	m_speed(35),
	state(PATROL),
	m_canAttack(true),
	m_timeTillNextAttack(0)
{
	m_boundingBox.setPosition(sf::Vector2f(0, 300));
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	
	m_velocity = getDirection();
	m_acceleration = sf::Vector2f(0, 0);
}

Abductor::Abductor(sf::Vector2f position, sf::Vector2f windowSize, int id, float speed) 
	: m_attackRange(17),
	m_speed(speed),
	m_currentId(id),
	state(PATROL), 
	m_canAttack(true),
	m_timeTillNextAttack(0)
{
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));

	int dir = 0;

	if (id > 1) 
	{
		dir = 1;
	}
		
	setDirection(dir);
	m_velocity = getDirection();
	m_acceleration = sf::Vector2f(0, 0);
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

sf::Vector2f Abductor::getVelocity()
{
	return m_velocity;
}

sf::Vector2f Abductor::getDirection()
{
	return directionVector;
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

void Abductor::setSize(sf::Vector2f value)
{
	m_boundingBox.setSize(value);
}

void Abductor::setColour(sf::Color colour)
{
	m_boundingBox.setFillColor(colour);
}

void Abductor::setSpeed(float value)
{
	m_speed = value;

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

void Abductor::flock(vector<Abductor *> * abductors)
{

	sf::Vector2f alignmentVect = alignment(abductors);
	sf::Vector2f cohesionVect = cohesion(abductors);
	sf::Vector2f separationVect = seperation(abductors);

	m_acceleration += separationVect;
	m_acceleration += cohesionVect * 1.5f;
	m_acceleration += alignmentVect;
	//Think more about whole group of neighbours than single alien
}

sf::Vector2f Abductor::alignment(vector<Abductor *> * abductors)
//Alignment is a behavior that causes a particular agent to line up with agents close by.
{
	sf::Vector2f sum = sf::Vector2f(0, 0);
	int neighborCount = 0;
	float neighbordist = 50;

	for (size_t i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
		if (distBetween > 0 && distBetween < neighbordist)
		{
			sum += (*abductors)[i]->m_velocity;
			neighborCount++;
		}
	}

	if (neighborCount > 0) 
	{
		sum /= static_cast<float>(neighborCount);
		sum = VectorHelper::normalise(sum);
		sum *= m_speed;

		sf::Vector2f steering;
		steering = sum - m_velocity;
		steering = VectorHelper::limitByScalar(steering, m_speed);
		return steering;
	}
	else 
	{
		return sf::Vector2f(0, 0);
	}
}

sf::Vector2f Abductor::seperation(vector<Abductor *> * abductors)
{
	sf::Vector2f steering = sf::Vector2f(0, 0);
	int neighborCount = 0;
	float desiredseparation = 35;
	for (size_t i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
		if (distBetween > 0 && distBetween < desiredseparation)
		{
			sf::Vector2f diff;
			diff = getPosition() - (*abductors)[i]->getPosition();
			diff = VectorHelper::normalise(diff);
			diff /= distBetween;
			steering += diff;
			neighborCount++;
		}
	}

	if (neighborCount > 0) 
	{
		steering /= static_cast<float>(neighborCount);
	}

	if (VectorHelper::magnitude(steering) > 0)
	{
		// Steering = Desired - Velocity
		steering = VectorHelper::normalise(steering);
		steering *= m_speed;
		steering -= m_velocity;
		steering = VectorHelper::limitByScalar(steering, m_speed);
	}

	return steering;
}

sf::Vector2f Abductor::cohesion(vector<Abductor *> * abductors)
//Cohesion is a behavior that causes agents to steer towards the "center of mass" - that is, the average position of the agents within a certain radius.
{
	float neighbordist = 50;

	sf::Vector2f sum(0, 0);
	int neighborCount = 0;

	for (int i = 0; i < abductors->size(); i++)
	{
		float distBetween = VectorHelper::distanceBetweenTwoVectors(getPosition(), (*abductors)[i]->getPosition());
		if ((distBetween > 0) && (distBetween < neighbordist))
		{
			sum += (*abductors)[i]->getPosition();
			neighborCount++;
		}
	}
	if (neighborCount > 0)
	{
		sum /= static_cast<float>(neighborCount);
		return seek(sum);
	}
	else 
	{
		return sf::Vector2f(0, 0);
	}
}

sf::Vector2f Abductor::seek(sf::Vector2f v)
{
	sf::Vector2f desired;
	desired -= v;
	desired = VectorHelper::normalise(desired);
	desired *= m_speed;

	// Steering = Desired minus Velocity
	m_acceleration = desired - m_velocity;
	m_acceleration = VectorHelper::limitByScalar(m_acceleration, m_speed);

	return m_acceleration;
}

void Abductor::patrol(sf::Time deltaTime)
{
	patrolLine[0].position = sf::Vector2f(m_worldBounds.getPosition().x, 300);
	patrolLine[1].position = sf::Vector2f(m_worldBounds.getPosition().x + m_worldBounds.getSize().x, 300);
	
	if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) < static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity.y = m_speed;
	}
	else if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) > static_cast<int>(patrolLine[0].position.y))
	{
		m_velocity.y = -m_speed;
	}
	else 
	{
		m_velocity.y = 0;
	}

}

void Abductor::attack(sf::RectangleShape target)
{
	//std::cout << "In Range " << std::endl;
}

void Abductor::boundaryResponse()
{
	sf::Vector2f vect = m_worldBounds.getSize();

	if (getPosition().y + getSize().y < getSize().y)
	{
		setPosition(sf::Vector2f(getPosition().x, 0));
		m_velocity.y = 0;
	}
	else if (getPosition().y + getSize().y > vect.y)
	{
		setPosition(sf::Vector2f(getPosition().x, vect.y - getSize().y));
		m_velocity.y = 0;
	}

}

void Abductor::update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox)
{
	switch (state)
	{
	case ABDUCT:

		break;
	case PATROL:
		//patrol(deltaTime);
		break;
	default:
		break;
	}

	m_acceleration *= 0.4f;
	// Update velocity
	m_velocity += m_acceleration;
	// Limit speed
	m_velocity = VectorHelper::limitByScalar(m_velocity, m_speed);
	// Reset accelertion to 0 each cycle
	m_acceleration = sf::Vector2f(0, 0);


	//Move Radius with Abductor as the abductor moves
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));

	//Can Abductor Attack
	if (m_canAttack == false)
	{
		m_timeTillNextAttack += deltaTime.asSeconds();
		if (m_timeTillNextAttack > 2.0f)
		{
			m_canAttack = true;
		}
	}
	else if (m_canAttack == true)
	{
		if (rectCollision(m_attackRangeBox, playerBoundingBox)) //Check if value is inside of Attack Boundary;
		{
			//Check if canAttack Player in rang
			attack(playerBoundingBox);// Attack player
			m_canAttack = false;
		}
	}

	boundaryResponse();

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
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
	renderer.draw(patrolLine, 2, sf::Lines);//Debug Patrol Line
	renderer.draw(m_boundingBox);
	//renderer.draw(m_attackRangeBox);
}