/*
 * PausedState.hpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#ifndef PAUSEDSTATE_HPP
#define PAUSEDSTATE_HPP

#include "GameState.hpp"

// Paused
class PausedState : public GameState {

public:

	PausedState() {}

	virtual void processEvents(Game& game);
	virtual void processCollisions(Game& game);
	virtual void update(Game& game, float delta);
	virtual void render(Game& game);
};

#endif
