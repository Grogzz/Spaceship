/*
 * AsteroidField.hpp
 *
 *  Created on: 20 juil. 2018
 *  Lastly updated : 29 juillet 2018
 *      Author: Clement
 */

#ifndef ASTEROIDFIELD_HPP
#define ASTEROIDFIELD_HPP

#include <SFML/Graphics.hpp>
#include "Asteroid.hpp"
#include "QuadTree.hpp"

class AsteroidField : public sf::Drawable, sf::Transformable {

private:

	static const unsigned int capacity;

	std::vector<Asteroid> field;

	void cleanField();
	void fillField();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	AsteroidField();
	virtual ~AsteroidField();

	void update(float delta);
	bool add(const std::vector<Asteroid>& asteroids);
	bool erase(const Asteroid& asteroid);

	std::vector<Asteroid>::iterator findAsteroid(sf::Vector2f);
	std::vector<Asteroid>& getField();
};

#endif /* ASTEROIDFIELD_HPP */
