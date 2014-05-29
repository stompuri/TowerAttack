#pragma once

#include "GameState.h"
#include "Game.h"
#include <SFML/System.hpp> // Using SFML stuff
#include <SFML/Graphics.hpp> // Drawing stuff with SFML

namespace TA
{
	class Tower;
	class Turret;

	class Drawer
	{
	public:
		// Game is a singleton class
		~Drawer() { instanceFlag = false; }
		static Drawer* GetInstance(); // Return a handle to the active Game object

		static void Init();
		static void Cleanup();

		static void AddPlacement(); // Adds 1 to placement
		static void ResetPlacement(); // Reset placement to 0
		static void DrawUpgradeHUD();
		static void DrawHUD();

		static void DrawAttacker(std::shared_ptr<Character>);
		static void DrawTower(std::shared_ptr<Tower>);
		static void DrawTile(std::shared_ptr<TileRoad> _tile);
		static void DrawText(std::shared_ptr<sf::Text> _text);
		static void DrawGameObject(std::shared_ptr<GameObject>);

		static void InitDrawer(sf::RenderWindow& window);
		static void FinalizeDrawer(sf::RenderWindow& window);

		static void SetUpgradeMenu(bool _value);

	private:
		Drawer() { }
		static Drawer* ptr_drawer; // pointer to our singleton game class
		static bool instanceFlag; // is our singleton class active

		static sf::RenderTexture gamePlane; // Texture plane for drawing
		static sf::RenderTexture hudPlane; // Texture plane for drawing
		static sf::RenderTexture upgradePlane; // Texture plane for upgrade menu

		static bool upgradeMenu; // Tells if the upgrade menu is shown
		static unsigned int placement; // helps to draw units in the right position in attack panel
		static sf::Sprite menuSprite;
		static sf::Texture menuImage;
		
		static sf::Sprite spriteAtt1; // sprite for the object (visual presentation of the object)
		static sf::Texture imageAtt1; // sprite texture

		static sf::Sprite spriteAtt2; // sprite for the object (visual presentation of the object)
		static sf::Texture imageAtt2; // sprite texture

		static sf::Sprite spriteAtt3; // sprite for the object (visual presentation of the object)
		static sf::Texture imageAtt3; // sprite texture

		static std::shared_ptr<sf::Text> ptr_structure;
		static std::shared_ptr<sf::Text> ptr_armor;
		static std::shared_ptr<sf::Text> ptr_shield;
		static std::shared_ptr<sf::Text> ptr_speed;
		static std::shared_ptr<sf::Text> ptr_ammo;
		static sf::Text attackText;
		static sf::CircleShape shield;
		static sf::RectangleShape metaRec;
		static sf::RectangleShape attackRec;
		static sf::RectangleShape planArea;
		static sf::RectangleShape recInfo;
		static sf::RectangleShape recUnits;
		static sf::RectangleShape closeRec;
		static std::shared_ptr<sf::Text> ptr_text;
		static sf::CircleShape ball;
		static sf::RectangleShape upgRec;

		static void DrawMetaStructure(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos);
		static void DrawMetaArmor(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos);
		static void DrawMetaShield(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos);
		static void DrawMetaSpeed(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos);
		
		static void DrawCloseBox(sf::RenderTexture * _ptr_plane, unsigned int _x, unsigned int _y);
		static void DrawBall(unsigned int _x, unsigned int _y, bool _state);

		static void UpdateAttackText();

		static const bool debug = false;
	};
}