#pragma once

#include "stdafx.h"
#include <cassert>
#include <SFML/System.hpp> // We are using stuff from there
#include <SFML/Graphics.hpp> // We are using stuff from there

namespace TA
{
	class GameObject // Base class for game objects
	{
	public:
		GameObject();
		~GameObject();

		// Save/Load game:
		GameObject(std::istream & is); // Constructor for loading a saved game (binary format). Each object type has to have it's own version of this kind of constructor.
		virtual void serialise(std::ostream & os); // Method to save the game (serialise data into binary format). Virtual, as each object has to have their own version of this method.

		void Load(std::string filename); // Method to load texture from a file
		void SetPosition(float x, float y); // Set the x,y position for the game object sprite
		sf::Vector2f GetPosition() const; // Return x,y position of the game object sprite
		bool IsLoaded() const; // Return true/false telling if the texture has been loaded for the object
		bool GetDeleteStatus() const; // Return true/false telling if the object has been flagged for deletion
		void SetDeleteStatus(bool status); // Set true/false value for the object, should the object be deleted (in the next game loop update)
		sf::Sprite& GetSprite(); // Return a handle for the objects sprite
		sf::Vector2f GetVelocity() const; // Return the x,y velocity of the object
		void SetVelocity(sf::Vector2f _velocity); // Set forward velocity of the game object
		float GetRotation() const; // Return the rotation value of the object
		void SetRotation(float _rotation); // Set rotation angle of the game object
		void SetObjectId(unsigned int _id); // Sets the objectID
		unsigned int GetObjectId() const; // Get the objectID

		virtual void Update(float elapsedTime) = 0; // Virtual method. Update method to be called from the game loop
		virtual std::string GetType() = 0; // Virtual method. Return the type of the game object (Character, Tower, Projectile,...)

		std::string GetActivePhase();
		
		bool GetReadiness();
		void SetReadiness(bool _value);

	protected:
		unsigned int id; // id that gameObjectManager gave to the object
		float rotation; // What's the rotation status of this object
		sf::Vector2f velocity; // Speed (x,y) of this object
		float maxVelocity; // Maximum speed (limited to) of this object
		std::string activePhase; // The game phase in which the object is updated (active)
		unsigned int gameTurn; // The turn which is going on in game (update object to be in same turn)
		bool ready; // Boolean flag if the game object has acted (turn phased gaming)

	private:
		bool toBeDeleted; // flag telling if the object should be deleted (in game loop)
		sf::Sprite sprite; // sprite for the object (visual presentation of the object)
		sf::Texture image; // sprite texture
		std::string filename; // filename for the texture (where it is/was loaded)
		bool isLoaded; // true/false, is the texture loaded for this game object

		static const bool debug = false;
	};
}