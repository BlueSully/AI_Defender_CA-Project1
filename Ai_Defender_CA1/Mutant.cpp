#include "Mutant.h"

Mutant::Mutant()
{

}

Mutant::Mutant(sf::Vector2f position, sf::Vector2f velocity) : m_state(SEEK), m_velocity(velocity)
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
	// Calculate the desired velocity
	sf::Vector2f desired_velocity = playerPostion - getPosition();
	float distance = VectorHelper::magnitude(desired_velocity);

	// check to See if Mutant enters slow down area
	if (distance < 175 )//&& distance > 75)
	{
		//if inside slowdown area slow down
		desired_velocity = VectorHelper::normalise(desired_velocity) * MAX_VELOCITY * (distance / 175);
	}
	else if (distance < 75)
	{
		desired_velocity = VectorHelper::normalise(desired_velocity) * (distance / 175);
	}
	else 
	{
		//if outside slowdown area no limts
		desired_velocity = VectorHelper::normalise(desired_velocity) * MAX_VELOCITY;
	}

	return desired_velocity - m_velocity;
}

void Mutant::swarm(vector<Mutant *> mutantsList)
{
/*  
    Function CalcLJ(Unit me, Unit you)
    Vector R = me.position - you.position
    Real D = R.magnitude()
    Real U = -A/pow(D,N)+ B/pow(D,M)
    R.normalise()

    Me.force+=vrotate2D(me.Orientation, R * U)
*/
	sf::Vector2f sum = sf::Vector2f(0, 0);

	int attraction = 50;
	int repulsion = 5000;
	int attractionExp = 1;
	int repulsionExp = 2;

	for (int i = 0; i < mutantsList.size(); i++)
	{
		if (mutantsList[i]->getPosition() != getPosition()) 
		{
			sf::Vector2f R = getPosition() - mutantsList[i]->getPosition();
			float dist = VectorHelper::magnitude(R);
			float U = (-attraction / (pow(dist, attractionExp))) + (repulsion / (pow(dist, repulsionExp)));
			R = VectorHelper::normalise(R);
			m_acceleration += R * U;
		}
	}
}

void Mutant::update(sf::Time deltaTime, sf::Vector2f playerPostion)
{
	sf::Vector2f steering;
	switch (m_state)
	{
	case SEEK:
		//m_acceleration += seek(playerPostion) * 0.001f;
		break;
	default:
		break;
	}

	//To make the slow down not as abrupt
	m_acceleration *= 0.4f;
	// Update velocity
	m_velocity += m_acceleration;

	m_velocity = VectorHelper::truncate(m_velocity, MAX_VELOCITY);

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());

	// Reset accelertion to 0 each cycle
	m_acceleration = sf::Vector2f(0, 0);
}

void Mutant::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
}
