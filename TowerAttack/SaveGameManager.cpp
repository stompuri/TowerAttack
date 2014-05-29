#include "SaveGameManager.h"
#include "Turret.h"

namespace TA
{
	SaveGameManager* SaveGameManager::ptr_saveGameManager = NULL;
	GameObjectManager* SaveGameManager::ptr_gameObjectManager = NULL;
	TileManager* SaveGameManager::ptr_tileManager = NULL;
	bool SaveGameManager::instanceFlag = false;
	std::map<std::string, std::string> SaveGameManager::markers;

	std::vector<std::shared_ptr<GameObject>> SaveGameManager::listCharacters; // local stack of characters
	std::vector<std::shared_ptr<GameObject>> SaveGameManager::listTowers; // local stack of towers
	std::vector<std::shared_ptr<GameObject>> SaveGameManager::listTurrets; // local stack of towers
	std::vector<std::shared_ptr<GameObject>> SaveGameManager::listProjectiles; // local stack of projectiles
	std::vector<std::shared_ptr<TileRoad>> SaveGameManager::listTiles; // local stack of tiles

	SaveGameManager* SaveGameManager::GetInstance()
	{
		if(!ptr_saveGameManager) {
			ptr_saveGameManager = new SaveGameManager();
			instanceFlag = true;
		}

		return ptr_saveGameManager;
	}

	void SaveGameManager::Init(GameObjectManager* _ptr_gom, TileManager* _ptr_tm)
	{	
		ptr_gameObjectManager = _ptr_gom;
		ptr_tileManager = _ptr_tm;
		markers["start"] = "ab";
		markers["gameObjects"] = "cd";
		markers["tiles"] = "ef";
		markers["end"] = "gh";
	}

	void SaveGameManager::SaveGame(std::string _filename) throw (...)
	{
		std::ofstream ofs(_filename, std::ios::binary);
		std::cout << "DEBUG <Game::SaveGame>: Opened binary stream to 'savegame.dat'!\n";

		// 1) Save 1st marker - beginning of the file
		SerialiseString(ofs, markers.find("start")->second);

		// 2) Save the view position
		float _viewPosX = Game::GetViewPosition().x;
		float _viewPosY = Game::GetViewPosition().y;
		ofs.write((char *)&_viewPosX, sizeof(float));
		ofs.write((char *)&_viewPosY, sizeof(float));

		// 3) Save 2nd marker - game objects
		SerialiseString(ofs, markers.find("gameObjects")->second);

		// 4) Save game objects
		ptr_gameObjectManager->SaveGame(ofs);

		// 5) Save 3rd marker - tiles
		SerialiseString(ofs, markers.find("tiles")->second);

		// 6) Save tiles
		ptr_tileManager->SaveGame(ofs);

		// 7) Save 4th marker - end of the file
		SerialiseString(ofs, markers.find("end")->second);

		ofs.close();
		std::cout << "DEBUG <Game::SaveGame>: Closed binary stream to 'savegame.dat'!\n";
	}

	void SaveGameManager::LoadGame(std::string _filename) throw(...)
	{
		std::ifstream ifs(_filename, std::ios::binary);
		std::cout << "DEBUG <GameObjectManager::LoadGame>: Opened binary stream for 'savegame.dat'!\n";
		if (ifs.is_open())
		{
			// Markers are used to mark different places in the save file, so we can notice a corrupted save file
			// 1st marker - start
			// 2nd marker - Game objects
			// 3rd marker - Tiles
			// 4th marker - end

			// First: do the preloading
			// 1) Read the start marker and check it's ok
			std::string _readMarker = ReadString(ifs);
			if(markers.find("start")->second != _readMarker) {
				ifs.close();
				ClearLists();
				throw std::exception("Could not open file! (bad first marker)");
			}
			std::cout << "DEBUG <SaveGameManager::LoadGame>: First marker OK";

			// 2) Read the view position
			float _viewPosX = 0;
			float _viewPosY = 0;
			ifs.read((char *)&_viewPosX, sizeof(float));
			ifs.read((char *)&_viewPosY, sizeof(float));

			// 3) Read the game objects marker and check it's ok
			_readMarker = ReadString(ifs);
			if(markers.find("gameObjects")->second != _readMarker) {
				ifs.close();
				ClearLists();
				throw std::exception("Could not open file! (bad game objects marker)");
			}
			std::cout << "DEBUG <SaveGameManager::LoadGame>: Game objects marker OK";
			// 4) Read game objects
			PreLoadGameObjects(ifs);
			
			// 5) Read the tiles marker and check it's ok
			_readMarker = ReadString(ifs);
			if(markers.find("tiles")->second != _readMarker) {
				ifs.close();
				ClearLists();
				throw std::exception("Could not open file! (bad tiles marker)");
			}
			std::cout << "DEBUG <SaveGameManager::LoadGame>: Tiles marker OK";
			// 6) Read the tiles
			PreLoadTiles(ifs);
			
			// 7) Read the end marker and check it's ok
			_readMarker = ReadString(ifs);
			if(markers.find("end")->second != _readMarker) {
				ifs.close();
				ClearLists();
				throw std::exception("Could not open file! (bad end marker)");
			}
			std::cout << "DEBUG <SaveGameManager::LoadGame>: End marker OK";

			// Second: if everything was preloaded fine, clear current objects from ongoing game & init the loaded game
			PostLoadGameObjects();
			PostLoadTiles();
			Game::SetViewPosition(_viewPosX, _viewPosY);

			// At the end: clear all local lists
			ClearLists();
		} else {
			throw std::exception("File not open, could not read!!");
			return;
		}
	}

	void SaveGameManager::PreLoadGameObjects(std::ifstream & ifs)
	{	
		// Read objectId counter
		unsigned int _oid = 0;
		ifs.read((char *)&_oid, sizeof(unsigned int));
		ptr_gameObjectManager->SetObjectId(_oid);

		// Read how many objects there're to recover
		int amount = 0;
		ifs.read((char *)&amount, sizeof(amount));
		std::cout << "DEBUG <GameObjectManager::LoadGame>: Found " + std::to_string(amount) + " gameObjects from the save file\n";

		// Set up mapping for pushing objects to local lists
		std::map<std::string, void (*)(std::ifstream & ifs)> m;
		m["Character"] = &PushCharacter;
		m["Tower"] = &PushTower;
		m["Turret"] = &PushTurret;
		m["Projectile"] = &PushProjectile;

		// Then read the GameObjects
		for(int i=0; i<amount; i++)
		{
			// 1) Read the object type
			std::string _type = ReadString(ifs);

			// 2) Read the object data & add the object into correct stack
			m.find(_type)->second(ifs);
		}
	}

	void SaveGameManager::PushCharacter(std::ifstream & ifs)
	{
		std::shared_ptr<CharacterSimple> _object(new CharacterSimple(ifs));
		listCharacters.push_back(std::move(_object));
	}

	void SaveGameManager::PushTower(std::ifstream & ifs)
	{
		std::shared_ptr<Tower> _object(new Tower(ifs));
		listTowers.push_back(std::move(_object));
	}

	void SaveGameManager::PushTurret(std::ifstream & ifs)
	{
		std::shared_ptr<Turret> _object(new Turret(ifs));
		listTurrets.push_back(std::move(_object));
	}

	void SaveGameManager::PushProjectile(std::ifstream & ifs)
	{
		std::shared_ptr<Projectile> _object(new Projectile(ifs));
		listProjectiles.push_back(std::move(_object));
	}

	void SaveGameManager::PostLoadGameObjects()
	{
		// Loaded data seems to be OK --> Clear all the existing gameObjects
		ptr_gameObjectManager->ClearObjects();

		// Initialize loaded game objects
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = listCharacters.begin(); it != listCharacters.end(); ++it) {
			ptr_gameObjectManager->Insert("Character_" + std::to_string(it->get()->GetObjectId()) + "_", *it);
		}
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = listTurrets.begin(); it != listTurrets.end(); ++it) {
			ptr_gameObjectManager->Insert("Turret_" + std::to_string(it->get()->GetObjectId()) + "_", *it);
		}
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = listTowers.begin(); it != listTowers.end(); ++it) {
			ptr_gameObjectManager->Insert("Tower_" + std::to_string(it->get()->GetObjectId()) + "_", *it);
			// Add pointer to turret (set value for turret with help of turretId)
			std::shared_ptr<Tower> _ptr_tower;
			_ptr_tower = std::dynamic_pointer_cast<Tower>(*it);
			_ptr_tower->SetTurret(ptr_gameObjectManager->Get(_ptr_tower->GetTurretId()));
		}
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = listProjectiles.begin(); it != listProjectiles.end(); ++it) {
			ptr_gameObjectManager->Insert("Projectile_" + std::to_string(it->get()->GetObjectId()) + "_", *it);
		}
	}

	void SaveGameManager::PreLoadTiles(std::ifstream & ifs)
	{
		// Read how many objects there're to recover
		int amount = 0;
		ifs.read((char *)&amount, sizeof(amount));

		std::cout << "DEBUG <SaveGameManager::PreLoadTiles>: Found " + std::to_string(amount) + " tileMap from the savedata.dat\n";

		// Then read the Tiles
		for(int i=0; i<amount; i++)
		{
			std::shared_ptr<TileRoad> _tile(new TileRoad(ifs));
			listTiles.push_back(_tile);
		}
	}

	void SaveGameManager::PostLoadTiles()
	{
		// Loaded data seems to be OK --> Clear all the existing tileMap
		ptr_tileManager->ClearObjects();

		// Initialize loaded game objects
		for(std::vector<std::shared_ptr<TileRoad>>::iterator it = listTiles.begin(); it != listTiles.end(); ++it) {
			ptr_tileManager->Add(it->get()->getX(), it->get()->getY(), *it);
		}
	}

	void SaveGameManager::SerialiseString(std::ostream & os, std::string _string)
	{
		std::cout << "DEBUG <SerialiseString>: Saving string: '" + _string + "'\n";
		// Save the length of the file name string
		int fSize = _string.length();
		os.write((char*)&fSize, sizeof(int));
		// Go through the file name string char by char saving each char
		const char* strdata = _string.c_str();

		std::cout << "DEBUG <SerialiseString>: Saving... fSize = " + std::to_string(fSize) + " strData[1] = " + strdata[1] + "\n";
		for (unsigned int i = 0; i < _string.length(); ++i)
		{
			os.write((char*)&strdata[i], sizeof(char)); // save filename char by char
			std::cout << "[" << strdata[i] << "]";
		}
		std::cout << "\nDEBUG <SerialiseString>: Filename saved\n";
	}

	std::string SaveGameManager::ReadString(std::istream & is)
	{
		std::cout << "DEBUG <ReadString>: Reading file name\n";
		// Read the length of the file name
		int fSize = 0;
		is.read((char*)&fSize, sizeof(int));
		std::cout << "DEBUG <ReadString>: Size = " + std::to_string(fSize) + "\n";
		// Make a char pointer and initialize space for the characters in file name string
		char *cString = new char[fSize];

		// Read the file name char by char
		for (int i = 0; i < fSize; ++i)
		{
			is.read((char*)&cString[i], sizeof(char)); // Read filename char by char
			std::cout << "[" << cString[i] << "]";
		}
		cString[fSize] = '\0';
		std::string _string = cString;
		std::cout << "\nDEBUG <ReadString>: String read:\n" + _string + "\n";

		return _string;
	}

	void SaveGameManager::ClearLists()
	{
		listCharacters.clear();
		listProjectiles.clear();
		listTowers.clear();
		listTurrets.clear();
		listTiles.clear();
	}
}