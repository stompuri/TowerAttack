#pragma once

#include "GameObject.h" // Parent class
#include "Game.h" // We are using methods from the Game class
#include "Projectile.h" // We are creating a new instance of this class

#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>

namespace TA
{
	class GameObjectManager; // Forward declaration (only pointing there)
	class Turret; // Pointer to turret

	class Tower: public GameObject
	{
	public:
		Tower();
		~Tower();

		Tower(float _x, float _y);

		// For writing/reading to/from binary file:
		Tower(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		void SetPosition(float _x, float _y);
		void Update(float elapsedTime); // Update method for the object, which is called once in every game loop iteration

		std::string GetType(); // Return the type of the game object (Character, Tower, Projectile,...)

		std::weak_ptr<GameObject> GetTarget();

		unsigned int GetAmmoCount();
		unsigned int GetClipSize();
		void Reload(); // Reload the tower

		void SetTurret(std::shared_ptr<GameObject> _turret); // Set turret for the tower
		unsigned int GetTurretId();
		
		void SetMetaText(sf::Text _text);
		void UpdateMetaText(float _x, float _y, std::string _text);
		sf::Text GetMetaText();

	private:
		sf::Clock shootClock; // Clock to get current time, used to calculate shooting frequency
		unsigned int ammoCount; // How many times tower has shot
		float shootFreq; // How often tower can shoot
		unsigned int clipSize; // Clip size for the turret

		std::shared_ptr<GameObject> turret;
		unsigned int turretId;

		sf::Text metaText;

		float shootDistance;
		std::weak_ptr<GameObject> target;
		float CheckTargetDistance(); // Check the distance between the tower and target position, return -1 if target is out of range
		void SelectNewTarget(); // Check if there's some new target in range, if there's no existing target

	};
}
