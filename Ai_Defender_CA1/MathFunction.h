#pragma once
#include "SFML\Graphics.hpp"

#define _USE_MATH_DEFINES

#include <random>
#include <limits>       // std::numeric_limits
#include <math.h>
#include <iostream>

using namespace std;

struct CollisionHelper 
{
	public:
		static bool RectangleCollision(sf::Vector2f position1, sf::Vector2f size1, sf::Vector2f position2, sf::Vector2f size2)
		{
			float aLeft = position1.x;
			float aTop = position1.y;
			float aRight = position1.x + size1.x;
			float aBottom = position1.y + size1.y;

			float bLeft = position2.x;
			float bTop = position2.y;
			float bRight = position2.x + size2.x;
			float bBottom = position2.y + size2.y;

			return (aLeft <= bRight &&
				bLeft <= aRight &&
				aTop <= bBottom &&
				bTop <= aBottom);
		}
};

typedef std::pair<int, float> MyPairType;
struct compareValues
{
	bool operator()(const MyPairType& left, const MyPairType& right) const
	{
		return left.second < right.second;
	}
};

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

		return MulitplyByScalar(vect, i);
	}

	static float VectorHelper::distanceBetweenTwoVectors(sf::Vector2f a, sf::Vector2f b)
	{
		float distance = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
		return distance;
	}

	static sf::Vector2f VectorHelper::limitByScalar(sf::Vector2f vect, float scalar)
	{
		if (vect.x > scalar)
		{
			vect.x = scalar;
		}
		else if (vect.x < -scalar)
		{
			vect.x = -scalar;
		}

		if (vect.y > scalar)
		{
			vect.y = scalar;
		}
		else if (vect.y < -scalar)
		{
			vect.y = -scalar;
		}
		return vect;
	}

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

	static sf::Vector2f DivideByScalar(sf::Vector2f vect1, float scaleVAl)
	{
		sf::Vector2f result;

		result.x = vect1.x / scaleVAl;
		result.y = vect1.y / scaleVAl;
		return result;
	}


	static sf::Vector2f MulitplyByScalar(sf::Vector2f vectToScale, float scaleVAl)
	{
		sf::Vector2f result;
		result = vectToScale * scaleVAl;
		return result;
	}
};