#include "Astronaut.h"

Astronaut::Astronaut() : m_beingAbducted(false), m_abductorId(-1)
{

	//Gets Called when player is Created
	m_boundingBox.setFillColor(sf::Color::White);
	m_boundingBox.setSize(sf::Vector2f(16, 32));
	m_velocity = sf::Vector2f(0, 0);
	m_state = AiState::WANDER;
	m_wanderAngle = 0;
}

Astronaut::Astronaut(sf::Vector2f position, sf::Vector2f veclocity) : m_beingAbducted(false), m_abductorId(-1)
{
	//Gets Called when player is Created
	m_boundingBox.setFillColor(sf::Color::White);
	m_boundingBox.setSize(sf::Vector2f(16, 32));
	m_velocity = sf::Vector2f(0, 0);
	m_state = AiState::WANDER;
	m_wanderAngle = 0;

	m_boundingBox.setPosition(sf::Vector2f(position.x, (position.y - m_boundingBox.getSize().y) - 10));
	m_velocity = veclocity;
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

void Astronaut::setFleeTarget(sf::Vector2f * value)
{
	m_fleeTarget = value;
}

sf::Vector2f Astronaut::getVelocity()
{
	return m_velocity;
}

AiState Astronaut::getState() const
{
	return m_state;
}

void Astronaut::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

void Astronaut::setState(AiState value)
{
	m_state = value;
}

void Astronaut::setBeingAbducted(bool value)
{
	m_beingAbducted = value;
}

bool Astronaut::getBeingAbducted() const
{
	return m_beingAbducted;
}

int Astronaut::getAbuctorId() const
{
	return m_abductorId;
}

void Astronaut::setAbductorId(int value)
{
	m_abductorId = value;
}

void Astronaut::setFollowTarget(sf::Vector2f & value, sf::Vector2f size)
{
	m_followTarget = &value;
	m_followSize = size;
}

sf::Vector2f Astronaut::wander()
{	
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

sf::Vector2f Astronaut::flee()
{
	sf::Vector2f desired_velocity = VectorHelper::normalise(getPosition() - *m_fleeTarget) * MAX_VELOCITY;
	sf::Vector2f steering = desired_velocity - m_velocity;
	steering.y = 0;
	if (steering.x < 0.1f)
	{
		steering.x = -MAX_VELOCITY;
	}
	return steering;
}

void Astronaut::update(sf::Time elapsedTime)
{
	m_wanderDelayTimer += elapsedTime.asMilliseconds();
	switch (m_state)
	{
	case GRABBED:
		m_steering = sf::Vector2f(0, 0);
		m_velocity = sf::Vector2f(0, 0);
		m_boundingBox.setPosition(sf::Vector2f((*m_followTarget).x, (*m_followTarget).y + m_followSize.y));
		break;
	case WANDER:
		if (m_wanderDelayTimer > 100)//0.1 of a second delay 
		{
			m_steering = wander();
			m_steering = VectorHelper::truncate(m_steering, MAX_FORCE);
			m_wanderDelayTimer = 0;
		}
		break;
	case FLEE:
		m_steering = flee();
		break;
	default:
		break;
	}

	if (m_state != GRABBED) 
	{
		m_velocity = VectorHelper::truncate(m_velocity + m_steering, MAX_VELOCITY);

		m_boundingBox.move(m_velocity * elapsedTime.asSeconds());
	}
}

void Astronaut::boundaryResponse(sf::Vector2f worldSize)
{
	sf::Vector2f vect = sf::Vector2f(worldSize.x, worldSize.y);

	if ((getPosition().y + getSize().y < (vect.y - 75) + getSize().y) && !m_beingAbducted)
	{
		setPosition(sf::Vector2f(getPosition().x, vect.y - 75));
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
}