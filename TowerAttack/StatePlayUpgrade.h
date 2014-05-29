#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class StatePlayUpgrade : public virtual TA::GameState
	{
	public:
		void Init();
		void Cleanup();

		void Pause();
		void Resume();

		void HandleEvents(sf::RenderWindow& window);
		void Update();
		void Draw(sf::RenderWindow& window);

		static StatePlayUpgrade* Instance() {
			return &m_PlayStateUpgrade;
		}

		void Load(sf::Sprite _sprite, sf::Texture _image, std::string _filename); // Method to load texture from a file

	protected:
		StatePlayUpgrade() { }

	private:
		static StatePlayUpgrade m_PlayStateUpgrade;
		static sf::Vector2f viewPosition;

		enum ClickResult { Nothing, Close, SimpleSpeed, SimpleArmor, SimpleShield, FastSpeed, FastArmor, FastShield, HeavySpeed, HeavyArmor, HeavyShield }; // Different actions when clicking around window

		struct ClickItem
		{
		public:
			sf::Rect<int> rect; // Area for the main menu item
			ClickResult action; // Action to be taken when clicked this menu item
			int cost; // Cost of an upgrade action
			int level; // Upgrade level this click rec is representing
		};
		static ClickItem HandleClick(int x, int y); // Check what kind of action should be activated when user clicks somewhere
		static std::list<ClickItem> clickItems; // List the different menu items
		static ClickResult action;

		void UpgradeAttribute(std::string _unitType, std::string _attributeType, ClickItem _item);
	};
}