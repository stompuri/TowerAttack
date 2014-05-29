#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class StatePlay : public virtual GameState
	{
	public:
		void Init();
		void Cleanup();

		void Pause();
		void Resume();

		void HandleEvents(sf::RenderWindow& window);
		void Update();
		void Draw(sf::RenderWindow& window);

		static StatePlay* Instance() {
			return &m_PlayState;
		}

		void Load(sf::Sprite _sprite, sf::Texture _image, std::string _filename); // Method to load texture from a file
		
		static void DrawHUD(sf::RenderWindow& window);

	protected:
		StatePlay() { }

	private:
		static StatePlay m_PlayState;
		static bool paused;
		static sf::Vector2f viewPosition;

		enum ClickResult { Nothing, Attack, AddSimple, AddFast, AddHeavy, DeleteItem, UpgradeItem }; // Different actions when clicking around main menu
		struct ClickItem
		{
		public:
			sf::Rect<int> rect; // Area for the main menu item
			std::shared_ptr<Character> clickedItem;
			ClickResult action; // Action to be taken when clicked this menu item
		};
		static ClickResult HandleClick(int x, int y); // Check what kind of action should be activated when user clicks somewhere
		static std::list<ClickItem> clickItems; // List the different menu items
		static ClickResult action;

		static std::shared_ptr<Character> AddUnit(std::string _type);
		static void UpdatePlacement(unsigned int _placement);
		static std::shared_ptr<Character> ptr_removeCharacter; // Pointing to a character that we want to remove
		static ClickItem dynamicClickRegion;

		static void AddDynamicClickRegion(std::shared_ptr<Character> _ptr_char);
		static void PostPlanActions(); // Do actions after player pressed "Attack" button
	};
}