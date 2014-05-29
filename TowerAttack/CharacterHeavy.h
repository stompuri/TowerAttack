#pragma once

//#include "GameObject.h" 
#include "Character.h" // Parent class

#include "stdafx.h"
#include <cassert>
#include <memory>

namespace TA
{
	//class GameObjectManager; // Forward declaration (only pointing there)

	class CharacterHeavy: public Character
	{
	public:
		CharacterHeavy();
		CharacterHeavy(unsigned int _placement);
		~CharacterHeavy();

		// For writing/reading to/from binary file:
		CharacterHeavy(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		//void Update(float elapsedTime); // Update method for the object, which is called once in every game loop iteration
		std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)
	};
}

//#include "Game.h" // We are using methods from the Game class
//#include "Projectile.h" // We are creating a new instance of this class