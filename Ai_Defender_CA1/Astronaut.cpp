#include "Astronaut.h"

Astronaut::Astronaut() : m_wander(true), m_flee(false)
{
	//Gets Called when player is Created
	m_boundingBox.setFillColor(sf::Color::Yellow);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	
	/*m_wanderCircle.setFillColor(sf::Color(0, 0, 255, 125));
	m_wanderCircle.setRadius(32);*/
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
}

sf::Vector2f Astronaut::wander()
{
	srand(static_cast<unsigned int>(time(NULL)));

	sf::Vector2f circleCenter = m_velocity;
	circleCenter = VectorHelper::normalise(circleCenter);
	circleCenter = VectorHelper::scaleBy(circleCenter, CIRCLEDISTANCE);


	sf::Vector2f displacement = sf::Vector2f(0, -1);
	displacement = VectorHelper::scaleBy(displacement, CIRCLE_RADIUS);

	displacement = VectorHelper::setVectorByAngle(displacement, m_wanderAngle);

	float randomF = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	m_wanderAngle += ((rand() % 2  * ANGLECHANGE) - (ANGLECHANGE - 0.5f));

	sf::Vector2f m_wanderforce = circleCenter + displacement;

	return m_wanderforce;
}

void Astronaut::flee()
{
}

void Astronaut::update(sf::Time elapsedTime)
{
	if (m_wander) 
	{
		m_steering = wander();
		m_steering = VectorHelper::truncate(m_steering, MAX_FORCE);
	}

	m_velocity += m_steering;
	m_velocity = VectorHelper::truncate(m_velocity, MAX_VELOCITY);

	std::cout << "posX: " << getPosition().x << " posY: " << getPosition().y << "veloX: " << m_velocity.x << " veloY: " << m_velocity.y << " angle: " << m_wanderAngle << std::endl;
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
}