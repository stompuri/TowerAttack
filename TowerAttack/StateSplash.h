#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class StateSplash : public virtual TA::GameState
	{
	public:
		void Init();
		void Cleanup();

		void Pause();
		void Resume();

		void HandleEvents(sf::RenderWindow& window);
		void Update();
		void Draw(sf::RenderWindow& window);

		static StateSplash* Instance() {
			return &m_SplashState;
		}

	protected:
		StateSplash() { }

	private:
		static StateSplash m_SplashState;
		static const bool debug = false;
	};
}