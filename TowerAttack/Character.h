#pragma once

#include "GameObject.h" // Parent class

#include "stdafx.h"
#include <cassert>
#include <memory>

namespace TA
{
	class Character: public GameObject
	{
	public:
		Character();
		//Character(unsigned int _placement);
		~Character();

		// For writing/reading to/from binary file:
		Character(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		void Update(float elapsedTime); // Update method for the object, which is called once in every game loop iteration
		void SetTargetPosition(unsigned int _x, unsigned int _y); // Set the targetPosition

		//std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)

		void TakeHit(unsigned int _damage);
		bool GetActivity();
		unsigned int GetPlacement();
		unsigned int GetStructure();
		unsigned int GetArmor();
		unsigned int GetShield();
		unsigned int GetSpeed();
		void SetStructure(unsigned int _value);
		void SetArmor(unsigned int _value);
		void SetShield(unsigned int _value);
		void RechargeShield(unsigned int _value = 1);
		void SetSpeed(unsigned int _value);
		unsigned int GetTravelled();
		void SetTravelled(unsigned int _value);
		unsigned int GetCost();

		unsigned int GetUpLevelSpeed(); // Return total upgrade level
		unsigned int GetLockedUpLevelSpeed();
		unsigned int GetUpLevelArmor();
		unsigned int GetLockedUpLevelArmor();
		unsigned int GetUpLevelShield();
		unsigned int GetLockedUpLevelShield();
		void AddUpLevelSpeed(unsigned int _value); // Add value to the total upgrade level
		void AddUpLevelArmor(unsigned int _value);
		void AddUpLevelShield(unsigned int _value);
		void ReduceUpLevelArmor(unsigned int _value);
		void ReduceUpLevelSpeed(unsigned int _value);
		void ReduceUpLevelShield(unsigned int _value);
		void ResetUpLevels();

		void LockUpLevels(); // Lock upgrade levels for all attributes

		void SetPlacement(unsigned int _value);

		bool GetArrived(); // Tell if the object is arrived to its target or travelling (false/true)
		sf::Vector2f GetTarget(); // Return the x,y target where to object is travelling to

		void SetMetaText(sf::Text _text);
		sf::Text GetMetaText();

	protected:
		sf::Clock moveClock; // Clock to get current time, used to calculate moving frequency
		sf::Vector2f targetPosition; // Position where the character is moving to
		std::string lastDirection; // Direction where the character goes
		unsigned int travelledTiles; // Amount of tiles the character has moved in this turn
		bool arrived; // Boolean telling if the character has arrived to the target cell
		bool active; // Boolean telling if the attacker has been activated

		struct Attribute
		{
		public:
			unsigned int baseValue;
			unsigned int currentValue;
		};

		unsigned int placement; // In which turn this attacker should start moving
		unsigned int armor;
		unsigned int structure;
		Attribute shield;
		unsigned int speed;
		unsigned int cost; // How many attack points this unit costs

		sf::Text metaText;

		struct UpLevel
		{
		public:
			unsigned int lockedLevel;
			unsigned int totalLevel;
		};

		UpLevel ulSpeed; // Amount of upgrades to the attribute (0 = none)
		UpLevel ulArmor;
		UpLevel ulShield;

		bool SetNextDirection(sf::Vector2f _location);

	private:
		static const bool debug = false;
	};
}