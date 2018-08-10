/*
 * QuadTree.cpp
 *
 *  Created on: 24 juil. 2018
 *  Lastly updated : 29 juillet 2018
 *      Author: Clement
 */

#include <iostream>
#include <algorithm>
#include "Game.hpp"
#include "Utilities.hpp"
#include "QuadTree.hpp"

const int QuadTree::capacity = 4;

QuadTree::QuadTree() {

	this->boundary = Game::worldRect;
	this->northWest = nullptr;
	this->northEast = nullptr;
	this->southWest = nullptr;
	this->southEast = nullptr;
}

QuadTree::QuadTree(sf::FloatRect boundary) {

	this->boundary = boundary;
	this->northWest = nullptr;
	this->northEast = nullptr;
	this->southWest = nullptr;
	this->southEast = nullptr;
}

bool QuadTree::insert(sf::Vector2f point, Object object) {

	if(!boundary.contains(point))
		return false;

	if(points.size() < capacity) {
		points.push_back(std::make_pair(point,object));
		return true;
	}

	if(northWest == nullptr)
		subdivide();

	if(northWest->insert(point,object)
	|| northEast->insert(point,object)
	|| southWest->insert(point,object)
	|| southEast->insert(point,object))
		return true;

	std::cout << "Something is wrong" << std::endl;
	std::cout << "Boundary(" << boundary.left << "," << boundary.top << "," << boundary.width << "," << boundary.height << ")" << std::endl;
	return false;
}

void QuadTree::subdivide(void) {

	unsigned int x = this->boundary.left;
	unsigned int y = this->boundary.top;
	unsigned int w = this->boundary.width / 2;
	unsigned int h = this->boundary.height / 2;

	northWest = new QuadTree(sf::FloatRect(x, y, w, h));
	northEast = new QuadTree(sf::FloatRect(x + w, y, w, h));
	southWest = new QuadTree(sf::FloatRect(x , y + h, w, h));
	southEast = new QuadTree(sf::FloatRect(x + w, y + h, w, h));
}

std::vector<std::pair<sf::Vector2f,QuadTree::Object>> QuadTree::query(sf::CircleShape circle) {

	std::vector<std::pair<sf::Vector2f,QuadTree::Object>> found;

	if(!utilities::intersects(circle,boundary))
		return found;

	for(auto &point : points)
		if(utilities::contains(circle,point.first))
			found.push_back(point);

	if(northWest != nullptr) {
		auto others = northWest->query(circle);
		found.insert(std::end(found),std::begin(others),std::end(others));
		others = northEast->query(circle);
		found.insert(std::end(found),std::begin(others),std::end(others));
		others = southWest->query(circle);
		found.insert(std::end(found),std::begin(others),std::end(others));
		others = southEast->query(circle);
		found.insert(std::end(found),std::begin(others),std::end(others));
	}

	return found;
}

void QuadTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform *= getTransform();

	// Asteroid bounding box
	sf::RectangleShape bb(sf::Vector2f(boundary.width,boundary.height));
	bb.setPosition(sf::Vector2f(boundary.left,boundary.top));
	bb.setFillColor(sf::Color::Transparent);
	bb.setOutlineThickness(1.0f);
	bb.setOutlineColor(sf::Color::White);

	sf::VertexArray vertexes(sf::Points,points.size());

	for(unsigned int i = 0; i < vertexes.getVertexCount(); i++) {
		vertexes[i].position = points[i].first;
		vertexes[i].color = sf::Color::Red;
	}

	target.draw(bb);
	target.draw(vertexes);

	if(northWest != nullptr) {
		target.draw(*northWest);
		target.draw(*northEast);
		target.draw(*southWest);
		target.draw(*southEast);
	}
}

std::vector<std::pair<sf::Vector2f,QuadTree::Object>>& QuadTree::getPoints() {
	return points;
}

QuadTree::~QuadTree() {

	free(northWest);
	free(northEast);
	free(southWest);
	free(southEast);
}

