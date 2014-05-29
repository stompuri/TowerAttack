#pragma once
#include "GameObject.h"
#include "Tower.h"

namespace TA
{
	class SaveGameManager
	{
	public:
		// This ended to be singleton class...could be a normal class...(too lazy to change now...)
		~SaveGameManager() { instanceFlag = false; }
		static SaveGameManager* GetInstance();
		static void Init(GameObjectManager* _ptr_gom, TileManager* _ptr_tm); // Init save game manager with pointers to game object manager & tile manager

		static void SaveGame(std::string _filename); // Serialise data into a binary file
		static void LoadGame(std::string _filename); // Deserialise data from the binary file and push the data into local lists

		static void SerialiseString(std::ostream & os, std::string _string); // Serialise string into bytes
		static std::string ReadString(std::istream & is); // Read string (1st the length 2nd the characters) and return it

		friend class UnitTests; // We want to unit test the private methods

	private:
		SaveGameManager() { /* private constructor */ };
		static SaveGameManager* ptr_saveGameManager; // "Singleton class": pointer to our singleton game object
		static bool instanceFlag;
		
		static GameObjectManager* ptr_gameObjectManager; // Pointer to game object manager
		static TileManager* ptr_tileManager; // Pointer to tile manager

		static void PreLoadGameObjects(std::ifstream & ifs); // Save the game object data from binary stream as objects & save them into local lists
		static void PostLoadGameObjects(); // Go through the local lists and add game objects into game object manager

		static void PreLoadTiles(std::ifstream & ifs); // Save the data from binary stream as objects & save them into local lists
		static void PostLoadTiles(); // Go through the local tile list and add tile objects into tile manager

		static std::map<std::string, std::string> markers; // Map for pushing objects into lists
		static void PushCharacter(std::ifstream & ifs); // Push character object
		static void PushTower(std::ifstream & ifs); // Push tower object
		static void PushTurret(std::ifstream & ifs); // Push turret object
		static void PushProjectile(std::ifstream & ifs); // Push projectile object

		static void ClearLists(); // Empty all local lists
		static std::vector<std::shared_ptr<GameObject>> listCharacters; // local list of characters
		static std::vector<std::shared_ptr<GameObject>> listTowers; // local list of towers
		static std::vector<std::shared_ptr<GameObject>> listTurrets; // local list of towers
		static std::vector<std::shared_ptr<GameObject>> listProjectiles; // local list of projectiles
		static std::vector<std::shared_ptr<TileRoad>> listTiles; // local list of tiles
	};
}
