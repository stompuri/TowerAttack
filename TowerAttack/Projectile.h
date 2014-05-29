#pragma once

#include "GameObject.h" // Parent class
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>

namespace TA
{
	class Projectile : public GameObject
	{
	public:
		Projectile(); // Constructor for empty object
		Projectile(sf::Vector2f _position, float _rotation, unsigned int _parentId); // Constructor which takes in different information for the object
		~Projectile();

		// For writing/reading to/from binary file
		Projectile(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		void Update(float elapsedTime); // Update method for the object, which is called once in every game loop iteration

		std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)

	private:
		unsigned int parentId; // objectId of the parent (tower)
	};
}