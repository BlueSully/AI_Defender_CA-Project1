#pragma once
#include "SFML\Graphics.hpp"
struct VectorHelper
{
public:
	static sf::Vector2f setVectorByAngle(sf::Vector2f vect1, float vect2)
	{
		sf::Vector2f result;
		float length = sqrt((vect1.x * vect1.x) + (vect1.y * vect1.y));

		result.x = cos(vect2) * length;
		result.y = sin(vect2) * length;

		return result;
	}

	static sf::Vector2f normalise(sf::Vector2f vect)
	{
		sf::Vector2f result;
		result = vect / magnitude(vect);
		return result;
	}

	static float magnitude(sf::Vector2f vect)
	{
		float result;
		result = std::sqrt((vect.x * vect.x) + (vect.y * vect.y));
		return result;
	}

	static sf::Vector2f truncate(sf::Vector2f vect, float value) 
	{
		float i = 0;
		float mag = magnitude(vect);
		i = value / magnitude(vect);
		i = i < 1.0f ? i : 1.0f;

		return scaleBy(vect, i);
	}

	static sf::Vector2f scaleBy(sf::Vector2f vectToScale, float scaleVAl)
	{
		sf::Vector2f result;
		result = vectToScale * scaleVAl;
		return result;
	}
};