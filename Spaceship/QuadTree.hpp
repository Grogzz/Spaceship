/*
 * QuadTree.hpp
 *
 *  Created on: 24 juil. 2018
 *      Author: cleme
 */

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <SFML/Graphics.hpp>
#include "Asteroid.hpp"

class QuadTree : public sf::Drawable, sf::Transformable {

public:
	enum class Object { Ship, Asteroid, Projectile };

private:
	sf::FloatRect boundary;
	std::vector<std::pair<sf::Vector2f,QuadTree::Object>> points;
	static const int capacity;

	QuadTree *northWest;
	QuadTree *northEast;
	QuadTree *southWest;
	QuadTree *southEast;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	QuadTree();
	QuadTree(sf::FloatRect boundary);
	virtual ~QuadTree();

	bool insert(sf::Vector2f point, QuadTree::Object object);
	void subdivide();
	std::vector<std::pair<sf::Vector2f,QuadTree::Object>>  query(sf::CircleShape circle);
	std::vector<std::pair<sf::Vector2f,QuadTree::Object>>& getPoints();
};

#endif /* QUADTREE_HPP_ */
