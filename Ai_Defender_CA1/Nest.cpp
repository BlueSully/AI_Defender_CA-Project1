#include "Nest.h"

Nest::Nest(sf::Vector2f position, sf::Vector2f veclocity) : m_wander(true), m_flee(false),m_missileNum(2),m_alive(true)
{
	//Gets Called when player is Created
	m_boundingBox.setPosition(position);
	m_boundingBox.setFillColor(sf::Color::Blue);
	m_boundingBox.setSize(sf::Vector2f(44, 16));

	m_velocity = sf::Vector2f(0, 0);

	m_fleeCircle.setFillColor(sf::Color(255, 175, 0, 75));
	m_fleeCircle.setRadius(500);
	m_fleeCircle.setOrigin(sf::Vector2f(m_fleeCircle.getRadius(), m_fleeCircle.getRadius()));

	state = AiState::WANDER;
	m_wanderAngle = 0;
	
	m_velocity = veclocity;
	m_timer = 0;
	m_abductorNum = 0;

	if (!m_nestText.loadFromFile("Resources/Nest.png"))
	{
		std::cout << "File failed to load. Check folder location is correct" << std::endl;
	}

	m_nestSprite.setTexture(m_nestText);
	m_nestSprite.setTextureRect(sf::IntRect(0, 0, 44, 16));
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
bool Nest::isAlive() const
{
	return m_alive;
}

void Nest::setAlive(bool value)
{
	m_alive = value;
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

	circleCenter = VectorHelper::MultiplyByScalar(circleCenter, CIRCLEDISTANCE);
	
	displacement = sf::Vector2f(0, -1);
	displacement = VectorHelper::MultiplyByScalar(displacement, CIRCLE_RADIUS);
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

void Nest::update(sf::Time elapsedTime, sf::RectangleShape  value)
{
	//int randC = rand() % 60 +40;
	m_wanderDelayTimer += elapsedTime.asMilliseconds();

	fleeCollisionCheck(value,elapsedTime);
	boundaryResponse();
	//animate(elapsedTime);
	spawnAbductor(elapsedTime);
	
	m_nestSprite.setPosition(m_boundingBox.getPosition());

	if (m_missileNum <= 2) 
	{
		projMan.UpdateMissile(elapsedTime, value);
	}
	switch (state)
	{
	case WANDER:
		if (m_wanderDelayTimer > 100)//0.1 of a second delay 
		{
			m_steering = wander();
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
void Nest::fleeCollisionCheck(sf::RectangleShape value, sf::Time elapsedTime)
{
	sf::Vector2f temp = sf::Vector2f(value.getPosition().x, value.getPosition().y);
	targetCircle.setPosition(temp);
	targetCircle.setRadius(10);
	targetCircle.setOrigin(sf::Vector2f(targetCircle.getRadius(), targetCircle.getRadius()));

	float a = m_fleeCircle.getPosition().x - targetCircle.getPosition().x;
	float b = m_fleeCircle.getPosition().y - targetCircle.getPosition().y;

	float distance = sqrt((a * a) + (b * b));

	float totalRadius = m_fleeCircle.getRadius() + targetCircle.getRadius();

	if (distance < totalRadius) //Check if value is inside of wander radius;
	{
		setState("Flee");
		setFleeingTarget(temp);
		fireMissiles(value, elapsedTime);
	}
}

void Nest::animate(sf::Time elapsedTime)
{
	m_timer += elapsedTime.asSeconds();
	if (m_timer >= 1)
	{
		m_timer = 0;
		m_nestSprite.setTextureRect(sf::IntRect(0, 0, 44, 16));
	}

}

void Nest::spawnAbductor(sf::Time elapsedTime)
{
	m_abductorTimer += elapsedTime.asSeconds();
	if (m_abductorTimer >= 20 && m_abductorNum < MAX_ABDUCTORS)
	{
		m_abductorTimer = 0;
		//spawn abductor
		m_abductorNum++;
	}

}

void Nest::fireMissiles(sf::RectangleShape  value, sf::Time elapsedTime)
{
	m_missileTimer += elapsedTime.asSeconds();
	//std::cout << m_missileTimer << std::endl;
	if (m_missileNum > 0 && m_missileTimer >= 3.5f)
	{
		projMan.addMissile(value, m_boundingBox.getPosition(), 10);
		m_missileNum--;
		m_missileTimer=0;
	}
	else if (m_missileNum == 0  && projMan.getProjNumber() <= 0)
	{	
		m_missileNum++;
	}
}
void Nest::boundaryResponse()
{
	sf::Vector2f vect = m_worldBounds.getSize();


	if (getPosition().y + getSize().y < (getSize().y + (vect.y / 10.0f)))//radar takes up the top 1/10 of the screen
	{
		setPosition(sf::Vector2f(getPosition().x, (vect.y / 10.0f)));
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
	//renderer.draw(m_boundingBox);
	projMan.Render(renderer);
	renderer.draw(m_nestSprite);
	
}

void Nest::renderRadar(sf::RenderWindow & renderer)
{

	renderer.draw(m_boundingBox);
	projMan.Render(renderer);

}