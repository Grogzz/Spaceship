/*
 * PlayingState.hpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#ifndef PLAYINGSTATE_HPP
#define PLAYINGSTATE_HPP


// Playing
class PlayingState : public GameState {

public:

	PlayingState() {}

	virtual void processEvents(Game& game);
	virtual void processCollisions(Game& game);
	virtual void update(Game& game, float delta);
	virtual void render(Game& game);
};

#endif
