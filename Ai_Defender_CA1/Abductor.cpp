#include "Abductor.h"



Abductor::Abductor()
{
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Red);
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

void Abductor::flock()
{

}

void Abductor::patrol()
{

}

void Abductor::attack(sf::Vector2f)
{

}

void Abductor::update()
{
	switch (state)
	{
	case ABDUCT:

		break;
	case PATROL:

		break;
	default:
		break;
	}
}

void Abductor::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
}