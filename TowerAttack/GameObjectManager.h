#pragma once

#include "GameObject.h" // This class handles GameObjects, forward declaration is not enough

#include "stdafx.h"
#include <list>
#include <map>
#include <fstream>
#include <memory>
#include <cassert>

namespace TA
{
	class GameObjectManager
	{
	public:	
		GameObjectManager();
		~GameObjectManager();

		unsigned int Add(std::string name, std::shared_ptr<GameObject>); // Add game object into game object map (all objects in the game world) and returns the id given to the object
		void Insert(std::string name, std::shared_ptr<GameObject>); // Insert game object into game object map with predefined id (not using increasing counter)
		void Remove(std::string name); // Remove game object from the game object map (all objects in the game world)
		unsigned int GetObjectCount() const; // Return how many objects there're in the game world
		std::shared_ptr<GameObject> Get(std::string name); // Return a handle to the specific game object in the game world
		std::shared_ptr<GameObject> Get(unsigned int _id); // Return a handle to the specific game object in the game world
		std::vector<std::shared_ptr<GameObject>> GetByType(std::string type); // Return a list of handles to all objects with certain type
		std::vector<std::shared_ptr<GameObject>> GetByPosition(float _x, float _y); // Return list of game objects in certain position
		//std::vector<std::shared_ptr<GameObject>> GetByPosition(int _x, int _y); // Return list of game objects in certain position
		std::vector<std::weak_ptr<GameObject>> GetByDistanceFrom(float _x, float _y, float _dist); // Return list of game objects with max dist of _dist from _x, _y
		unsigned int GetPassiveAtts(); // Return number of passive characters
		unsigned int GetAmountOf(std::string name); // Return how many objects of the specified name exists
		void ClearObjects(); // Clear game objects from gameObjects
		void Flush(); // Delete objects marked to be deleted

		void Draw(); // Draw objects in the game world
		void Update(sf::Time elapsedTime); // Update objecst in the game world

		void SaveGame(std::ofstream & ofs); // Save the all the game objects (serialise data into the binary file)

		bool CheckMovable(float _x, float _y, GameObject* _myself); // Checks if there's any (other than the object itself) objects preventing movement in x, y

		unsigned int GetObjectId(); // Each object has an id number, which is set by ObjectManager
		void SetObjectId(unsigned int _oid); // Object id is set "manually" after loading the game (so we keep the same id as in the previous game state)

		void Upgrade(std::string _type, std::string _attribute, unsigned int _value);
		int PushDrawText(std::shared_ptr<sf::Text> _text); // push text into texts and return id

	private:
		unsigned int objectId; // Increasing counter for the object IDs that are saved in the map
		std::map<std::string, std::shared_ptr<GameObject>> gameObjects; // Maping all the game objects in the game world. Using shared pointers.

		static const bool debug = true;
	};
}