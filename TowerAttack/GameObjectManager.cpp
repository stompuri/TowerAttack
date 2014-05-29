#include "GameObjectManager.h"
#include "SaveGameManager.h"
#include "Game.h"
#include "Serialise.h"
#include "Turret.h"
#include "Drawer.h"

namespace TA
{
	GameObjectManager::GameObjectManager(): objectId(0)
	{
	}

	GameObjectManager::~GameObjectManager()
	{
		// Use lambda to delete each gameobject if the GameObjectManager is deleted
		std::for_each(gameObjects.begin(),gameObjects.end(),
			[] (std::pair<std::string,std::shared_ptr<GameObject>> p) { p.second.reset(); }
			);
	}

	unsigned int GameObjectManager::Add(std::string _name, std::shared_ptr<GameObject> _ptr_gameObject)
	{
		std::string _idname = _name.append("_" + std::to_string(objectId) + "_");
		if(debug) if(debug) std::cout << "DEBUG <GameObjectManager::Add>: " << _idname << " added\n";
		gameObjects.insert(std::pair<std::string,std::shared_ptr<GameObject>>(_idname, _ptr_gameObject));
		objectId++;
		return objectId-1;
	}

	void GameObjectManager::Insert(std::string _idname, std::shared_ptr<GameObject> _ptr_gameObject)
	{
		gameObjects.insert(std::pair<std::string,std::shared_ptr<GameObject>>(_idname, _ptr_gameObject));
		if(debug) std::cout << "DEBUG <GameObjectManager::Insert>: " << _idname << " added\n";
	}

	void GameObjectManager::Remove(std::string _name)
	{
		//commented1 if(debug) std::cout << "DEBUG <GameObjectManager::Remove>: Removing game object '"<< _name << "'\n";
		std::map<std::string, std::shared_ptr<GameObject>>::iterator result = gameObjects.find(_name);
		if(result != gameObjects.end() )
		{
			result->second.reset();
			delete result->second.get();
			gameObjects.erase(result);

			if(debug) std::cout << "DEBUG <GameObjectManager::Remove>: Removed GameObject!\n";
		}
	}

	std::shared_ptr<GameObject> GameObjectManager::Get(std::string name)
	{
		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator results = gameObjects.find(name);
		if(results == gameObjects.end() ) {
			if(debug) std::cout << "DEBUG <GameObjectManager::Get>: Didn't find GameObject!\n";
			return NULL;
		}
		//if(debug) std::cout << "DEBUG <GameObjectManager::Get> " << results->second->GetType() << "\n";
		return results->second;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetByType(std::string type)
	{
		//if(debug) std::cout << "DEBUG <GameObjectManager::Get> Searching for gameObject! gameObjects.size = " << gameObjects.size() << " finding with id = " << _id << "\n";
		std::vector<std::shared_ptr<GameObject>> _retu;
		std::map<std::string, std::shared_ptr<GameObject>>::iterator itr = gameObjects.begin();
		while(itr != gameObjects.end() ) {
			if(!itr->second->GetDeleteStatus()) {
				//if(debug) std::cout << "DEBUG <GameObjectManager::Get> searching..." << itr->first << " == " << _id << "\n";
				unsigned int found = itr->first.find(type);
				if (found != std::string::npos)
				{
					//if(debug) std::cout << "DEBUG <GameObjectManager::GetByType> FOUND, added '" << itr->first << "' to list!\n";
					_retu.push_back(itr->second);
				}
			}
			itr++;
		}
		return _retu;
	}

	std::shared_ptr<GameObject> GameObjectManager::Get(unsigned int _id)
	{
		//commented1 if(debug) std::cout << "DEBUG <GameObjectManager::Get> Searching for gameObject! gameObjects.size = " << gameObjects.size() << " finding with id = " << _id << "\n";
		std::map<std::string, std::shared_ptr<GameObject>>::iterator itr = gameObjects.begin();
		while(itr != gameObjects.end() ) {
			//if(debug) std::cout << "DEBUG <GameObjectManager::Get> searching..." << itr->first << " == " << _id << " type = " << itr->second->GetType() << "\n";
			unsigned int found = itr->first.find("_" + std::to_string(_id) + "_");
			if (found != std::string::npos)
			{
				//if(debug) std::cout << "DEBUG <GameObjectManager::Get> FOUND!\n";
				return itr->second;
			}
			itr++;
		}
		if(debug) std::cout << "DEBUG <GameObjectManager::Get> NOT FOUND!\n";
		assert(false);
		return NULL;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetByPosition(int _x, int _y)
	{
		std::vector<std::shared_ptr<GameObject>> _selectedObjects;

		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if(!itr->second->GetDeleteStatus()) {
				sf::Vector2f _position = itr->second->GetPosition();
				unsigned int _roundedX2 = int(floor((_position.x+100.0f) / 100.0f));
				unsigned int _roundedY2 = int(floor((_position.y+100.0f) / 100.0f));
				//if(debug) std::cout << "DEBUG<GameObjectManager::GetByPosition(int)>: _roundedX = (" << _position.x << ")" << _roundedX2 << "==" << _x << " _roundedY = (" << _position.y << ")" << _roundedY2 << " == " << _y << "\n";
				if (_roundedX2 == _x && _roundedY2 == _y)
					_selectedObjects.push_back(itr->second);
			}
			itr++;	
		}

		return _selectedObjects;
	}

	std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetByPosition(float _x, float _y)
	{
		unsigned int _roundedX1 = int(floor((_x+100.0f) / 100.0f));
		unsigned int _roundedY1 = int(floor((_y+100.0f) / 100.0f));

		std::vector<std::shared_ptr<GameObject>> _selectedObjects;

		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if(!itr->second->GetDeleteStatus()) {
				sf::Vector2f _position = itr->second->GetPosition();
				unsigned int _roundedX2 = int(floor((_position.x+100.0f) / 100.0f));
				unsigned int _roundedY2 = int(floor((_position.y+100.0f) / 100.0f));
				if(itr->second->GetType() == "CharacterSimple")
				if(debug) std::cout << "DEBUG<GameObjectManager::GetByPosition(float)>: (" << itr->second->GetType() << ") _roundedX1 = (" << _x << ")" <<
							_roundedX1 << " == (" << _position.x << ") " << _roundedX2 << " _roundedY1 = (" << _y << ")" <<
				 			_roundedY1 << " == (" << _position.y << ") " << _roundedY2 << "\n";
				if (_roundedX2 == _roundedX1 && _roundedY2 == _roundedY1) {
					_selectedObjects.push_back(itr->second);
					if(debug) std::cout << "Added: " << itr->second->GetType() << "\n";
				}
			}
			itr++;	
		}

		return _selectedObjects;
	}

	std::vector<std::weak_ptr<GameObject>> GameObjectManager::GetByDistanceFrom(float _x, float _y, float _dist)
	{
		unsigned int _roundedX1 = int((_x) / 100.0f);
		unsigned int _roundedY1 = int((_y) / 100.0f);

		std::vector<std::weak_ptr<GameObject>> _selectedObjects;

		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if(!itr->second->GetDeleteStatus()) {
				unsigned int _found = itr->second->GetType().find("Character");
				if(_found != std::string::npos)
				{
					std::shared_ptr<Character> _ptr_attacker = std::dynamic_pointer_cast<Character>(itr->second);
					if(_ptr_attacker->GetActivity()) {
						sf::Vector2f _targetPos = itr->second->GetPosition();
						float deltaX = abs(_targetPos.x - _x);
						float deltaY = abs(_targetPos.y - _y);

						// Check the distance
						if(sqrt(deltaX*deltaX + deltaY*deltaY) <= _dist && !itr->second->GetDeleteStatus())
							_selectedObjects.push_back(itr->second);
					}
				}
			}
			itr++;	
		}

		return _selectedObjects;
	}

	unsigned int GameObjectManager::GetObjectCount() const
	{
		return gameObjects.size();
	}

	unsigned int GameObjectManager::GetPassiveAtts()
	{
		//commented1 if(debug) std::cout << "<DEBUG> GetPassiveAtts\n";
		int returnValue = 0;
		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if(debug) std::cout << "<DEBUG> type = " << itr->first << "\n";
			if (itr->first.find("Character") != std::string::npos &&
				!std::dynamic_pointer_cast<Character>(itr->second)->GetActivity() &&
				!std::dynamic_pointer_cast<Character>(itr->second)->GetDeleteStatus()) {
				if(debug) std::cout << "<DEBUG> Character activity = " << std::dynamic_pointer_cast<Character>(itr->second)->GetActivity() << "\n";
				returnValue++;
			}
			itr++;	
		}

		return returnValue;
	}

	unsigned int GameObjectManager::GetAmountOf(std::string _name)
	{
		int returnValue = 0;
		std::map<std::string, std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if (itr->first == _name)
				returnValue++;
			itr++;	
		}

		return returnValue;
	}

	void GameObjectManager::Draw()
	{
		std::vector<std::shared_ptr<GameObject>> _projectiles; // stack for projectiles
		std::vector<std::shared_ptr<GameObject>> _turrets; // stack for turrets
		std::vector<std::shared_ptr<sf::Text>> _texts; // stack for texts (shield, armor, structure,...)

		// Go through all the gameobjects in gameobjects map and draw the sprites in them
		// Draw first other game objects, then projectiles on top of them
		std::map<std::string,std::shared_ptr<GameObject>>::iterator itr = gameObjects.begin();

		while(itr != gameObjects.end())
		{
			if(!itr->second->GetDeleteStatus())
			{
				if (itr->second->GetType() == "Projectile")
					_projectiles.push_back(itr->second);
				else if (itr->second->GetType() == "Turret") {
					_turrets.push_back(itr->second);
				} else if (itr->second->GetType() == "Tower") {
					Drawer::DrawTower(std::dynamic_pointer_cast<Tower>(itr->second));
					_texts.push_back(std::make_shared<sf::Text>(std::dynamic_pointer_cast<Tower>(itr->second)->GetMetaText()));
				} else if(itr->second->GetType().find("Character") != std::string::npos) {
					std::shared_ptr<Character> _ptr_attacker = std::dynamic_pointer_cast<Character>(itr->second);
					
					// Draw only active characters
					if(_ptr_attacker->GetActivity()) {
						Drawer::DrawAttacker(_ptr_attacker);
					}

				} else
					Drawer::DrawGameObject(itr->second);
			}
			itr++;
		}

		// Draw turrets on top of other game objects
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _turrets.begin(); it != _turrets.end(); ++it) {
			Drawer::DrawGameObject(*it);
		}
		
		// Draw texts
		for(std::vector<std::shared_ptr<sf::Text>>::iterator it = _texts.begin(); it != _texts.end(); ++it) {
			Drawer::DrawText(*it);
		}

		// Draw projectiles on top of other game objects
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _projectiles.begin(); it != _projectiles.end(); ++it) {
			Drawer::DrawGameObject(*it);
		}
	}

	void GameObjectManager::Update(sf::Time elapsedTime)
	{
		if(Game::GetPlayPhase() != "plan") { // Not a plan phase, but attack is going on
			// Go through all the gameobjects in the game world and update them +push the ones marked to be deleted into dellist
			std::map<std::string,std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
			while(itr != gameObjects.end())
			{
				if (!itr->second->GetDeleteStatus())
				{
					if(Game::GetPlayPhase() == itr->second->GetActivePhase() ||
					   itr->second->GetActivePhase() == "always")
					{
						// Do the generic update for all the game objects
						itr->second->Update(elapsedTime.asSeconds());
					}
				}
				itr++;
			}

			// Updated all the objects... & Attack is going on
			// 1) check if Attacker has done all its actions
			if(Game::GetPlayPhase() == "attack")
			{
				std::vector<std::shared_ptr<GameObject>> attackers = GetByType("Character");
				if(attackers.size() > 0) {
					std::vector<std::shared_ptr<GameObject>>::iterator t_itr = attackers.begin();
					while(t_itr != attackers.end())
					{
						std::shared_ptr<Character> _ptr_attacker = std::dynamic_pointer_cast<Character>(*t_itr);
						if(_ptr_attacker->GetActivity() && !t_itr->get()->GetReadiness())
							return;
						t_itr++;
					}

					if(debug) std::cout << "DEBUG <GameObjectManager::Update>: Next phase -> Defend\n";
					Game::NextPhase();
					Game::SetGoMark(false);

					// Attack phase finished, clear the readiness for all attackers
					t_itr = attackers.begin();
					while(t_itr != attackers.end())
					{
						t_itr->get()->SetReadiness(false);
						t_itr++;
					}
				} else {
					if(debug) std::cout << "DEBUG: going to plan\n";
					// No attackers left, go back to plan phase
					Game::SetPlayPhase("plan");
					// Delete objects marked to be deleted
					Flush();
					// Load all the towers
					std::vector<std::shared_ptr<GameObject>> _towers = GetByType("Tower");
					std::vector<std::shared_ptr<GameObject>>::iterator t_itr = _towers.begin();
					while(t_itr != _towers.end())
					{
						std::dynamic_pointer_cast<Tower>(*t_itr)->Reload();
						t_itr++;
					}

					// Set round & turn
					Game::NextPlayRound();
					Game::ResetTurn();

					// Reset placement for drawing units
					Drawer::ResetPlacement();

					// Add some unit points
					Game::AddAttackPoints(Game::GetPlayRound() * 3);

					// Add money
					Game::AddMoney(Game::GetPlayRound() + 1);
				}
				//if(debug) std::cout << "DEBUG <GameObjectManager::Update>: " << Game::GetPlayPhase() << " " << Game::GetPlayTurn() << " " << Game::GetGoMark() << "\n";
			} else {
				// 2) check if Defender has done all its actions
				if(Game::GetPlayPhase() == "defend")
				{
					std::vector<std::shared_ptr<GameObject>> towers = GetByType("Tower");
					std::vector<std::shared_ptr<GameObject>>::iterator t_itr = towers.begin();
					while(t_itr != towers.end())
					{
						if(!t_itr->get()->GetReadiness())
							return;
						t_itr++;
					}

					// Wait untill all projectiles have been deleted
					while(GetByType("Projectile").size() > 0) { return; }

					if(debug) std::cout << "DEBUG <GameObjectManager::Update>: Next phase -> Attack\n";
					Game::NextPhase();
					Game::NextTurn();
					Game::SetGoMark(true);

					// Defend phase finished, clear the readiness for all towers
					t_itr = towers.begin();
					while(t_itr != towers.end())
					{
						t_itr->get()->SetReadiness(false);
						t_itr++;
					}

					//if(debug) std::cout << "DEBUG <GameObjectManager::Update>: " << Game::GetPlayPhase() << " " << Game::GetPlayTurn() << " " << Game::GetGoMark() << "\n";
				}
			}
		}
	}

	void GameObjectManager::SaveGame(std::ofstream & ofs) throw (...)
	{
		std::map<std::string,std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();

		// Save objectId counter
		ofs.write((char *)&objectId, sizeof(unsigned int));
		if(debug) std::cout << "DEBUG <GameObjectManager::SaveGame>: objectId = " << objectId << "\n";

		// Save the amount of GameObjects
		int amount = gameObjects.size();
		ofs.write((char *)&amount, sizeof(amount));
		if(debug) std::cout << "DEBUG <GameObjectManager::SaveGame>: amout = " << amount << "\n";

		while(itr != gameObjects.end())
		{
			if(debug) std::cout << "DEBUG <GameObjectManager::SaveGame>: type = " << itr->second->GetType() << "\n";
			// Save the type of the game object
			SaveGameManager::SerialiseString(ofs, itr->second->GetType());

			// Save (serialise) the game object
			itr->second->serialise(ofs);
			itr++;
		}

		if(debug) std::cout << "DEBUG <GameObjectManager::SaveGame>: Saved game objects!\n";
	}

	// Check if there're any game objects blocking the way
	bool GameObjectManager::CheckMovable(float _x, float _y, GameObject* _myself)
	{
		unsigned int _roundedX1 = int(floor(_x / 100.0f));
		unsigned int _roundedY1 = int(floor(_y / 100.0f));

		// 1) Go through all the gameobjects in the game world and check if any of them are travelling towards the target tile
		std::map<std::string,std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if (itr->second.get() != _myself &&
				!itr->second.get()->GetDeleteStatus())
			{
				// Go through all the character objects and check if they are heading towards x,y
				if (itr->second->GetType().find("Character") != std::string::npos)
				{
					//std::cout << _roundedX1 << "/" << _roundedY1 << ": " << !std::dynamic_pointer_cast<Character>(itr->second)->GetArrived() << " " << std::dynamic_pointer_cast<Character>(itr->second)->GetTarget().x << "/" << std::dynamic_pointer_cast<Character>(itr->second)->GetTarget().y << "\n";
					if (!std::dynamic_pointer_cast<Character>(itr->second)->GetArrived() && // object is travelling
						std::dynamic_pointer_cast<Character>(itr->second)->GetTarget().x == _roundedX1 && // object is travelling towards the same x
						std::dynamic_pointer_cast<Character>(itr->second)->GetTarget().y == _roundedY1) // object is travelling towards the same y
					{
						return false;
					}
				}
			}
			itr++;
		}

		// 2) Go through all the gameobjects in the game world and check if they are located in _x, _y
		itr = gameObjects.begin();
		//if(debug) std::cout << "DEBUG <GameObjectManager::CheckMovable>: x = " << _roundedX1 << " y = " << _roundedY1 << "\n";
		while(itr != gameObjects.end())
		{
			if (itr->second.get() != _myself &&
				!itr->second.get()->GetDeleteStatus())
			{
				if (itr->second->GetType().find("Character") == std::string::npos ||
					itr->second->GetType().find("Character") != std::string::npos &&
					std::dynamic_pointer_cast<Character>(itr->second)->GetArrived()) // object is still in the tile (arrived)
				{
					//if(debug) std::cout << "DEBUG <GameObjectManager::CheckMovable>: " << itr->second->GetType() << "\n";
					// Make sure the x,y coords are rounded
					sf::Vector2f _position = itr->second->GetPosition();
					unsigned int _roundedX2 = int(floor((_position.x+100.0f) / 100.0f));
					unsigned int _roundedY2 = int(floor((_position.y+100.0f) / 100.0f));
					//if(debug) std::cout << "DEBUG <GameObjectManager::CheckMovable>: x = " << _roundedX2 << " y = " << _roundedY2 << "\n";
					if (_roundedX2 == _roundedX1 && _roundedY2 == _roundedY1)
						return false;
				}
			}
			itr++;
		}

		return true;
	}

	unsigned int GameObjectManager::GetObjectId()
	{
		return objectId;
	}
	
	void GameObjectManager::SetObjectId(unsigned int _oid)
	{
		objectId = _oid;
	}

	void GameObjectManager::ClearObjects()
	{
		gameObjects.clear();
		if(debug) std::cout << "DEBUG <GameObjectManager::LoadGame>: Cleared all the gameObjects!\n";
	}

	void GameObjectManager::Flush()
	{
		// Go through all the gameobjects in the game world and update them +push the ones marked to be deleted into dellist
		std::map<std::string,std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		std::list<std::string> delList; // list of gameObjects to be deleted
		
		while(itr != gameObjects.end())
		{
			if (itr->second->GetDeleteStatus())
			{
				delList.push_back(itr->first);
			}
			itr++;
		}
		// Go through the game objects in delete list and remove them
		std::list<std::string>::iterator d_itr = delList.begin();
		while(d_itr != delList.end())
		{
			if(debug) std::cout << "DEBUG <GameObjectManager::Update>: Delete gameObject '" << d_itr->data() << "'!\n";
			Remove(d_itr->data());
			d_itr++;
		}
	}

	void GameObjectManager::Upgrade(std::string _type, std::string _attribute, unsigned int _value)
	{
		// Go through all the gameobjects and update the _attribute for the _type objects
		std::map<std::string,std::shared_ptr<GameObject>>::const_iterator itr = gameObjects.begin();
		while(itr != gameObjects.end())
		{
			if (!itr->second.get()->GetDeleteStatus() &&
				itr->second->GetType().find(_type) != std::string::npos)
			{
				if(_attribute == "Speed")
					std::dynamic_pointer_cast<Character>(itr->second)->SetSpeed(std::dynamic_pointer_cast<Character>(itr->second)->GetSpeed() + _value);
				else if(_attribute == "Structure")
					std::dynamic_pointer_cast<Character>(itr->second)->SetStructure(std::dynamic_pointer_cast<Character>(itr->second)->GetStructure() + _value);
				else if(_attribute == "Armor")
					std::dynamic_pointer_cast<Character>(itr->second)->SetArmor(std::dynamic_pointer_cast<Character>(itr->second)->GetArmor() + _value);
				else if(_attribute == "Shield")
					std::dynamic_pointer_cast<Character>(itr->second)->SetShield(std::dynamic_pointer_cast<Character>(itr->second)->GetShield() + _value);
				else
					if(debug) std::cout << "DEBUG<GameObjectManager::Upgrade>: ERROR! No such attribute!\n";
			}
			itr++;
		}
	}
}