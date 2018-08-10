/*
 * GameOverState.hpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "GameState.hpp"

// Game over
class GameOverState : public GameState {

public:

	GameOverState() {}

	virtual void processEvents(Game& game);
	virtual void processCollisions(Game& game);
	virtual void update(Game& game, float delta);
	virtual void render(Game& game);
	virtual void enter(Game& game);
	virtual void exit(Game & game);
};

#endif
