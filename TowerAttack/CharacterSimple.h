#pragma once

//#include "GameObject.h" 
#include "Character.h" // Parent class

#include "stdafx.h"
#include <cassert>
#include <memory>

namespace TA
{
	//class GameObjectManager; // Forward declaration (only pointing there)

	class CharacterSimple: public Character
	{
	public:
		CharacterSimple();
		CharacterSimple(unsigned int _placement);
		~CharacterSimple();

		// For writing/reading to/from binary file:
		CharacterSimple(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)

	};
}