/*
 * Ship.hpp
 *
 *  Created on: 24 août 2017
 *      Author: Clement
 */

#ifndef SHIP_HPP
#define SHIP_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>
#include <algorithm>

class Ship : public sf::Drawable, public sf::Transformable {

private:

	// Ship
	static const float maxVelocity;
	static const float accelerationPower;
	static const float steeringPower;
	static const float fireRate;
	static const float projectileVelocity;
	static const float shootDelay;

	sf::Clock shootClock;
	float timeElapsedShoot;
	float lastShoot;

	sf::ConvexShape hull;
	sf::RectangleShape bb;
	std::vector<sf::RectangleShape> projectiles;
	float velocity;
	float steering;
	float acceleration;

	void computeBoundingBox();
	void warp();
	void setLook();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:

	// Ctors and dtors
	Ship();
	virtual ~Ship();

	//
	void move(int direction);
	void turn(int direction);
	void shoot(void);
	void update(float delta);

	float getSteering() const;
	float getVelocity() const;
	const sf::RectangleShape& getBB() const;
	std::vector<sf::RectangleShape>& getProjectiles();
	std::vector<sf::RectangleShape>::iterator findProjectile(sf::Vector2f);
	bool eraseProjectile(const sf::RectangleShape& projectile);
};

#endif /* SHIP_HPP_ */
