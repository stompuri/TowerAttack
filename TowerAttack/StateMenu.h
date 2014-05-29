#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML
#include <list> // Listing menu items

namespace TA
{
	class StateMenu : public virtual TA::GameState
	{
	public:
		void Init();
		void Cleanup();

		void Pause();
		void Resume();

		void HandleEvents(sf::RenderWindow& window);
		void Update();
		void Draw(sf::RenderWindow& window);

		static StateMenu* Instance() {
			return &m_MenuState;
		}

		enum MenuResult { Nothing, Exit, Play }; // Different actions when clicking around main menu
		struct MenuItem
		{
		public:
			sf::Rect<int> rect; // Area for the main menu item
			MenuResult action; // Action to be taken when clicked this menu item
		};

	protected:
		StateMenu() { }

	private:
		static StateMenu m_MenuState;
		static sf::Sprite * p_sprite;
		static sf::Texture image;
		static MenuResult action;

		static MenuResult HandleClick(int x, int y); // Check what kind of action should be activated when user clicks somewhere
		static std::list<MenuItem> menuItems; // List the different menu items
	};
}