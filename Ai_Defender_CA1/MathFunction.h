#pragma once
#include "SFML\Graphics.hpp"
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

struct VectorHelper
{
public:
	static sf::Vector2f setVectorByAngle(sf::Vector2f vect1, float value)
	{
		sf::Vector2f result;
		float length = sqrt((vect1.x * vect1.x) + (vect1.y * vect1.y));

		result.x = cos(value) * length;
		result.y = sin(value) * length;

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

	//Calculate the angle between Pvector 1 and Pvector 2
	static float angleBetween(sf::Vector2f a, sf::Vector2f b)
	{
		if (a.x == 0 && a.y == 0) return 0.0f;
		if (b.x == 0 && b.y == 0) return 0.0f;

		double dot = a.x * b.x + a.y * b.y;
		double v1mag = magnitude(a);
		double v2mag = magnitude(b);
		double amt = dot / (v1mag * v2mag); //Based of definition of dot product
											//dot product / product of magnitudes gives amt
		if (amt <= -1) 
		{
			return static_cast<float>(M_PI);
		}
		else if (amt >= 1) 
		{
			return 0;
		}
		float tmp = static_cast<float>(acos(amt));
		return tmp;
	}

	static sf::Vector2f Mulitply(sf::Vector2f vect1, sf::Vector2f vect2)
	{
		sf::Vector2f result;

		result.x = vect1.x * vect2.x;
		result.y = vect1.y * vect2.y;
		return result;
	}

	static sf::Vector2f scaleBy(sf::Vector2f vectToScale, float scaleVAl)
	{
		sf::Vector2f result;
		result = vectToScale * scaleVAl;
		return result;
	}
};