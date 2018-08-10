/*
 * Utilities.cpp
 *
 *  Created on: 28 août 2017
 *      Author: Clement
 */

#include <cmath>
#include <SFML/Graphics.hpp>

#include "Game.hpp"
#include "Asteroid.hpp"
#include "Ship.hpp"
#include "Utilities.hpp"
#include "Constants.hpp"

namespace utilities {

	sf::Vector2f direction(float angle) {
		float vx,vy;
		vx = cos(angle * constants::pi / 180);
		vy = sin(angle * constants::pi / 180);
		return sf::Vector2f(vx,vy);
	}

	float degreeToRadian(float degre) {
		return degre * constants::pi / 180.0f;
	}

	float radianToDegree(float radian) {
		return radian * 180.0f / constants::pi;
	}

	float computeDistance(const sf::Vector2f a, const sf::Vector2f b) {
		return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	}

	bool isInRange(unsigned int value, unsigned int minValue, unsigned int maxValue) {
		return (value < minValue || value > maxValue) ? (false) : (true);
	}

	bool contains(const sf::CircleShape& circle, const sf::Vector2f& point) {
		float d = utilities::computeDistance(circle.getPosition(),point);
		return d <= circle.getRadius();
	}

	sf::Vector2f nearest(const Asteroid& asteroid, const Ship& ship) {
		return utilities::nearest(asteroid.getBB(),ship.getBB());
	}

	sf::Vector2f nearest(const sf::CircleShape& circle, const sf::RectangleShape& rectangle) {
		sf::Vector2f origin = rectangle.getTransform().transformPoint(circle.getPosition());
		float nearestX = std::max(rectangle.getLocalBounds().left, std::min(origin.x, rectangle.getLocalBounds().left + rectangle.getLocalBounds().width));
		float nearestY = std::max(rectangle.getLocalBounds().top,  std::min(origin.y, rectangle.getLocalBounds().top  + rectangle.getLocalBounds().height));
		return sf::Vector2f(nearestX,nearestY);
	}

	bool intersects(const sf::CircleShape& circle, const sf::FloatRect& range) {
		float r = circle.getRadius();
		float nearestX = std::max(range.left, std::min(circle.getPosition().x, range.left + range.width));
		float nearestY = std::max(range.top,  std::min(circle.getPosition().y, range.top  + range.height));
		float dx = circle.getPosition().x - nearestX;
		float dy = circle.getPosition().y - nearestY;
		return (dx * dx + dy * dy) < (r * r);
	}

	bool intersects(const sf::CircleShape& circle, const sf::RectangleShape& rectangle) {
		return intersects(circle,rectangle.getGlobalBounds());
	}

	bool intersects(const sf::CircleShape& circle, const sf::CircleShape& other) {
		auto d = utilities::distance(circle.getPosition(),other.getPosition());
		return d <= circle.getRadius() + other.getRadius();
	}

	bool intersects(const Asteroid& asteroid, const Asteroid& other) {
		return intersects(asteroid.getBB(),other.getBB());
	}

	bool intersects(const Asteroid& asteroid, const Ship& ship) {
		return intersects(asteroid.getBB(),ship.getBB());
	}

	bool intersects(const Asteroid& asteroid, const sf::RectangleShape& projectile) {
		return intersects(asteroid.getBB(),projectile.getGlobalBounds());
	}

	float distance(sf::Vector2f a, sf::Vector2f b) {
		return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	}

	float distance(Asteroid asteroid, Asteroid other) {
		return distance(asteroid.getPosition(),other.getPosition());
	}

	float length(sf::Vector2f a) {
		return distance(sf::Vector2f(0.0f,0.0f),a);
	}

	float dot(sf::Vector2f a, sf::Vector2f b) {
		return a.x * b.x + a.y * b.y;
	}

	float perProduct(sf::Vector2f a, sf::Vector2f b) {
		return dot(a,rightHandNormal(b));
	}

	sf::Vector2f normalize(sf::Vector2f v) {
		return v / length(v);
	}

	sf::Vector2f project(sf::Vector2f a, sf::Vector2f b) {
		sf::Vector2f projection;
		projection.x = ( dot(a,b) / ( length(b) * length(b) ) ) * b.x;
		projection.y = ( dot(a,b) / ( length(b) * length(b) ) ) * b.y;

		return projection;
	}

	sf::Vector2f leftHandNormal(sf::Vector2f a) {
		return sf::Vector2f(a.y,-a.x);
	}

	sf::Vector2f rightHandNormal(sf::Vector2f a) {
		return -leftHandNormal(a);
	}
}
