/*
 * AsteroidField.cpp
 *
 *  Created on: 20 juil. 2018
 *  Lastly updated : 29 juillet 2018
 *      Author: Clement
 */

#include <iostream>
#include "AsteroidField.hpp"
#include "Utilities.hpp"
#include "Game.hpp"

const unsigned int AsteroidField::capacity = 40;

AsteroidField::AsteroidField() {}

void AsteroidField::update(float delta) {

	cleanField();
	fillField();

	// Update asteroids and QuadTree
	for(auto &asteroid : field)
		asteroid.update(delta);
}

void AsteroidField::cleanField() {

	// Remove out of screen asteroids
	field.erase(std::remove_if(field.begin(),
							   field.end(),
							   [](Asteroid& asteroid) -> bool {
									return !Game::worldRect.contains(asteroid.getPosition());
							   }),
							   field.end());
}

void AsteroidField::fillField() {

	Asteroid asteroid;
	bool overlap = false;
	int attempts = 0,max = 10;

	while((field.empty() || field.size() < AsteroidField::capacity) && attempts < max) {
		asteroid = Asteroid();
		overlap = false;
		attempts++;

		for(auto &other : field) {
			if(asteroid != other && utilities::intersects(asteroid,other)) {
				overlap = true;
				break;
			}
		}

		if(!overlap) {
			field.push_back(asteroid);
			attempts = 0;
		}
	}
}

bool AsteroidField::add(const std::vector<Asteroid>& asteroids) {

	auto it = field.insert(std::end(this->field),std::begin(asteroids),std::end(asteroids));
	return it != this->field.end();
}

bool AsteroidField::erase(const Asteroid& asteroid) {

	auto it = this->field.erase(std::remove(this->field.begin(),this->field.end(),asteroid),this->field.end());
	return it != this->field.end();
}

std::vector<Asteroid>& AsteroidField::getField() {
	return field;
}

std::vector<Asteroid>::iterator AsteroidField::findAsteroid(sf::Vector2f point) {
	return std::find_if(field.begin(),
					 	field.end(),
						[&](const auto& asteroid) -> bool {
							return asteroid.getPosition() == point;
						});
}

void AsteroidField::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	for(auto &asteroid : field)
		target.draw(asteroid);
}

AsteroidField::~AsteroidField() {}
