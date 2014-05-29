#include "UnitTests.h"

namespace TA
{
	UnitTests::UnitTests()
	{
	}


	UnitTests::~UnitTests()
	{
	}

	void UnitTests::RunTests()
	{
		std::map<bool, std::string> resultM;
		resultM.insert(std::pair<bool, std::string>(true, "PASS"));
		resultM.insert(std::pair<bool, std::string>(false, "FAIL"));
		std::vector<std::string> _listResults;
		_listResults.push_back("Game::Get/SetViewPosition\t= " + resultM.find(Game_Test1())->second + "\n");
		_listResults.push_back("Game::getScreenWidth/Heigth\t= " + resultM.find(Game_Test2())->second + "\n");
		_listResults.push_back("Game::GetGameObjectManager\t= " + resultM.find(Game_Test3())->second + "\n");
		_listResults.push_back("Game::GetTileManager\t\t= " + resultM.find(Game_Test4())->second + "\n");
		_listResults.push_back("Game::Save&LoadGame\t\t= " + resultM.find(Game_Test5())->second + "\n");
		_listResults.push_back("GameObject::Set/Get rotation\t= " + resultM.find(GameObject_Test1())->second + "\n");
		_listResults.push_back("GameObject::Set/Get velocity\t= " + resultM.find(GameObject_Test2())->second + "\n");
		_listResults.push_back("GameObject::Set/Get object id\t= " + resultM.find(GameObject_Test3())->second + "\n");
		_listResults.push_back("Character::Save&Load\t\t= " + resultM.find(Character_Test1())->second + "\n");
		_listResults.push_back("Character::GetType\t\t= " + resultM.find(Character_Test2())->second + "\n");
		_listResults.push_back("Character::Serialise & Load constructor\t= " + resultM.find(Character_Test3())->second + "\n");
		_listResults.push_back("Projectile::Save&Load\t\t= " + resultM.find(Projectile_Test1())->second + "\n");
		_listResults.push_back("Projectile::GetType\t\t= " + resultM.find(Projectile_Test2())->second + "\n");
		_listResults.push_back("Projectile::Serialise & Load constructor\t= " + resultM.find(Projectile_Test3())->second + "\n");
		_listResults.push_back("Tower::Save&Load\t\t= " + resultM.find(Tower_Test1())->second + "\n");
		_listResults.push_back("Tower::GetType\t\t\t= " + resultM.find(Tower_Test2())->second + "\n");
		_listResults.push_back("Tower::Serialise & Load constructor\t= " + resultM.find(Tower_Test3())->second + "\n");
		_listResults.push_back("Turret::Save&Load\t\t= " + resultM.find(Turret_Test1())->second + "\n");
		_listResults.push_back("Turret::GetType\t\t\t= " + resultM.find(Turret_Test2())->second + "\n");
		_listResults.push_back("Turret::Serialise & Load constructor\t= " + resultM.find(Turret_Test3())->second + "\n");
		_listResults.push_back("GameObjectManager::Add\t\t= " + resultM.find(GameObjectManager_Test1())->second + "\n");
		_listResults.push_back("GameObjectManager::CheckMovable\t= " + resultM.find(GameObjectManager_Test2())->second + "\n");
		_listResults.push_back("GameObjectManager::GetByType\t= " + resultM.find(GameObjectManager_Test3())->second + "\n");
		_listResults.push_back("GameObjectManager::GetById\t= " + resultM.find(GameObjectManager_Test4())->second + "\n");
		_listResults.push_back("GameObjectManager::GetByPosition\t= " + resultM.find(GameObjectManager_Test5())->second + "\n");
		_listResults.push_back("SaveGameManager::Save/LoadGame\t\t= " + resultM.find(SaveGameManager_Test1())->second + "\n");
		_listResults.push_back("SaveGameManager::Serialise/Read string\t= " + resultM.find(SaveGameManager_Test2())->second + "\n");
		_listResults.push_back("SaveGameManager::Pre load game objects\t= " + resultM.find(SaveGameManager_Test3())->second + "\n");
		_listResults.push_back("SaveGameManager::Post load game objects\t= " + resultM.find(SaveGameManager_Test4())->second + "\n");
		_listResults.push_back("TileManager::CheckMovable\t\t= " + resultM.find(TileManager_Test1())->second + "\n");
		_listResults.push_back("TileManager::GetStartPosition\t\t= " + resultM.find(TileManager_Test2())->second + "\n");
		_listResults.push_back("Tile::GetMovable\t\t\t= " + resultM.find(Tile_Test1())->second + "\n");
		_listResults.push_back("Tile::GetMovableDirections\t\t= " + resultM.find(Tile_Test2())->second + "\n");
		_listResults.push_back("TileRoad::Serialise & Load constructor\t= " + resultM.find(TileRoad_Test1())->second + "\n");

		// Route cout stream to unittests.txt file
		std::ofstream out("unittests_results.txt");
		std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

		std::cout << "******* UNIT TESTS *******\n";
		for(std::vector<std::string>::iterator itr = _listResults.begin(); itr != _listResults.end(); itr++)
		{
			std::cout << itr->data();
		}
		std::cout << "******* UNIT TESTS *******\n\n";
		
		std::cout.rdbuf(coutbuf); //reset to standard output again
	}

	bool UnitTests::Game_Test1()
	{
		// Test Set & Get viewposition methods of Game class
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();

		float _origX = game->GetViewPosition().x;
		float _origY = game->GetViewPosition().y;
		float _x = 123.123f;
		float _y = 321.321f;

		game->SetViewPosition(_x, _y);
		float _afterX = game->GetViewPosition().x;
		float _afterY = game->GetViewPosition().y;
		_result = (_x == _afterX);
		if(_result)
			_result = (_y == _afterY);
		
		game->SetViewPosition(_origX, _origY);
		return _result;
	}

	bool UnitTests::Game_Test2()
	{
		// GetScreenWidth/Heigth
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		unsigned int _screenWidth = game->getScreenWidth();
		unsigned int _screenHeigth = game->getScreenHeigth();
		_result = (_screenWidth == 800);
		if(_result)
			_result = (_screenHeigth == 600);

		return _result;
	}

	bool UnitTests::Game_Test3()
	{
		//GetGameObjectManager
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		_result = (game->GetGameObjectManager() != NULL);
		return _result;
	}

	bool UnitTests::Game_Test4()
	{
		//GetTileManager
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		_result = (game->GetTileManager() != NULL);
		return _result;
	}

	bool UnitTests::Game_Test5()
	{
		//Save game
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		float _viewX = game->GetViewPosition().x;
		float _viewY = game->GetViewPosition().y;
		game->SetViewPosition(666, 666);
		game->SaveGame("unittest_savegame.dat");
		game->SetViewPosition(_viewX, _viewY);
		game->LoadGame("unittest_savegame.dat");
		sf::Vector2f _pos = game->GetViewPosition();
		_result = (_pos.x == 666 && _pos.y == 666);
		game->SetViewPosition(_viewX, _viewY);

		return _result;
	}

	bool UnitTests::GameObject_Test1()
	{
		//Set/Get rotation
		bool _result = true;
		std::shared_ptr<CharacterSimple> _ptr_object(new CharacterSimple());
		_ptr_object->SetRotation(66.66f);
		_result = (_ptr_object->GetRotation() == 66.66f);

		return _result;
	}

	bool UnitTests::GameObject_Test2()
	{
		//Set/Get velocity
		bool _result = true;
		std::shared_ptr<CharacterSimple> _ptr_object(new CharacterSimple());
		_ptr_object->SetVelocity(sf::Vector2f(66.6f, 66.6f));
		_result = (_ptr_object->GetVelocity().x == 66.6f && _ptr_object->GetVelocity().y == 66.6f);
		
		return _result;
	}

	bool UnitTests::GameObject_Test3()
	{
		//Set/Get object id
		bool _result = true;
		std::shared_ptr<CharacterSimple> _ptr_object(new CharacterSimple());
		_ptr_object->SetObjectId(123);
		_result = (_ptr_object->GetObjectId() == 123);
		
		return _result;
	}

	bool UnitTests::Character_Test1()
	{
		//Save/Load character
		bool _result = true;
		std::shared_ptr<CharacterSimple> _ptr_object(new CharacterSimple());
		_ptr_object->SetRotation(123.123f);
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_ptr_object->serialise(ofs);
		ofs.close();
		_ptr_object->SetRotation(222.0f);
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<CharacterSimple> _character(new CharacterSimple(ifs));
		ifs.close();
		_result = (_character->GetRotation() == 123.123f);
		
		return _result;
	}

	bool UnitTests::Character_Test2()
	{
		//Get type
		bool _result = true;
		std::shared_ptr<CharacterSimple> _ptr_object(new CharacterSimple());

		_result = (_ptr_object->GetType() == "Character");
		
		return _result;
	}

	bool UnitTests::Character_Test3()
	{
		//Serialise method & Load constructor
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		// Create character object and set object id for it
		std::shared_ptr<CharacterSimple> _object(new CharacterSimple());
		_object->SetObjectId(666);

		// Save the character object by using its serialise method
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_object->serialise(ofs);
		ofs.close();
		
		// Create new character object by using load constructor
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<CharacterSimple> _object2(new CharacterSimple(ifs));
		ifs.close();
		
		_result = (_object2->GetObjectId() == 666);
		
		return _result;
	}

	bool UnitTests::Projectile_Test1()
	{
		//Save/Load projectile
		bool _result = true;
		std::shared_ptr<Projectile> _ptr_object(new Projectile());
		_ptr_object->SetRotation(123.123f);
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_ptr_object->serialise(ofs);
		ofs.close();
		_ptr_object->SetRotation(222.0f);
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Projectile> _projectile(new Projectile(ifs));
		ifs.close();
		_result = (_projectile->GetRotation() == 123.123f);
		
		return _result;
	}

	bool UnitTests::Projectile_Test2()
	{
		//Get type
		bool _result = true;
		std::shared_ptr<Projectile> _ptr_object(new Projectile());

		_result = (_ptr_object->GetType() == "Projectile");
		
		return _result;
	}

	bool UnitTests::Projectile_Test3()
	{
		//Serialise method & Load constructor
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		// Create character object and set object id for it
		std::shared_ptr<Projectile> _object(new Projectile());
		_object->SetObjectId(666);

		// Save the character object by using its serialise method
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_object->serialise(ofs);
		ofs.close();

		//Reset _character1 to be sure we are not using old data...
		_object.reset();

		// Create new character object by using load constructor
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Projectile> _object2(new Projectile(ifs));
		ifs.close();

		_result = (_object2->GetObjectId() == 666);
		
		return _result;
	}

	bool UnitTests::Tower_Test1()
	{
		//Save/Load projectile
		bool _result = true;
		std::shared_ptr<Tower> _ptr_object(new Tower());
		_ptr_object->SetRotation(123.123f);
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_ptr_object->serialise(ofs);
		ofs.close();
		_ptr_object->SetRotation(222.0f);
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Tower> _turret(new Tower(ifs));
		ifs.close();
		_result = (_turret->GetRotation() == 123.123f);
		
		return _result;
	}

	bool UnitTests::Tower_Test2()
	{
		//Get type
		bool _result = true;
		std::shared_ptr<Tower> _ptr_object(new Tower());

		_result = (_ptr_object->GetType() == "Tower");
		
		return _result;
	}

	bool UnitTests::Tower_Test3()
	{
		//Serialise method & Load constructor
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		// Create character object and set object id for it
		std::shared_ptr<Tower> _object(new Tower());
		_object->SetObjectId(666);

		// Save the character object by using its serialise method
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_object->serialise(ofs);
		ofs.close();

		//Reset _character1 to be sure we are not using old data...
		_object.reset();

		// Create new character object by using load constructor
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Tower> _object2(new Tower(ifs));
		ifs.close();

		_result = (_object2->GetObjectId() == 666);
		
		return _result;
	}

	bool UnitTests::Turret_Test1()
	{
		//Save/Load projectile
		bool _result = true;
		std::shared_ptr<Turret> _ptr_object(new Turret());
		_ptr_object->SetRotation(123.123f);
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_ptr_object->serialise(ofs);
		ofs.close();
		_ptr_object->SetRotation(222.0f);
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Turret> _turret(new Turret(ifs));
		ifs.close();
		_result = (_turret->GetRotation() == 123.123f);
		
		return _result;
	}

	bool UnitTests::Turret_Test2()
	{
		//Get type
		bool _result = true;
		std::shared_ptr<Turret> _ptr_object(new Turret());

		_result = (_ptr_object->GetType() == "Turret");
		
		return _result;
	}

	bool UnitTests::Turret_Test3()
	{
		//Serialise method & Load constructor
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		// Create character object and set object id for it
		std::shared_ptr<Turret> _object(new Turret());
		_object->SetObjectId(666);

		// Save the character object by using its serialise method
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_object->serialise(ofs);
		ofs.close();

		//Reset _character1 to be sure we are not using old data...
		_object.reset();

		// Create new character object by using load constructor
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<Turret> _object2(new Turret(ifs));
		ifs.close();

		_result = (_object2->GetObjectId() == 666);
		
		return _result;
	}

	bool UnitTests::GameObjectManager_Test1()
	{
		//Add
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";
		_result = (game->GetGameObjectManager()->Get(_findString)->GetType() == "Character");
		
		// Remove used objects
		game->GetGameObjectManager()->Remove(_findString);

		return _result;
	}

	bool UnitTests::GameObjectManager_Test2()
	{
		//CheckMovable (check if there's any game object in the tile blocking the way
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		_character1->SetPosition(2000, 2000);
		std::shared_ptr<CharacterSimple> _character2(new CharacterSimple());
		_character2->SetPosition(1000, 1000);
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		std::string _findString1 = "Character_" + std::to_string(_retu) + "_";
		_retu = game->GetGameObjectManager()->Add("Character", _character2);
		std::string _findString2 = "Character_" + std::to_string(_retu) + "_";

		// 1) check that the checkmovable method returns false, if two object are at the same location
		_result = !(game->GetGameObjectManager()->CheckMovable(1000, 1000, _character1.get()));
		//std::cout << "DEBUG <UnitTests::GameObjectManager_Test2>: _result = " << _result << "\n";
		if(_result)
		{
			// 2) check that the checkmovable method returns true, if they are not at the same location
			_character2->SetPosition(3000, 3000);
			_result = (game->GetGameObjectManager()->CheckMovable(1000, 1000, _character1.get()));
		}
		
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString1);
		game->GetGameObjectManager()->Remove(_findString2);

		return _result;
	}

	bool UnitTests::GameObjectManager_Test3()
	{
		//GetByType
		TA::Game *game = TA::Game::getInstance();
		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";

		std::vector<std::shared_ptr<GameObject>> _findList = game->GetGameObjectManager()->GetByType("Character");
		// Go through all the found objects & check they all are "Character" type
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _findList.begin(); it != _findList.end(); it++)
		{
			if(it->get()->GetType() != "Character")
				return false;
		}
		
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString);

		return true;
	}

	bool UnitTests::GameObjectManager_Test4()
	{
		//GetById
		bool _result = true;
		TA::Game *game = TA::Game::getInstance();
		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";
		
		std::shared_ptr<GameObject> _character2(nullptr);
		_character2 = game->GetGameObjectManager()->Get(_retu);
		_result = (_character1->GetObjectId() == _character2->GetObjectId());
		
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString);

		return _result;
	}

	bool UnitTests::GameObjectManager_Test5()
	{
		//GetByPosition
		bool _result = false;
		TA::Game *game = TA::Game::getInstance();
		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		_character1->SetPosition(666.6f, 666.6f);
		_character1->SetObjectId(123);
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";

		// 1) test object is in the searched location
		std::vector<std::shared_ptr<GameObject>> _findList = game->GetGameObjectManager()->GetByPosition(666.6f, 666.6f);
		// Go through all the found objects & check character1 is in them
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _findList.begin(); it != _findList.end(); it++)
		{
			//std::cout << "DEBUG <UnitTests::GameObjectManager_Test5>: GetObjectId = " << it->get()->GetType() << " " << it->get()->GetObjectId() << " _retu = " << _retu << "\n";
			if(it->get()->GetObjectId() == 123)
			{
				_result = true;
				break;
			}
		}
		//std::cout << "DEBUG <UnitTests::GameObjectManager_Test5>: _result = " << _result << " _findList.size = " << _findList.size() << "\n";

		// 2) test object is not in the searched location
		if(_result)
		{
			_findList = game->GetGameObjectManager()->GetByPosition(150, 150);
			// Go through all the found objects & check the character1 is not found from them
			for(std::vector<std::shared_ptr<GameObject>>::iterator it = _findList.begin(); it != _findList.end(); it++)
			{
				if(it->get()->GetObjectId() == 123)
					return false;
			}
		}
		
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString);

		return _result;
	}

	bool UnitTests::SaveGameManager_Test1()
	{
		//Save/Load the game
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		sgm->SaveGame("unittest_savegame.dat");

		sgm->LoadGame("unittest_savegame.dat");

		return _result;
	}

	bool UnitTests::SaveGameManager_Test2()
	{
		// SerialiseString & Readstring
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		sgm->SerialiseString(ofs, "Unittests");
		ofs.close();
		
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::string _string = sgm->ReadString(ifs);
		ifs.close();

		_result = (_string == "Unittests");

		return _result;
	}

	bool UnitTests::SaveGameManager_Test3()
	{
		// Preload game objects
		bool _result = false;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		_character1->SetObjectId(_retu);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";

		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		game->GetGameObjectManager()->SaveGame(ofs);
		ofs.close();
		
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		sgm->PreLoadGameObjects(ifs);
		ifs.close();

		//std::cout << "DEBUG <UnitTests::GameObjectManager_Test3>: list size = " << sgm->listCharacters.size() << "\n";

		// Check that our character1 object is found from the local list in saveGameManager
		// if found, return true, if not return false
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = sgm->listCharacters.begin(); it != sgm->listCharacters.end(); it++)
		{
			//std::cout << "DEBUG <UnitTests::GameObjectManager_Test3>: GetObjectId = " << it->get()->GetType() << " " << it->get()->GetObjectId() << " _retu = " << _retu << "\n";
			if(it->get()->GetObjectId() == _retu)
			{
				_result = true;
				break;
			}
		}

		// Clear all the object lists
		sgm->ClearLists();
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString);

		return _result;
	}

	bool UnitTests::SaveGameManager_Test4()
	{
		// Postload game objects
		bool _result = false;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		std::shared_ptr<CharacterSimple> _character1(new CharacterSimple());
		unsigned int _retu = game->GetGameObjectManager()->Add("Character", _character1);
		_character1->SetObjectId(_retu);
		std::string _findString = "Character_" + std::to_string(_retu) + "_";

		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		game->GetGameObjectManager()->SaveGame(ofs);
		ofs.close();
		
		// Clear game object manager objects
		game->GetGameObjectManager()->ClearObjects();

		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		sgm->PreLoadGameObjects(ifs);
		ifs.close();

		sgm->PostLoadGameObjects();  // <- We're testing this method

		// Check that our character1 object post loaded correctly
		// and it can be found from the gameObjectManager
		std::shared_ptr<GameObject> _character2(nullptr);
		_character2 = game->GetGameObjectManager()->Get(_retu);
		_result = (_character1->GetObjectId() == _character2->GetObjectId());

		// Clear all the object lists
		sgm->ClearLists();
		// Clear used objects
		game->GetGameObjectManager()->Remove(_findString);

		return _result;
	}

	bool UnitTests::TileManager_Test1()
	{
		// CheckMovable
		bool _result = true;
		TA::TileManager *tlm = TA::Game::getInstance()->GetTileManager();

		// Place start tile
		std::shared_ptr<TileRoad> _tile(new TileRoad(99, 666, 666));
		tlm->Add(666, 666, _tile);

		// Check that we get true for movable(right)
		_result = tlm->CheckMovable(666, 666, "right");

		// Check that we get false for movable(up)
		if(_result) _result = !(tlm->CheckMovable(666, 666, "up"));

		// Check that we get false for movable(left)
		if(_result) _result = !(tlm->CheckMovable(666, 666, "left"));

		// Check that we get false for movable(down)
		if(_result) _result = !(tlm->CheckMovable(666, 666, "down"));
		
		// Clean up
		tlm->Remove(666, 666);

		return _result;
	}

	bool UnitTests::TileManager_Test2()
	{
		// GetStartPosition
		bool _result = true;
		bool _placed = false;
		TA::TileManager *tlm = TA::Game::getInstance()->GetTileManager();

		sf::Vector2f _startTile = tlm->GetStartPosition();
		if(_startTile.x == 9999 && _startTile.y == 9999) // No existing start tile found, add one
		{
			// Place start tile
			std::shared_ptr<TileRoad> _tile(new TileRoad(99, 666, 666));
			tlm->Add(666, 666, _tile);
			_placed = true;
		}
		_startTile = tlm->GetStartPosition();
		bool* _startPos = tlm->Get(unsigned int(_startTile.x), unsigned int(_startTile.y))->GetMovableDirections();
		// If GetStartPosition returned the start tile position, we can check the movable directions of it
		_result = (!_startPos[0] && !_startPos[1] && _startPos[2] && !_startPos[3]);
		
		// Clean up
		if(_placed)
			tlm->Remove(666, 666);

		return _result;
	}

	bool UnitTests::Tile_Test1()
	{
		// GetMovable
		bool _result = true;
		
		// Place start tile
		std::shared_ptr<TileRoad> _tile(new TileRoad(99, 666, 666));

		// Check the directions for the start tile
		_result = !(_tile->GetMovable("up")) && _tile->GetMovable("right");

		return _result;
	}

	bool UnitTests::Tile_Test2()
	{
		// GetMovableDirections
		bool _result = true;
		bool _placed = false;
		TA::TileManager *tlm = TA::Game::getInstance()->GetTileManager();

		// Place start tile
		std::shared_ptr<TileRoad> _tile(new TileRoad(99, 666, 666));

		bool* _startPos = _tile->GetMovableDirections();
		// If GetStartPosition returned the start tile position, we can check the movable directions of it
		_result = (!_startPos[0] && !_startPos[1] && _startPos[2] && !_startPos[3]);

		return _result;
	}

	bool UnitTests::TileRoad_Test1()
	{
		//Serialise method & Load constructor
		bool _result = true;
		TA::SaveGameManager *sgm = TA::SaveGameManager::GetInstance();
		TA::Game *game = TA::Game::getInstance();

		// Create character object and set object id for it
		std::shared_ptr<TileRoad> _object(new TileRoad());
		_object->setX(666);

		// Save the character object by using its serialise method
		std::ofstream ofs("unittest_savegame.dat", std::ios::binary);
		_object->serialise(ofs);
		ofs.close();

		//Reset _character1 to be sure we are not using old data...
		_object.reset();

		// Create new character object by using load constructor
		std::ifstream ifs("unittest_savegame.dat", std::ios::binary);
		std::shared_ptr<TileRoad> _object2(new TileRoad(ifs));

		ifs.close();

		_result = (_object2->getX() == 666);
		
		return _result;
	}

}