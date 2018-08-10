/*
 * GameOverState.cpp
 *
 *  Created on: 9 août 2018
 *      Author: cleme
 */

#include "GameOverState.hpp"
#include "Game.hpp"

void GameOverState::processEvents(Game& game) {
	sf::Event event;

	// Window controls
	while (game.window.pollEvent(event)) {
		// Exit controls
		if (event.type == sf::Event::Closed)
			game.window.close();
		if(event.type == sf::Event::KeyPressed) {
			if(event.key.code == sf::Keyboard::Space) {
				game.state->exit(game);
				game.state = &Game::playing;
			}
		}
	}
}
void GameOverState::processCollisions(Game& game) {

}
void GameOverState::update(Game& game, float delta) {

}
void GameOverState::render(Game& game) {

	sf::Font font;
	sf::Text gameOver, pressRestart;

	if(font.loadFromFile("rsc/Fonts/comic.ttf")) {
		gameOver.setFont(font);
		gameOver.setString("Game Over !");
		gameOver.setCharacterSize(72);
		gameOver.setFillColor(sf::Color::Cyan);
		gameOver.setPosition(sf::Vector2f(Game::worldRect.width/2.0f,Game::worldRect.height/2.0f));
		gameOver.setOrigin(sf::Vector2f(gameOver.getGlobalBounds().width/2.0f,gameOver.getGlobalBounds().height/2.0f));

		pressRestart.setFont(font);
		pressRestart.setString("Press \"Spacebar\" to restart...");
		pressRestart.setCharacterSize(48);
		pressRestart.setFillColor(sf::Color::Cyan);
		pressRestart.setPosition(sf::Vector2f(Game::worldRect.width/2.0f,Game::worldRect.height - (Game::worldRect.height/3.0f)));
		pressRestart.setOrigin(sf::Vector2f(pressRestart.getGlobalBounds().width/2.0f,pressRestart.getGlobalBounds().height/2.0f));
	}

	game.window.clear();
#ifdef DEBUG
	game.window.draw(game.qtree);
#endif
	game.window.draw(game.ship);
	game.window.draw(game.asteroidField);
	game.window.draw(gameOver);
	game.window.draw(pressRestart);
	game.window.display();
}
void GameOverState::enter(Game& game) {

}
void GameOverState::exit(Game& game) {
	game.reset();
}
