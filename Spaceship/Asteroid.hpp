/*
 * Asteroid.hpp
 *
 *  Created on: 25 août 2017
 *  Lastly updated : 29 juillet 2018
 *      Author: Clement
 */

#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <SFML/Graphics.hpp>
#include "Ship.hpp"

class Asteroid : public sf::Drawable, sf::Transformable {

private:

	sf::ConvexShape body;
	sf::CircleShape bb;
	sf::Vector2f velocity;
	float steering;

	void computeBoundingBox();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setLook();
	void randomShape();
	void randomPosition();
	void randomVelocity();
	void randomSteering();
	void randomRadius();
	float randomAngle(float min,float max);

public:

	static const unsigned int minVertexes;
	static const unsigned int maxVertexes;
	static const float minRadius;
	static const float maxRadius;
	static const float maxVelocity;
	static const float maxSteering;

	Asteroid();
	Asteroid(sf::Vector2f origin, float radius);
	virtual ~Asteroid();

	void update(float delta);
	void bounce(const Asteroid& other);
	void bounce(const Ship& ship);
	std::vector<Asteroid>shatter();

	const sf::CircleShape& getBB() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getVelocity() const;
	float getRadius() const;

	void setVelocity(const sf::Vector2f& velocity);
	void setColor(const sf::Color& color);
	void setPosition(const sf::Vector2f& position);

	bool operator==(const Asteroid& a) const { return this->getPosition() == a.getPosition(); };
	bool operator!=(const Asteroid& a) const { return !Asteroid::operator==(a); };
};

#endif /* ASTEROID_HPP_ */
