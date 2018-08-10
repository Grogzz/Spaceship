/*
 * PlayingState.cpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#include "GameState.hpp"
#include "PlayingState.hpp"
#include "PausedState.hpp"
#include "Game.hpp"
#include "Utilities.hpp"

void PlayingState::processEvents(Game& game) {

	sf::Event event;

	// Window controls
	while (game.window.pollEvent(event)) {
		// Exit controls
		if (event.type == sf::Event::Closed)
			game.window.close();
		if(event.type == sf::Event::KeyPressed)
			if(event.key.code == sf::Keyboard::Escape)
				game.state = &Game::paused;
	}

	// Ship controls
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		game.ship.turn(-1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		game.ship.turn(1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		game.ship.move(1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		game.ship.move(-1);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		game.ship.shoot();
}
void PlayingState::processCollisions(Game& game) {

	sf::CircleShape range;
	for(auto &asteroid : game.asteroidField.getField()) {
		range = asteroid.getBB();
		range.setRadius(range.getRadius() * 2.0f);
		auto points = game.qtree.query(range);
		for(auto &point : points) {
			if(point.second == QuadTree::Object::Asteroid) {
				auto other = game.asteroidField.findAsteroid(point.first);
				if(asteroid != (*other) && utilities::intersects(asteroid,(*other))) {
					asteroid.bounce(*other);
					other->bounce(asteroid);
				}
			}
			else if(point.second == QuadTree::Object::Projectile) {
				auto projectile = game.ship.findProjectile(point.first);
				if(utilities::intersects(asteroid,(*projectile))) {
					game.asteroidField.add(asteroid.shatter());
					game.asteroidField.erase(asteroid);
					game.ship.eraseProjectile((*projectile));
				}
			}
			else if(point.second == QuadTree::Object::Ship) {
				game.asteroidField.add(asteroid.shatter());
				game.asteroidField.erase(asteroid);
				game.state = &Game::over;
				game.state->enter(game);
			}
		}
	}
}
void PlayingState::update(Game& game, float delta) {

	game.ship.update(delta);
	game.asteroidField.update(delta);

	// Reset QuadTree
	game.qtree = QuadTree(Game::worldRect);

	// Insert ship, projectiles and asteroids
	game.qtree.insert(game.ship.getPosition(),QuadTree::Object::Ship);
	for(auto &projectile : game.ship.getProjectiles())
		game.qtree.insert(projectile.getPosition(),QuadTree::Object::Projectile);
	for(auto &asteroid : game.asteroidField.getField())
		game.qtree.insert(asteroid.getPosition(),QuadTree::Object::Asteroid);
}
void PlayingState::render(Game& game) {

	game.window.clear();
#ifdef DEBUG
	game.window.draw(game.qtree);
#endif
	game.window.draw(game.ship);
	game.window.draw(game.asteroidField);
	game.window.display();
}
