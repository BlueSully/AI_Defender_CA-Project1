#include "Nest.h"

Nest::Nest(sf::Vector2f position, sf::Vector2f veclocity) : m_wander(true), m_flee(false)
{
	//Gets Called when player is Created
	m_boundingBox.setFillColor(sf::Color::White);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_velocity = sf::Vector2f(0, 0);
	m_fleeCircle.setFillColor(sf::Color(255, 175, 0, 75));
	m_fleeCircle.setRadius(240);
	m_fleeCircle.setOrigin(sf::Vector2f(m_fleeCircle.getRadius(), m_fleeCircle.getRadius()));
	state = AiState::WANDER;
	m_wanderAngle = 0;
	m_boundingBox.setPosition(position);
	m_velocity = veclocity;

}

Nest::~Nest()
{

}

sf::Vector2f Nest::getSize()
{
	return m_boundingBox.getSize();
}

sf::Vector2f Nest::getPosition()
{
	return m_boundingBox.getPosition();
}

void Nest::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

void Nest::setWorldRectangle(sf::Vector2f postion, sf::Vector2f size)
{
	m_worldBounds.setPosition(postion);
	m_worldBounds.setSize(size);
}

void Nest::setColour(sf::Color colour)
{
	m_boundingBox.setFillColor(colour);
}

sf::Vector2f Nest::wander()
{
	srand(static_cast<unsigned int>(time(NULL)));
	sf::Vector2f wanderForce, circleCenter, displacement;

	circleCenter = m_velocity;	

	if (circleCenter.x != 0 || circleCenter.y != 0) 
	{
		circleCenter = VectorHelper::normalise(circleCenter);
	}

	circleCenter = VectorHelper::scaleBy(circleCenter, CIRCLEDISTANCE);

	displacement = sf::Vector2f(0, -1);
	displacement = VectorHelper::scaleBy(displacement, CIRCLE_RADIUS);
	displacement = VectorHelper::setVectorByAngle(displacement, m_wanderAngle);

	float randomF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX + 1);
	m_wanderAngle += (randomF * ANGLECHANGE) - (ANGLECHANGE - 0.5f);

	wanderForce = circleCenter + displacement;

	return wanderForce;
}

sf::Vector2f Nest::flee()
{
	sf::Vector2f desired_velocity = VectorHelper::normalise(m_fleeFromPos - getPosition()) * 100.0f;
	return (desired_velocity - m_velocity);
}

void Nest::update(sf::Time elapsedTime)
{
	int randC = rand() % 60 +40;
	m_wanderDelayTimer += elapsedTime.asMilliseconds();
	std::cout << swap << std::endl;
	switch (state)
	{
	case WANDER:
		if (m_wanderDelayTimer > randC)//0.1 of a second delay 
		{
			swap++;
			if (swap <50)
			{
				m_steering = wander();
				
			}
			else if (swap<rand() % 90 + 51 && swap > 50)
			{
					m_steering = sf::Vector2f((1.5f * wander().x), (-1.5f * wander().y));

				
			}
			else if (swap<rand() % 140 + 91 && swap > 90)
			{
				m_steering = sf::Vector2f((-1.5f * wander().x), (1.5f * wander().y));
				swap = 0;

			}
			

			m_steering = VectorHelper::truncate(m_steering, MAX_FORCE);
			m_wanderDelayTimer = 0;
		}
		break;
	case FLEE:
		m_steering = -flee();
		break;
	default:
		break;
	}

	setState("Wander");
	//std::cout << m_wanderDelayTimer << std::endl;
	m_velocity = VectorHelper::truncate(m_velocity + m_steering, MAX_VELOCITY);
	m_boundingBox.move(m_velocity * elapsedTime.asSeconds());
	
	
	m_fleeCircle.setPosition(m_boundingBox.getPosition() + sf::Vector2f((m_boundingBox.getSize().x / 2), (m_boundingBox.getSize().y / 2)));
}

void Nest::setFleeingTarget(sf::Vector2f value)
{
	m_fleeFromPos = value;
}

void Nest::setState(std::string str)
{
	if (str == "Wander")
	{
		state = AiState::WANDER;
	}
	else if (str == "Flee") 
	{
		state = AiState::FLEE;
	}	
}

void Nest::fleeCollisionCheck(sf::Vector2f value)
{
	targetCircle.setPosition(value);
	targetCircle.setRadius(32);
	targetCircle.setOrigin(sf::Vector2f(targetCircle.getRadius(), targetCircle.getRadius()));

	float a = m_fleeCircle.getPosition().x - targetCircle.getPosition().x;
	float b = m_fleeCircle.getPosition().y - targetCircle.getPosition().y;

	float distance = sqrt((a * a) + (b * b));

	float totalRadius = m_fleeCircle.getRadius() + targetCircle.getRadius();

	if (distance < totalRadius) //Check if value is inside of wander radius;
	{
		setState("Flee");
		setFleeingTarget(value);
	}
}

void Nest::boundaryResponse(sf::Vector2f worldSize)
{
	sf::Vector2f vect = sf::Vector2f(worldSize.x, worldSize.y);

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

void Nest::render(sf::RenderWindow & renderer)
{
	//renderer.draw(m_fleeCircle);
	//renderer.draw(targetCircle);
	renderer.draw(m_boundingBox);
	
}