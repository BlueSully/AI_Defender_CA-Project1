#include "Abductor.h"

Abductor::Abductor()
	: m_attackRange(17),
	m_speed(35),
	m_state(PATROL),
	m_canAttack(true),
	m_grabbedAstronaut(false),
	m_spawnMutant(false),
	m_alive(true),
	m_timeTillNextAttack(0),
	m_abducteeId(-1)
{
	m_boundingBox.setPosition(sf::Vector2f(0, 300));
	m_boundingBox.setSize(sf::Vector2f(36, 32));
	m_boundingBox.setFillColor(sf::Color::Cyan);

	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));
	
	m_position = m_boundingBox.getPosition();
	m_velocity = getDirection();
	m_acceleration = sf::Vector2f(0, 0);

	if (!m_abductorText.loadFromFile("Resources/Abductor.png"))
	{
		std::cout << "File failed to load. Check folder location is correct" << std::endl;
	}

	m_abductorSprite.setTexture(m_abductorText);
	m_abductorSprite.setTextureRect(sf::IntRect(0, 0, 36, 32));
}

Abductor::Abductor(sf::Vector2f position, sf::Vector2f windowSize, int id, float speed)
	: m_attackRange(17),
	m_speed(speed),
	m_state(PATROL),
	m_canAttack(true),
	m_grabbedAstronaut(false),
	m_spawnMutant(false),
	m_alive(true),
	m_timeTillNextAttack(0),
	m_abducteeId(-1)
{
	m_boundingBox.setPosition(position);
	m_boundingBox.setSize(sf::Vector2f(36, 32));
	m_boundingBox.setFillColor(sf::Color::Cyan);
	
	m_attackRangeBox.setFillColor(sf::Color(125, 50, 0, 175));
	m_attackRangeBox.setSize(sf::Vector2f(static_cast<float>(32 * m_attackRange), static_cast<float>(32 * m_attackRange)));
	m_attackRangeBox.setPosition(m_boundingBox.getPosition() - sf::Vector2f(static_cast<float>(32 * (m_attackRange / 2)), static_cast<float>(32 * (m_attackRange / 2))));

	int dir = 0;

	if (id < 3) 
	{
		dir = 1;
	}
		
	setDirection(dir);
	m_velocity = getDirection();
	m_acceleration = sf::Vector2f(0, 0);
	m_position = m_boundingBox.getPosition();

	if (!m_abductorText.loadFromFile("Resources/Abductor.png"))
	{
		std::cout << "File failed to load. Check folder location is correct" << std::endl;
	}

	m_abductorSprite.setTexture(m_abductorText);
	m_abductorSprite.setTextureRect(sf::IntRect(0, 0, 36, 32));

}

Abductor::~Abductor()
{

}

sf::Vector2f Abductor::getSize() const
{
	return m_boundingBox.getSize();
}

sf::Vector2f & Abductor::getPosition()
{
	return m_position;
}

sf::RectangleShape Abductor::getBoundingBox() const
{
	return m_boundingBox;
}

AiState Abductor::getState() const
{
	return m_state;
}

sf::Vector2f Abductor::getVelocity() const
{
	return m_velocity;
}

sf::Vector2f Abductor::getDirection() const
{
	return directionVector;
}

int Abductor::getAbucteeId() const
{
	return m_abducteeId;
}

void Abductor::setAbducteeId(int astronnautId)
{
	m_abducteeId = astronnautId;
}

bool Abductor::getGrabbedAstronaut() const
{
	return m_grabbedAstronaut;
}

void Abductor::setGrabbedAstronaut(bool value)
{
	m_grabbedAstronaut = value;
}

bool Abductor::canSpawnMutant() const
{
	return m_spawnMutant;
}

bool Abductor::isAlive() const
{
	return m_alive;
}

void Abductor::setAlive(bool value)
{
	m_alive = value;
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

void Abductor::setAbductorTarget(sf::Vector2f * target)
{
	m_abducteePosition = target;
}

void Abductor::setState(AiState value)
{
	m_state = value;
}

void Abductor::setVelocity(sf::Vector2f value)
{
	m_velocity = value;
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
	float desiredseparation = 25;
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

void Abductor::abduct(sf::Time deltaTime, sf::Vector2f * pos)
{
	m_abducteePosition = pos;

	if (static_cast<int>(m_abducteePosition->x) > static_cast<int>(getPosition().x))
	{
		m_velocity.x = m_speed;
	}
	else if (static_cast<int>(m_abducteePosition->x) < static_cast<int>(getPosition().x))
	{
		m_velocity.x = -m_speed;
	}
	else if (static_cast<int>(m_abducteePosition->x) == static_cast<int>(getPosition().x))
	{
		m_velocity.x = 0;
	}
}

void Abductor::patrol(sf::Time deltaTime)
{
	int patrolHeight = rand() % 300 + 100;

	if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) < static_cast<int>(patrolHeight))
	{
		m_velocity.y = m_speed;
	}
	else if (static_cast<int>(m_boundingBox.getPosition().y + m_boundingBox.getSize().y / 2) > static_cast<int>(patrolHeight))
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
	if (target.getPosition().x < m_boundingBox.getPosition().x)
	{
		projMan.addLaser(true, m_boundingBox.getPosition(), m_velocity.x, 3, true);
	}
	else
	{
		projMan.addLaser(false, m_boundingBox.getPosition(), m_velocity.x, 3, true);
	}
}



void Abductor::animate(sf::Time elapsedTime)
{
	m_timer += elapsedTime.asSeconds();
	if (m_timer >= 1)
	{
		m_timer = 0;
		m_abductorSprite.setTextureRect(sf::IntRect(0, 0, 36, 32));
	}
}

void Abductor::boundaryResponse()
{
	sf::Vector2f vect = m_worldBounds.getSize();

	if (getPosition().y + getSize().y < getSize().y )
	{
		setPosition(sf::Vector2f(getPosition().x, 0));
		m_velocity.y = 0;

		if (m_grabbedAstronaut) 
		{
			m_spawnMutant = true;
		}
	}
	else if (getPosition().y + getSize().y > vect.y)
	{
		setPosition(sf::Vector2f(getPosition().x, vect.y - getSize().y));
		m_velocity.y = 0;
	}
}

void Abductor::update(sf::Time deltaTime, sf::RectangleShape playerBoundingBox)
{
	
	switch (m_state)
	{
	case ABDUCTING:
		if (m_grabbedAstronaut)
		{
			m_velocity = sf::Vector2f(0, -(m_speed / 2));
		}
		else if (!m_grabbedAstronaut) 
		{
			m_velocity = sf::Vector2f(m_velocity.x, (m_speed / 2));
		}
		break;
	case PATROL:
		patrol(deltaTime);
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
	//Sprite and Animation 
	m_abductorSprite.setPosition(m_boundingBox.getPosition());
	animate(deltaTime);
	//Can Abductor Attack
	if (m_canAttack == false)
	{
		m_timeTillNextAttack += deltaTime.asSeconds();
		if (m_timeTillNextAttack > 2.0f)
		{
			m_canAttack = true;
			m_timeTillNextAttack = 0;
		}
	}
	else if (m_canAttack == true)
	{
		if (CollisionHelper::RectangleCollision(m_attackRangeBox.getPosition(), m_attackRangeBox.getSize(), playerBoundingBox.getPosition(), playerBoundingBox.getSize())) //Check if value is inside of Attack Boundary;
		{
			//Check if canAttack Player in rang
			attack(playerBoundingBox);// Attack player
			m_canAttack = false;
		}
	}

	boundaryResponse();
	projMan.Update(deltaTime, playerBoundingBox);

	m_boundingBox.move(m_velocity * deltaTime.asSeconds());
	m_position = m_boundingBox.getPosition();
}

void Abductor::render(sf::RenderWindow & renderer)
{
	//renderer.draw(m_boundingBox);
	//renderer.draw(m_attackRangeBox);
	renderer.draw(m_abductorSprite);
	projMan.Render(renderer);
}

void Abductor::renderRadar(sf::RenderWindow & renderer)
{
	renderer.draw(m_boundingBox);
	projMan.Render(renderer);
}