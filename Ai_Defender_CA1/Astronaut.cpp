#include "Astronaut.h"

Astronaut::Astronaut() : m_wander(true), m_flee(false)
{
	//Gets Called when player is Created
	m_boundingBox.setFillColor(sf::Color::White);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_velocity = sf::Vector2f(0, 0);

	m_wanderAngle = 0;

}

Astronaut::~Astronaut()
{

}

sf::Vector2f Astronaut::getSize()
{
	return m_boundingBox.getSize();
}

sf::Vector2f Astronaut::getPosition()
{
	return m_boundingBox.getPosition();
}

void Astronaut::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

void Astronaut::init(sf::Vector2f position, sf::Vector2f veclocity)
{
	//Sets up the variables for this Astronauts position, etc;
	m_boundingBox.setPosition(position);
	m_velocity = veclocity;

	m_wanderDebugCircle.setFillColor(sf::Color::Yellow);
	m_wanderDebugCircle.setRadius(CIRCLE_RADIUS);

}

sf::Vector2f Astronaut::wander()
{
	srand(static_cast<unsigned int>(time(NULL)));
	sf::Vector2f wanderForce, circleCenter, displacement;

	circleCenter = m_velocity;	
	if (circleCenter.x != 0 || circleCenter.y != 0) 
	{
		circleCenter = VectorHelper::normalise(circleCenter);
	}

	circleCenter = VectorHelper::scaleBy(circleCenter, CIRCLEDISTANCE);

	m_wanderDebugCircle.setPosition(circleCenter + getPosition());

	displacement = sf::Vector2f(0, -1);
	displacement = VectorHelper::scaleBy(displacement, CIRCLE_RADIUS);
	displacement = VectorHelper::setVectorByAngle(displacement, m_wanderAngle);

	float randomF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX + 1);
	m_wanderAngle += (randomF * ANGLECHANGE) - (ANGLECHANGE - 0.5f);

	wanderForce = circleCenter + displacement;

	return wanderForce;
}

void Astronaut::flee()
{

}

void Astronaut::align(sf::Vector2f target)
{
	float angletoTarget = VectorHelper::angleBetween(target, getPosition());
	float angleToMove = m_orientation - angletoTarget;

	while (m_orientation != angletoTarget )
	{
		if (angleToMove > 0)
		{
			m_orientation -= 0.1f;
		}
		else if (angleToMove < 0)
		{
			m_orientation += 0.1f;
		}
		angleToMove = m_orientation - angletoTarget;
	}
	//m_orientation = angletoTarget;
}

void Astronaut::face(sf::Vector2f target)
{
	sf::Vector2f direction = target - getPosition();

	if (VectorHelper::magnitude(direction) != 0) 
	{
		float newOrientation = atan2(-direction.x, direction.y);

		align(target);
	}
}

float Astronaut::getNewOrientation(float currentOrientation, sf::Vector2f velocity)
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

void Astronaut::update(sf::Time elapsedTime)
{
	/*if (m_wanderAngle > 360) 
	{
		m_wanderAngle = 0;
	}
	else if (m_wanderAngle < 0)
	{
		m_wanderAngle = 360;
	}*/

	m_wanderDelayTimer += elapsedTime.asMilliseconds();
	if (m_wander)
	{
		if (m_wanderDelayTimer > 100)//0.1 of a second delay 
		{
			m_steering = wander();
			m_steering = VectorHelper::truncate(m_steering, MAX_FORCE);
			m_wanderDelayTimer = 0;
		}
	}

	m_orientation = getNewOrientation(m_orientation, m_velocity);
	double degreeort = m_orientation * 180 / M_PI;

	m_velocity = VectorHelper::truncate(m_velocity + m_steering, MAX_VELOCITY);

	std::cout << "pos: " << getPosition().x << "," << getPosition().x << std::endl;

	m_boundingBox.move(m_velocity * elapsedTime.asSeconds());
}

void Astronaut::boundaryResponse(sf::Vector2f worldSize)
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

void Astronaut::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
	renderer.draw(m_wanderDebugCircle);
}