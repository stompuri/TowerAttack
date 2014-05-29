#pragma once

#include "stdafx.h"
#include <cassert>

#include <SFML/System.hpp> // We are using stuff from there
#include <SFML/Graphics.hpp> // We are using stuff from there

namespace TA
{
	class Tile // Base class for tiles in game board
	{
	public:
		Tile();
		~Tile();

		Tile(std::istream & is); // Constructor for loading saved game
		virtual void serialise(std::ostream & os); // Method to save game (serialise)

		void Load(std::string filename); // Method to load texture from a file
		void SetSpritePosition(float x, float y); // Set the x,y position for the object
		sf::Vector2f GetSpritePosition() const; // Return x,y position of the object
		bool IsLoaded() const; // Return true/false telling if the texture has been loaded for the object
		sf::Sprite& GetSprite(); // Return a handle for the objects sprite
		void setRotation(float _angle);

		bool GetMovable(std::string _direction); // Return true if tile is movable to asked direction
		void SetMovableUp(bool _value);
		void SetMovableDown(bool _value);
		void SetMovableLeft(bool _value);
		void SetMovableRight(bool _value);
		bool* GetMovableDirections(); // Return which directions are movable

	protected:
		bool movableUp, movableDown, movableLeft, movableRight;
		float rotation;

	private:
		sf::Sprite sprite; // sprite for the object (visual presentation of the object)
		sf::Texture image; // sprite texture
		std::string filename; // filename for the texture (where it is/was loaded)
		bool isLoaded; // true/false, is the texture loaded for this game object
	};
}

