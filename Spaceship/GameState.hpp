/*
 * GameState.hpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class Game;

class GameState {

public:

	GameState() {}
	virtual ~GameState() {}
	virtual void processEvents(Game& game) {}
	virtual void processCollisions(Game& game) {}
	virtual void update(Game& game, float delta) {}
	virtual void render(Game& game) {}
	virtual void enter(Game& game) {}
	virtual void exit(Game& game) {}
};

#endif
