#pragma once
#include "SFML\Graphics.hpp"
#include <stdlib.h>
#include <time.h>


struct VectorHelper
{

	static float VectorHelper::distanceBetweenTwoVectors(sf::Vector2f a, sf::Vector2f b)
	{
		float distance = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
		return distance;
	}

	static float VectorHelper::magnitude(sf::Vector2f vect) 
	{
		return sqrt((vect.x * vect.x) + (vect.y * vect.y));
	}

	static sf::Vector2f VectorHelper::normalise(sf::Vector2f vect)
	{
		return sf::Vector2f(vect.x / magnitude(vect), vect.y / magnitude(vect));
	}
};