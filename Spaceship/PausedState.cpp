/*
 * PausedState.cpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#include "GameState.hpp"
#include "PlayingState.hpp"
#include "PausedState.hpp"
#include "Game.hpp"
#include "Utilities.hpp"

void PausedState::processEvents(Game& game) {

	sf::Event event;

	// Window controls
	while (game.window.pollEvent(event)) {
		// Exit controls
		if (event.type == sf::Event::Closed)
			game.window.close();
		if(event.type == sf::Event::KeyPressed)
			if(event.key.code == sf::Keyboard::Escape)
				game.state = &Game::playing;
	}
}
void PausedState::processCollisions(Game& game) {

}
void PausedState::update(Game& game, float delta) {

}
void PausedState::render(Game& game) {

//	sf::Image background = game.window.capture();
//
//	std::uniform_int_distribution<sf::Uint8> d(0,255);
//	sf::Uint8 blurArray[(int)(Game::windowRect.width*Game::windowRect.height*4)];
//
//	for(auto i = 0u; i < Game::windowRect.width*Game::windowRect.height*4; i += 4) {
//		blurArray[i] = blurArray[i+1] = blurArray[i+2] = d(utilities::g);
//		blurArray[i+3] = 15;
//	}
//
//	sf::
//
//	sf::Image blur;
//	sf::Texture texture;
//	sf::Sprite sprite;
//
//	blur.create(Game::windowRect.width,Game::windowRect.height,blurArray);
//	texture.loadFromImage(blur);
//	 sprite.setTexture(texture);

	game.window.clear();
#ifdef DEBUG
	game.window.draw(game.qtree);
#endif
	game.window.draw(game.ship);
	game.window.draw(game.asteroidField);
	//game.window.draw(sprite);
	game.window.display();
}
