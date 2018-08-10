/*
 * Utilities.hpp
 *
 *  Created on: 28 août 2017
 *      Author: Clement
 */

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <random>
#include "Asteroid.hpp"
#include "Ship.hpp"
#include "Constants.hpp"
#include "GameParameters.hpp"

namespace utilities {

	static std::mt19937 g(time(NULL));

	// Asteroid randomization
	int randomVertex();
	float randomRadius();
	float randomAngle(float min, float max);
	sf::Vector2f randomVelocity();
	sf::Vector2f randomPosition();
	sf::Vector2f randomOutOfScreenPosition();

	sf::Vector2f direction(float angle);

	// Angle
	float degreeToRadian(float degre);
	float radianToDegree(float radian);
	bool isInRange(unsigned int value, unsigned int minValue, unsigned int maxValue);

	// Bounding boxes
	bool contains(const sf::CircleShape& circle, const sf::Vector2f& point);
	bool intersects(const sf::CircleShape& circle, const sf::FloatRect& rect);
	bool intersects(const sf::CircleShape& circle, const sf::RectangleShape& rectangle);
	bool intersects(const sf::CircleShape& circle, const sf::CircleShape& other);
	bool intersects(const Asteroid& asteroid, const Asteroid& other);
	bool intersects(const Asteroid& asteroid, const sf::RectangleShape& projectile);
	bool intersects(const Asteroid& asteroid, const Ship& ship);

	sf::Vector2f nearest(const Asteroid& asteroid, const Ship& ship);
	sf::Vector2f nearest(const sf::CircleShape& circle, const sf::RectangleShape& rectangle);

	// Vectors
	float distance(sf::Vector2f a, sf::Vector2f b);
	float distance(Asteroid asteroid, Asteroid other);
	float length( sf::Vector2f a);
	float dot(sf::Vector2f a,  sf::Vector2f b);
	float perProduct(sf::Vector2f a,  sf::Vector2f b);
	sf::Vector2f normalize(sf::Vector2f v);
	sf::Vector2f project( sf::Vector2f a, sf::Vector2f b);
	sf::Vector2f leftHandNormal(sf::Vector2f a);
	sf::Vector2f rightHandNormal(sf::Vector2f a);
}

#endif /* UTILITIES_HPP_ */
