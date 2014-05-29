#pragma once
#include "Game.h"
#include "Character.h"
#include "Tower.h"
#include "Turret.h"
#include "Projectile.h"
#include "SaveGameManager.h"

namespace TA
{
	class UnitTests
	{
	public:
		UnitTests();
		~UnitTests();
		void RunTests(); // Execute all tests

	private:
		bool Game_Test1(); // Get/SetViewPosition
		bool Game_Test2(); // GetScreenWidth/Heigth
		bool Game_Test3(); // GetGameObjectManager
		bool Game_Test4(); // GetTileManager
		bool Game_Test5(); // Save&LoadGame
		bool GameObject_Test1(); // Set/Get rotation
		bool GameObject_Test2(); // Set/Get velocity
		bool GameObject_Test3(); // Set/Get object id
		bool Character_Test1(); // Save&Load
		bool Character_Test2(); // GetType
		bool Character_Test3(); // Serialise method & Load constructor
		bool Projectile_Test1(); // Save&Load
		bool Projectile_Test2(); // GetType
		bool Projectile_Test3(); // Serialise method & Load constructor
		bool Tower_Test1(); // Save&Load
		bool Tower_Test2(); // GetType
		bool Tower_Test3(); // Serialise method & Load constructor
		bool Turret_Test1(); // Save&Load
		bool Turret_Test2(); // GetType
		bool Turret_Test3(); // Serialise method & Load constructor
		bool GameObjectManager_Test1(); // Test Add method
		bool GameObjectManager_Test2(); // Test CheckMovable method
		bool GameObjectManager_Test3(); // GetByType
		bool GameObjectManager_Test4(); // Get(by id)
		bool GameObjectManager_Test5(); // GetByPosition
		bool SaveGameManager_Test1(); // Save/Load game
		bool SaveGameManager_Test2(); // SerialiseString & ReadString
		bool SaveGameManager_Test3(); // Preload game objects
		bool SaveGameManager_Test4(); // Postload game objects
		bool TileManager_Test1(); // CheckMovable
		bool TileManager_Test2(); // GetStartPosition
		bool Tile_Test1(); // GetMovable
		bool Tile_Test2(); // GetMovableDirections
		bool TileRoad_Test1(); // Serialise method & Load constructor
	};
}
