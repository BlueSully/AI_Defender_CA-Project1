#include "Mutant.h"

Mutant::Mutant()
{

}

Mutant::Mutant(sf::Vector2f position, sf::Vector2f velocity) : m_state(SEEK), m_velocity(velocity),m_lives(2)
{
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(32, 32));
	m_boundingBox.setFillColor(sf::Color::Magenta);

	m_acceleration = sf::Vector2f(0, 0);
}

Mutant::~Mutant()
{

}

sf::Vector2f Mutant::getSize() const
{
	return m_boundingBox.getSize();
}
std::vector<Projectile> Mutant::getProjList() const
{
	return projMan.getProjList();
}
sf::Vector2f Mutant::getVelocity() const
{
	return m_velocity;
}

void Mutant::setVelocity(sf::Vector2f value)
{
	m_velocity = value;
}
int Mutant::getLives() const
{
	return m_lives;
}
void Mutant::setLives(int value)
{
	m_lives = value;
}

sf::Vector2f Mutant::getPosition() const
{
	return m_boundingBox.getPosition();
}

void Mutant::setPosition(sf::Vector2f value)
{
	m_boundingBox.setPosition(value);
}

bool Mutant::isAlive() const
{
	return m_alive;
}

void Mutant::setAlive(bool value)
{
	m_alive = value;
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
	sf::Vector2f sum = sf::Vector2f(0, 0);

	int attraction = 75;
	int repulsion = 5000;
	int attractionExp = 1;
	int repulsionExp = 2;

	for (int i = 0; i < mutantsList.size(); i++)
	{
		if (mutantsList[i]->getPosition() != getPosition()) 
		{
			sf::Vector2f R = getPosition() - mutantsList[i]->getPosition();
			float dist = VectorHelper::magnitude(R);
			if (dist < 150) 
			{
				float U = (-attraction / (pow(dist, attractionExp))) + (repulsion / (pow(dist, repulsionExp)));
				R = VectorHelper::normalise(R);
				m_acceleration += R * U;
			}
		}
	}
}

void Mutant::update(sf::Time deltaTime, sf::Vector2f playerPostion)
{
	sf::Vector2f steering;
	switch (m_state)
	{
	case SEEK:
		m_acceleration += seek(playerPostion) *0.05f;
		break;
	default:
		break;
	}
	projMan.Update(deltaTime);
	m_acceleration *= 0.4f;

	m_velocity += m_acceleration;

	m_velocity = VectorHelper::truncate(m_velocity + steering, MAX_VELOCITY);

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());

	m_acceleration = sf::Vector2f(0, 0);
}

void Mutant::render(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
	projMan.Render(renderer);
}
void Mutant::renderRadar(sf::RenderWindow & renderer)
{

	renderer.draw(m_boundingBox);
	projMan.Render(renderer);
}
