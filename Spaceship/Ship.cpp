/*
 * Ship.cpp
 *
 *  Created on: 24 août 2017
 *      Author: Clement
 */

#include <iostream>
#include <cmath>

#include "Ship.hpp"
#include "Game.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"


// Ship constants
const float Ship::maxVelocity = 350.0f;
const float Ship::accelerationPower = 0.1f;
const float Ship::steeringPower = 100.0f;
const float Ship::projectileVelocity = 500.0f;
const float Ship::fireRate= 3.0f;
const float Ship::shootDelay = 1.0f/Ship::fireRate;


// Ctors and dtors
Ship::Ship() : velocity(0.0f), steering(0.0f), acceleration(0.0f) {

	// Ship default look
	setLook();

	computeBoundingBox();

	// Position
	this->setOrigin(sf::Vector2f(bb.getGlobalBounds().width/2.0f,bb.getGlobalBounds().height/2.0f));
	this->setPosition(sf::Vector2f(Game::windowRect.left + Game::windowWidth/2.0f,Game::windowRect.top + Game::windowHeight/2.0f));
}

Ship::~Ship() {}


void Ship::setLook() {

	// Shape
	hull.setPointCount(4);
	hull.setPoint(0, sf::Vector2f(0.0f, 0.0f));
	hull.setPoint(1, sf::Vector2f(100.0f, 40.0f));
	hull.setPoint(2, sf::Vector2f(0.0f, 80.0f));
	hull.setPoint(3, sf::Vector2f(20.0f, 40.0f));

	// Look
	rotate(-90.0f);
	hull.scale(sf::Vector2f(0.5f,0.5f));
#ifdef DEBUG
	hull.setFillColor(sf::Color::Transparent);
	hull.setOutlineColor(sf::Color(180,155,0));
	hull.setOutlineThickness(2.0f);
	// Bounding box look
	bb.setFillColor(sf::Color::Transparent);
	bb.setOutlineColor(sf::Color::Red);
	bb.setOutlineThickness(1.0f);
#else
	hull.setFillColor(sf::Color(180,155,0));
#endif
}

// Class main methods
void Ship::turn(int direction) {

	steering = Ship::steeringPower * direction;
}

void Ship::move(int direction) {

	velocity += Ship::accelerationPower * direction;

	if(velocity < 0.0f)
		velocity = 0.0f;
	else if(velocity > Ship::maxVelocity)
		velocity = Ship::maxVelocity;
}

void Ship::shoot(void) {

	// Look
	sf::RectangleShape shell(sf::Vector2f(15.0f,3.0f));
	shell.setFillColor(sf::Color(155,0,0));

	// Position
	shell.setOrigin(sf::Vector2f(-this->getOrigin().x/2.0f,0.0f));
	shell.setPosition(this->getPosition());
	shell.setRotation(this->getRotation());

	// Time handling
	timeElapsedShoot = shootClock.restart().asSeconds();
	lastShoot += timeElapsedShoot;

	if(lastShoot > shootDelay) {
		projectiles.push_back(shell);
		lastShoot = 0.0f;
		timeElapsedShoot = shootClock.restart().asSeconds();
	}
}

void Ship::warp() {

	if(this->getPosition().x + bb.getGlobalBounds().left + bb.getGlobalBounds().width < Game::windowRect.left)
		this->setPosition(sf::Vector2f(Game::windowRect.left + Game::windowRect.width, this->getPosition().y));
	else if(this->getPosition().x + bb.getGlobalBounds().left > Game::windowRect.left + Game::windowRect.width)
		this->setPosition(sf::Vector2f(Game::windowRect.left,this->getPosition().y));
	if(this->getPosition().y + bb.getGlobalBounds().top + bb.getGlobalBounds().height < Game::windowRect.top)
		this->setPosition(sf::Vector2f(this->getPosition().x,Game::windowRect.top + Game::windowRect.height));
	else if(this->getPosition().y + bb.getGlobalBounds().top > Game::windowRect.top + Game::windowRect.height)
		this->setPosition(sf::Vector2f(this->getPosition().x,Game::windowRect.top));
}

// Class local method
void Ship::update(float delta) {

	// Ship
	sf::Vector2f hullVelocity = utilities::direction(this->getRotation()) * velocity * delta;
	this->rotate(steering * delta);
	this->setPosition(sf::Vector2f(this->getPosition() + hullVelocity));

	steering = 0.0f;

	// Ship warp
	warp();

	// Ship bounding box
	computeBoundingBox();

	// Projectiles
	for(auto &projectile : projectiles) {
		sf::Vector2f projectileVelocity = utilities::direction(projectile.getRotation()) * Ship::projectileVelocity * delta;
		projectile.move(projectileVelocity);
	}

	projectiles.erase(std::remove_if(projectiles.begin(),projectiles.end(),
									 [](sf::RectangleShape projectile) -> bool {
										return !Game::worldRect.contains(projectile.getPosition());
									 }),
									 projectiles.end());

	lastShoot += timeElapsedShoot;
}

void Ship::computeBoundingBox() {

	sf::Vector2f topLeft,bottomRight;
	sf::Vector2f vertex;

	// Computation
	topLeft = bottomRight = hull.getTransform().transformPoint(hull.getPoint(0));

	for(auto i = 0u; i < hull.getPointCount(); i++) {
		vertex = hull.getTransform().transformPoint(hull.getPoint(i));
		if(vertex.x < topLeft.x)
			topLeft.x = vertex.x;
		else if(vertex.x > bottomRight.x)
			bottomRight.x = vertex.x;
		if(vertex.y < topLeft.y)
			topLeft.y = vertex.y;
		else if(vertex.y > bottomRight.y)
			bottomRight.y = vertex.y;
	}

	// Shaping
	bb.setSize(bottomRight - topLeft);
}

void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();

#ifdef DEBUG
	target.draw(bb,states);
#endif
	target.draw(hull, states);

	states.transform = states.transform.Identity;
	for(auto &projectile : projectiles)
		target.draw(projectile, states);
}

float Ship::getSteering() const {
	return steering;
}

float Ship::getVelocity() const {
	return velocity;
}

std::vector<sf::RectangleShape>& Ship::getProjectiles() {
	return projectiles;
}

std::vector<sf::RectangleShape>::iterator Ship::findProjectile(sf::Vector2f point) {
	return std::find_if(projectiles.begin(),
						projectiles.end(),
						[&](const auto& projectile) -> bool {
							return projectile.getPosition() == point;
						});
}

bool Ship::eraseProjectile(const sf::RectangleShape& projectile) {
	auto it = this->projectiles.erase(std::remove_if(this->projectiles.begin(),
													 this->projectiles.end(),
													 [&projectile](const sf::RectangleShape& other) -> bool {
			  	  	  	  	  	  	  	  	  	  	 	 return projectile.getPosition() == other.getPosition();
		 	  	  	  	  	  	  	  	  	  	  	 }),
													 this->projectiles.end());

	return it != this->projectiles.end();
}
const sf::RectangleShape& Ship::getBB() const {
	return this->bb;
}
