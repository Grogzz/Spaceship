/*
 * Game.cpp
 *
 *  Created on: 28 août 2017
 *  Lastly updated : 29 juillet 2018
 *      Author: Clement
 */

#include <cmath>
#include "Game.hpp"
#include "GameParameters.hpp"
#include "Utilities.hpp"
#include "GameState.hpp"

// Window
const unsigned int Game::windowWidth(800);
const unsigned int Game::windowHeight(600);
const sf::FloatRect Game::windowRect(Game::spawnDimensions,Game::spawnDimensions,Game::windowWidth,Game::windowHeight);
const float Game::frameRate(60.0f);

// World dimensions
const float Game::spawnDimensions(200.0f);
const float Game::worldWidth(Game::windowWidth + 2 * Game::spawnDimensions);
const float Game::worldHeight(Game::windowHeight + 2 * Game::spawnDimensions);
const sf::FloatRect Game::worldRect(0.0f,0.0f,Game::worldWidth,Game::worldHeight);

// States
PlayingState Game::playing;
PausedState Game::paused;
GameOverState Game::over;

bool isEqual(const sf::RectangleShape& a, const sf::RectangleShape& b) { return a.getSize() == b.getSize() && a.getPosition() == b.getPosition(); }


// Ctors and dtors
Game::Game():
			settings(0,0,8,2,0),
			window(sf::VideoMode(Game::windowWidth,Game::windowHeight),"Spaceship",sf::Style::Default,settings),
#ifdef DEBUG
			view(Game::worldRect),
#else
			view(Game::windowRect),
#endif
			score(0),
			ship(),
			asteroidField(),
			qtree(QuadTree(Game::worldRect))
{
	state = &Game::playing;
	window.setView(view);
}
Game::~Game() {}

// Game main method
void Game::run(void) {

	sf::Clock clock;
	float timeElapsedFrame = 0.0f;
	float timeSinceLastUpdate = 0.0f;
	float timePerFrame = 1.0f/Game::frameRate;

	while (window.isOpen()) {
		timeElapsedFrame = clock.restart().asSeconds();
		timeSinceLastUpdate += timeElapsedFrame;

		while(timeSinceLastUpdate > timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			timeElapsedFrame = clock.restart().asSeconds();
		}

		processEvents();
		processCollisions();
		update(timeElapsedFrame);
		render();
	}
}

// Game internal methods
void Game::processEvents(void) {
	state->processEvents(*this);
}
void Game::processCollisions(void) {
	state->processCollisions(*this);
}
void Game::update(float delta) {
	state->update(*this,delta);
}
void Game::render(void) {
	state->render(*this);
}
void Game::reset() {
	ship = Ship();
	asteroidField = AsteroidField();
	qtree = QuadTree();
	score = 0;
}
