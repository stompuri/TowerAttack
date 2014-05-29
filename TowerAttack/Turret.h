#pragma once

#include "GameObject.h" // Parent class
#include "Game.h" // We are using methods from the Game class
#define _USE_MATH_DEFINES
#include <math.h>

namespace TA
{
	class GameObjectManager; // Forward declaration (only pointing there)

	class Turret: public GameObject
	{
	public:
		Turret();
		~Turret();

		Turret(float _x, float _y);

		// For writing/reading to/from binary file:
		Turret(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		void Update(float elapsedTime); // Update method for the object, which is called once in every game loop iteration

		std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)

		void SetTarget(std::weak_ptr<GameObject> _target);

	private:
		std::weak_ptr<GameObject> target;
		void SelectClosestTarget();

	};
}