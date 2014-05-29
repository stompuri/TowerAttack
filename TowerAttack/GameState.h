// Virtual parent class for tiles of game board

#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class GameState
	{
	public:
		virtual void Init() = 0;
		virtual void Cleanup() = 0;

		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual void HandleEvents(sf::RenderWindow& window) = 0;
		virtual void Update() = 0;
		virtual void Draw(sf::RenderWindow& window) = 0;

		void ChangeState(GameState* state) { Game::ChangeState(state); }

		~GameState() { }

	protected:
		GameState() { } // Virtual class, constructor private
	};
}