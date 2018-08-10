/*
 * Asteroid.cpp
 *
 *  Created on: 25 août 2017
 *      Author: Clement
 */

#include <iostream>
#include <cmath>

#include "Asteroid.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"
#include "Game.hpp"

const unsigned int Asteroid::minVertexes(8);
const unsigned int Asteroid::maxVertexes(12);
const float Asteroid::minRadius(30.0f);
const float Asteroid::maxRadius(40.0f);
const float Asteroid::maxVelocity(100.0f);
const float Asteroid::maxSteering(5.0f);

Asteroid::Asteroid() {

	// Asteroid default look
	setLook();

	// Asteroid random shape
	randomRadius();
	randomShape();
	randomPosition();
	randomVelocity();
	randomSteering();
}

Asteroid::Asteroid(sf::Vector2f origin, float radius) : Asteroid() {

	// Asteroid shape
	body.setPosition(origin);
	bb.setPosition(origin);
	bb.setRadius(radius);
	bb.setOrigin(bb.getRadius(),bb.getRadius());

	randomShape();
	randomVelocity();
	randomSteering();
}

void Asteroid::setLook() {

#ifdef DEBUG
	// Asteroid look
	// Green color 	: (0,155,80)
	// Grey color 	: (155,155,155)
	body.setFillColor(sf::Color::Transparent);
	body.setOutlineColor(sf::Color(0,155,80));
	body.setOutlineThickness(2.0f);

	// Bounding box look
	bb.setFillColor(sf::Color::Transparent);
	bb.setOutlineColor(sf::Color::Red);
	bb.setOutlineThickness(1.0f);
#else
	body.setFillColor(sf::Color(155,155,155));
	body.setOutlineColor(sf::Color(155,155,155));
#endif
}

void Asteroid::randomShape() {

	std::uniform_int_distribution<unsigned int>d(Asteroid::minVertexes,Asteroid::maxVertexes);
	unsigned int points = 1;
	float angle, step, min, max;

	points =  d(utilities::g);
	step = 2.0f * constants::pi / (float)points;
	min = 0.0f;
	max = step;

	body.setPointCount(points);

	for(auto i = 0u; i < body.getPointCount(); i++) {
		angle = randomAngle(min,max);
		float x = cos(angle) * bb.getRadius();
		float y = sin(angle) * bb.getRadius();
		body.setPoint(i, sf::Vector2f(x,y));
		min += step;
		max += step;
	}
}

void Asteroid::randomPosition() {

	std::uniform_real_distribution<float>dx(0.0f,Game::worldWidth);
	std::uniform_real_distribution<float>dy(0.0f,Game::worldHeight);
	sf::Vector2f position;

	do {
		position = sf::Vector2f(dx(utilities::g),dy(utilities::g));
		body.setPosition(position);
		bb.setPosition(body.getPosition());
	} while(utilities::intersects(this->getBB(),Game::windowRect));
}

void Asteroid::randomVelocity() {

	float vx, vy;
	std::uniform_real_distribution<float> dx;
	std::uniform_real_distribution<float> dy;

	if(body.getPosition().x < Game::windowRect.left)
		dx = std::uniform_real_distribution<float>(0.0f,Asteroid::maxVelocity);
	else
		dx = std::uniform_real_distribution<float>(-Asteroid::maxVelocity,0.0f);

	if(body.getPosition().y < Game::windowRect.top)
		dy = std::uniform_real_distribution<float>(0.0f,Asteroid::maxVelocity);
	else
		dy = std::uniform_real_distribution<float>(-Asteroid::maxVelocity,0.0f);

	vx = dx(utilities::g);
	vy = dy(utilities::g);

	velocity = sf::Vector2f(vx,vy);
}

void Asteroid::randomSteering() {

	std::uniform_real_distribution<float>d(0.0f,Asteroid::maxSteering);
	steering = d(utilities::g);
}

void Asteroid::randomRadius() {

	std::uniform_real_distribution<float>d(Asteroid::minRadius,Asteroid::maxRadius);
	float radius = d(utilities::g);
	bb.setRadius(radius);
	bb.setOrigin(bb.getRadius(),bb.getRadius());
}

float Asteroid::randomAngle(float min, float max) {

	std::uniform_real_distribution<float>d(min,max);
	return d(utilities::g);
}

const sf::Vector2f& Asteroid::getPosition() const {
	return this->getBB().getPosition();
}

void Asteroid::update(float delta) {

	body.move(velocity * delta);
	bb.move(velocity * delta);
	body.rotate(steering * delta);
}

void Asteroid::bounce(const Asteroid& other) {

	float o1o2 = utilities::distance(this->getPosition(),other.getPosition());
	float r1r2 = this->getRadius() + other.getRadius();
	float ratio = 1 - (o1o2 / r1r2);
	sf::Vector2f normal = -utilities::normalize(sf::Vector2f(this->getPosition().x - other.getPosition().x, this->getPosition().y - other.getPosition().y));
	sf::Vector2f newVelocity(-2.0f * utilities::project(this->getVelocity(),normal) + this->getVelocity());

	// Push the asteroid out of the other
	this->setPosition(this->getPosition() - (normal * ratio));
	// Bounce
	this->setVelocity(newVelocity);
}

void Asteroid::bounce(const Ship& ship) {
	sf::Vector2f nearest = utilities::nearest(*this,ship);
	sf::Vector2f normal = utilities::normalize(sf::Vector2f(this->getPosition().x - nearest.x, this->getPosition().y - nearest.y));
	sf::Vector2f newVelocity(-2.0f * utilities::project(this->getVelocity(),normal) + this->getVelocity());
	this->setVelocity(newVelocity);
}

std::vector<Asteroid> Asteroid::shatter() {

	std::vector<Asteroid> asteroids;

	if(this->getRadius() < Asteroid::minRadius)
		return asteroids;

	sf::Vector2f o,p1,p2,p3;
	float A,B,C,D,E,F,G,r;

	p3 = this->getPosition();

	auto pieces = this->body.getPointCount() / 2u;

//	std::cout << "Point count : " << this->body.getPointCount() << std::endl;
//	std::cout << "Pieces : " << pieces << std::endl;

	for(auto i = 0u; i < pieces; i++) {
		p1 = this->body.getTransform().transformPoint(this->body.getPoint(i*2));
		p2 = this->body.getTransform().transformPoint(this->body.getPoint(i*2+1));

		A = p2.x - p1.x;
		B = p2.y - p1.y;
		C = p3.x - p1.x;
		D = p3.y - p1.y;

		E = A * (p1.x + p2.x) + B * (p1.y + p2.y);
		F = C * (p1.x + p3.x) + D * (p1.y + p3.y);

		G = 2.0 * (A * (p3.y - p2.y) - B * (p3.x - p2.x));

		if(G == 0)
			return asteroids;

		o.x = (D * E - B * F) / G;
		o.y = (A * F - C * E) / G;
		r = std::sqrt((p1.x - o.x) * (p1.x - o.x) + (p1.y - o.y) * (p1.y - o.y));

		asteroids.push_back(Asteroid(o,r));
	}

	return asteroids;
}

void Asteroid::computeBoundingBox() {
	float radius = getRadius();
	bb.setRadius(radius);
	bb.setPosition(body.getPosition());
	bb.setOrigin(sf::Vector2f(radius,radius));
}

void Asteroid::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();
#ifdef DEBUG
	target.draw(bb,states);
//	sf::Vertex o(bb.getPosition(), sf::Color::Red);
//	target.draw(&o,1,sf::Points);
#endif
	target.draw(body, states);

}

float Asteroid::getRadius() const {
	return this->getBB().getRadius();
}

const sf::Vector2f& Asteroid::getVelocity() const {
	return this->velocity;
}

void Asteroid::setVelocity(const sf::Vector2f& velocity) {
	this->velocity = velocity;
}

void Asteroid::setColor(const sf::Color& color) {
	this->body.setFillColor(color);
	this->body.setOutlineColor(color);
}

void Asteroid::setPosition(const sf::Vector2f& position) {
	this->body.setPosition(position);
	this->bb.setPosition(this->body.getPosition());
}

Asteroid::~Asteroid() {}

const sf::CircleShape& Asteroid::getBB() const {
	return this->bb;
}
