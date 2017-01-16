#include "Mutant.h"

Mutant::Mutant()
{

}

Mutant::Mutant(sf::Vector2f position, sf::Vector2f velocity) : m_state(SEEK)
{
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Magenta);

	m_acceleration = sf::Vector2f(0, 0);
}

Mutant::~Mutant()
{

}

sf::Vector2f Mutant::getVelocity() const
{
	return m_velocity;
}

void Mutant::setVelocity(sf::Vector2f value)
{
	m_velocity = value;
}

sf::Vector2f Mutant::getPosition() const
{
	return m_boundingBox.getPosition();
}

void Mutant::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

sf::Vector2f Mutant::seek(sf::Vector2f playerPostion)
{
	sf::Vector2f desired_velocity = VectorHelper::normalise(sf::Vector2f(playerPostion.x - getPosition().x, playerPostion.y - getPosition().y)) * MAX_VELOCITY;
	return desired_velocity - m_velocity;
}

void Mutant::swarm()
{

}

void Mutant::update(sf::Time deltaTime, sf::Vector2f playerPostion)
{
	sf::Vector2f steering;
	switch (m_state)
	{
	case SEEK:
		steering = seek(playerPostion);
		break;
	default:
		break;
	}

	m_velocity = VectorHelper::truncate(m_velocity + steering, MAX_VELOCITY);

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
}

void Mutant::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
}
