#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class StatePause : public virtual TA::GameState
	{
	public:
		void Init();
		void Cleanup();

		void Pause();
		void Resume();

		void HandleEvents(sf::RenderWindow& window);
		void Update();
		void Draw(sf::RenderWindow& window);

		static StatePause* Instance() {
			return &m_PauseState;
		}


	protected:
		StatePause() { }

	private:
		static StatePause m_PauseState;
	};
}