/*
 * Game.hpp
 *
 *  Created on: 28 août 2017
 *  Lastly updated : 29 juillet 2018
 *  	Author: Clement
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "SFML/Graphics.hpp"
#include "Ship.hpp"
#include "AsteroidField.hpp"
#include "QuadTree.hpp"
#include "GameState.hpp"
#include "PlayingState.hpp"
#include "PausedState.hpp"
#include "GameOverState.hpp"

class Game {

friend class GameState;
friend class PlayingState;
friend class PausedState;
friend class GameOverState;

	static PlayingState playing;
	static PausedState paused;
	static GameOverState over;

	protected:
		// Window
		sf::ContextSettings settings;
		sf::RenderWindow window;
		sf::View view;

		GameState* state;
		unsigned int score;

		// Game objects
		Ship ship;
		AsteroidField asteroidField;
		QuadTree qtree;

		// Game internal methods
		virtual void processEvents(void);
		virtual void processCollisions(void);
		virtual void update(float delta);
		virtual void render(void);
		void reset(void);

	public:
		// Game parameters

		// Window and display
		static const sf::FloatRect windowRect;
		static const unsigned int windowWidth;
		static const unsigned int windowHeight;
		static const float frameRate;

		// World dimensions
		static const sf::FloatRect worldRect;
		static const float spawnDimensions;
		static const float worldWidth;
		static const float worldHeight;

		// Ctors and dtors
		Game();
		virtual ~Game();

		// Game main method
		void run(void);
};

#endif /* GAME_HPP_ */
