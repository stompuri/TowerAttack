#include "StatePlay.h"
#include "StateMenu.h"
#include "StateMidMenu.h"
#include "StatePlayUpgrade.h"
#include "Drawer.h"
#include "stdafx.h"

namespace TA
{
	StatePlay StatePlay::m_PlayState;
	bool StatePlay::paused = false;
	sf::Vector2f StatePlay::viewPosition;

	std::list<StatePlay::ClickItem> StatePlay::clickItems; // List the different menu items
	StatePlay::ClickResult StatePlay::action;
	std::shared_ptr<Character> StatePlay::ptr_removeCharacter;
	StatePlay::ClickItem StatePlay::dynamicClickRegion;

	void StatePlay::Init()
	{
		printf("StatePlay Init\n");

		// Center view to start tile
		sf::Vector2f _pos = Game::GetTileManager()->Get(Game::GetTileManager()->GetStartPosition().x, Game::GetTileManager()->GetStartPosition().y)->GetSpritePosition();
		std::cout << "DEBUG<StatePlay::Init>: Start sprite position x = " << _pos.x << " y = " << _pos.y << "\n";
		
		float _startX = _pos.x - Game::GetView().getCenter().x;
		float _startY = _pos.y - Game::GetView().getCenter().y;

		if(viewPosition.x == NULL) {
			std::cout << "DEBUG<StatePlay::Init>: View position x = " << Game::GetViewPosition().x << " y = " << Game::GetViewPosition().y << "\n";
			std::cout << "DEBUG<StatePlay::Init>: Start position x = " << _startX << " y = " << _startY << "\n";
			Game::SetViewPosition(_startX, _startY);
			viewPosition.x = _startX;
			viewPosition.y = _startY;
		} else {
			Game::SetViewPosition(viewPosition.x, viewPosition.y);
		}

		// Set up clickable regions
		// Dynamic click region
		dynamicClickRegion.rect.height = 15;
		dynamicClickRegion.rect.left = 70;
		dynamicClickRegion.rect.width = 12;
		dynamicClickRegion.action = DeleteItem;

		// Play - menu item coordinates
		ClickItem attackButton;
		attackButton.rect.top = 465;
		attackButton.rect.height = 40;
		attackButton.rect.left = 10;
		attackButton.rect.width = 80;
		attackButton.action = Attack;
		clickItems.push_back(attackButton);

		// Add CharacterSimple
		ClickItem addSimple;
		addSimple.rect.top = 535;
		addSimple.rect.height = 60;
		addSimple.rect.left = 40;
		addSimple.rect.width = 60;
		addSimple.action = AddSimple;
		clickItems.push_back(addSimple);
		
		// Add CharacterFast
		ClickItem addFast;
		addFast.rect.top = 535;
		addFast.rect.height = 60;
		addFast.rect.left = 115;
		addFast.rect.width = 60;
		addFast.action = AddFast;
		clickItems.push_back(addFast);
		
		// Add CharacterHeavy
		ClickItem addHeavy;
		addHeavy.rect.top = 535;
		addHeavy.rect.height = 60;
		addHeavy.rect.left = 190;
		addHeavy.rect.width = 60;
		addHeavy.action = AddHeavy;
		clickItems.push_back(addHeavy);

		// Add Upgrade button
		ClickItem UpgradeRec;
		UpgradeRec.rect.top = 545;
		UpgradeRec.rect.height = 40;
		UpgradeRec.rect.left = 300;
		UpgradeRec.rect.width = 100;
		UpgradeRec.action = UpgradeItem;
		clickItems.push_back(UpgradeRec);
	}

	void StatePlay::Cleanup()
	{
		printf("StatePlay Cleanup\n");
	}

	void StatePlay::Pause()
	{
		printf("StatePlay Pause\n");
		if(paused)
			Resume();
		else
			paused = true;
	}

	void StatePlay::Resume()
	{
		printf("StatePlay Resume\n");
		paused = false;
		Game::GetElapsedTime();
	}

	void StatePlay::HandleEvents(sf::RenderWindow& window)
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
			{
				Game::Quit();
			} else

			if(Event.type == sf::Event::MouseButtonPressed)
			{
				ClickResult _action = HandleClick(Event.mouseButton.x,Event.mouseButton.y);
				if(_action == Attack) {
					std::cout << "DEBUG: Pressed attack\n";
					if(Game::GetAttackPoints() == 0) {
						PostPlanActions(); // Delete all the click areas defined for the units in the left panel
						Game::GetBaseCSimple()->LockUpLevels();
						Game::GetBaseCFast()->LockUpLevels();
						Game::GetBaseCHeavy()->LockUpLevels();
						Game::SetPlayPhase("attack");
					} else {
						std::cout << "DEBUG<StatePlay::HandleEvents>: Attackpoints left, can't go to attack mode!\n";
					}
				} else if(_action == AddSimple) {
					std::cout << "DEBUG: Pressed addSimple\n";
					if(Game::GetAttackPoints() >= 1) {
						AddDynamicClickRegion(AddUnit("Simple")); // Add click region for delete action
						Game::ReduceAttackPoints(1);
					}
				} else if(_action == AddFast) {
					std::cout << "DEBUG: Pressed addFast\n";
					if(Game::GetAttackPoints() >= 2) {
						AddDynamicClickRegion(AddUnit("Fast")); // Add click region for delete action
						Game::ReduceAttackPoints(2);
					}
				} else if(_action == AddHeavy) {
					std::cout << "DEBUG: Pressed addHeavy\n";
					if(Game::GetAttackPoints() >= 3) {
						AddDynamicClickRegion(AddUnit("Heavy")); // Add click region for delete action
						Game::ReduceAttackPoints(3);
					}
				} else if(_action == DeleteItem) { // User pressed the delete icon for an unit in the left panel
					// Set the delete status to true
					ptr_removeCharacter->SetDeleteStatus(true);
					// Save the placement number of the deleted unit
					unsigned int _placement = ptr_removeCharacter->GetPlacement();
					// Increase the player attackpoints according to the delete unit
					Game::AddAttackPoints(ptr_removeCharacter->GetCost());
					// Clear the pointer used to delete the units
					ptr_removeCharacter.reset();
					delete ptr_removeCharacter.get();
					// Update the placement numbers of the left units
					UpdatePlacement(_placement);
				} else if(_action == UpgradeItem) {
					std::cout << "DEBUG: Pressed UpgradeItem\n";
					Game::ChangeState( StatePlayUpgrade::Instance() );
				} else
					std::cout << "DEBUG: No event found for click\n";
			} else

			if(Event.type == sf::Event::KeyPressed)
			{
				if(Event.key.code == sf::Keyboard::S) // Save key 'S'
					Game::SaveGame("savegame.dat");
				if(Event.key.code == sf::Keyboard::L) // Load key 'L'
					Game::LoadGame("savegame.dat");
				if(Event.key.code == sf::Keyboard::P) // Pause key 'P'
					Pause();
				if(Event.key.code == sf::Keyboard::Escape) {
					std::cout << "DEBUG: ESC pressed\n";
					viewPosition = Game::GetViewPosition();
					Game::SetViewPosition(0.0f, 0.0f);
					Game::ChangeState( StateMidMenu::Instance() );
				}
				if(Event.key.code == sf::Keyboard::A) {
					Game::AddMoney(10);
				}
				if(Event.key.code == sf::Keyboard::D) {
					std::cout << "DEBUG <StatePlay::keyEvent>: Pressed D: x=" << Game::GetViewPosition().x << " y = " << Game::GetViewPosition().y << "\n";
					std::cout << "DEBUG <StatePlay::keyEvent>: Pressed D: x=" << Game::GetViewPosition().x+400 << " y = " << Game::GetViewPosition().y+300 << "\n";

					Game::GetBaseCSimple()->LockUpLevels();
					Game::GetBaseCFast()->LockUpLevels();
					Game::GetBaseCHeavy()->LockUpLevels();
				}
			}
		}
	}

	void StatePlay::Update() 
	{
		// Handle screen scrolling (ugly way)
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			Game::SetViewPosition(Game::GetViewPosition().x, Game::GetViewPosition().y-10);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			Game::SetViewPosition(Game::GetViewPosition().x, Game::GetViewPosition().y+10);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			Game::SetViewPosition(Game::GetViewPosition().x-10, Game::GetViewPosition().y);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			Game::SetViewPosition(Game::GetViewPosition().x+10, Game::GetViewPosition().y);
		}

		if(!paused) {
			Game::GetGameObjectManager()->Update(Game::GetElapsedTime());
		}
	}

	void StatePlay::Draw(sf::RenderWindow& window) 
	{
		// Clear the screen (fill it with white color)
		window.clear(sf::Color::White);

		// Init drawer planes
		Drawer::InitDrawer(window);

		// Draw game map
		Game::GetTileManager()->Draw();

		// Draw the GameObjects
		Game::GetGameObjectManager()->Draw();

		// Draw HUD
		Drawer::DrawHUD();

		// Combine draw planes
		Drawer::FinalizeDrawer(window);

		// Display window contents on screen
		window.display();
	}

	StatePlay::ClickResult StatePlay::HandleClick(int x, int y)
	{
		//std::cout << "Mouse clik on x = " << x << " y = " << y << "\n";
		std::list<ClickItem>::iterator it;
		for ( it = clickItems.begin(); it != clickItems.end(); it++)
		{
			sf::Rect<int> clickItemRect = (*it).rect;
			//std::cout << "ir.top = " << clickItemRect.top << " ir.height = " << clickItemRect.height << " ir.left = " << clickItemRect.left << " ir.width = " << clickItemRect.width << "\n";
			if( clickItemRect.top + clickItemRect.height > y
				&& clickItemRect.top < y 
				&& clickItemRect.left < x 
				&& clickItemRect.left + clickItemRect.width > x)
				{
					if((*it).clickedItem != NULL)
					{
						if((*it).action == DeleteItem)
						{
							ptr_removeCharacter = (*it).clickedItem;
							std::cout << "DEBUG: deleteCLICK\n";
							std::cout << "DEBUG: Deleting clickArea " << (*it).rect.top << "\n";
							clickItems.erase(it);
							std::cout << "DEBUG: Deleted clickArea\n";
							return DeleteItem;
						} else if((*it).action == UpgradeItem)
						{
							std::cout << "DEBUG: upgradeCLICK\n";
						}
					}
					return (*it).action;
				}
		}

		return Nothing;
	}

	// Load texture from a file
	void StatePlay::Load(sf::Sprite _sprite, sf::Texture _image, std::string _filename)
	{
		if(!_image.loadFromFile(_filename))
		{
			std::cout << "DEBUG <StatePlay::Load>: FAIL!\n";
		}
		else
		{
			_sprite.setTexture(_image);
			std::cout << "DEBUG <StatePlay::Load>: SUCCESS! Filename = " << _filename << "\n";
		}
	}

	std::shared_ptr<Character> StatePlay::AddUnit(std::string _type)
	{
		unsigned int _placement = Game::GetGameObjectManager()->GetPassiveAtts() + Game::GetPlayTurn();//Game::GetGameObjectManager()->GetPassiveAtts();
		std::cout << "placement = " << _placement << "\n";
		if(_type == "Simple") {
			std::shared_ptr<CharacterSimple> _attacker(new CharacterSimple(_placement));
			_attacker->SetPosition(Game::GetTileManager()->GetStartPosition().x*Game::GetTileManager()->GetTileWidth()-50, Game::GetTileManager()->GetStartPosition().y*Game::GetTileManager()->GetTileWidth()-50);
			_attacker->SetTargetPosition(int(Game::GetTileManager()->GetStartPosition().x), int(Game::GetTileManager()->GetStartPosition().y));
			_attacker->SetObjectId(Game::GetGameObjectManager()->Add("Character", _attacker));
			std::cout << "DEBUG<StatePlay::AddUnit>: Added Simple unit! " << _attacker->GetPlacement() << "\n";
			return _attacker;
		} else if(_type == "Fast") {
			std::shared_ptr<CharacterFast> _attacker(new CharacterFast(_placement));
			_attacker->SetPosition(Game::GetTileManager()->GetStartPosition().x*Game::GetTileManager()->GetTileWidth()-50, Game::GetTileManager()->GetStartPosition().y*Game::GetTileManager()->GetTileWidth()-50);
			_attacker->SetTargetPosition(int(Game::GetTileManager()->GetStartPosition().x), int(Game::GetTileManager()->GetStartPosition().y));
			_attacker->SetObjectId(Game::GetGameObjectManager()->Add("Character", _attacker));
			std::cout << "DEBUG: StatePlay.cpp::AddUnit: Added Fast unit!\n";
			return _attacker;
		} else if(_type == "Heavy") {
			std::shared_ptr<CharacterHeavy> _attacker(new CharacterHeavy(_placement));
			_attacker->SetPosition(Game::GetTileManager()->GetStartPosition().x*Game::GetTileManager()->GetTileWidth()-50, Game::GetTileManager()->GetStartPosition().y*Game::GetTileManager()->GetTileWidth()-50);
			_attacker->SetTargetPosition(int(Game::GetTileManager()->GetStartPosition().x), int(Game::GetTileManager()->GetStartPosition().y));
			_attacker->SetObjectId(Game::GetGameObjectManager()->Add("Character", _attacker));
			std::cout << "DEBUG: StatePlay.cpp::AddUnit: Added Heavy unit!\n";
			return _attacker;
		}
		return NULL;
	}

	void StatePlay::UpdatePlacement(unsigned int _placement)
	{
		std::cout << "DEBUG<StatePlay::UpdatePlacement>: placement = " << _placement << "\n";
		// Go through the attackers and reduce the placement number
		std::vector<std::shared_ptr<GameObject>> _attackers = Game::GetGameObjectManager()->GetByType("Character");
		std::cout << "Attacker amount = " << _attackers.size() << "\n";
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _attackers.begin(); it != _attackers.end(); it++) {
			std::shared_ptr<Character> _ita = std::dynamic_pointer_cast<Character>(*it);
			if(_ita->GetPlacement() > _placement) {
				_ita->SetPlacement(_ita->GetPlacement() - 1);
				std::cout << "DEBUG: New placement = " << _ita->GetPlacement() << "\n";
			}
		}
		
		// Go thourgh the click recions and update them accordingly
		//1st delete the click recion for deleted placement
		std::list<ClickItem>::iterator it;
		for ( it = clickItems.begin(); it != clickItems.end(); it++)
		{
			if( (*it).action == DeleteItem && (*it).clickedItem != NULL)
			{
				std::cout << "Update placement: " << (*it).clickedItem->GetType() << " old = " << (*it).rect.top << " new = " << float(((*it).clickedItem->GetPlacement() - Game::GetPlayTurn())*60 + 70) << "\n";
				(*it).rect.top = int(((*it).clickedItem->GetPlacement() - Game::GetPlayTurn())*60 + 70);
			}
		}
	}

	/* Go through the clickItems and remove the ones marked to be deleted
	*/
	void StatePlay::PostPlanActions()
	{
		std::list<ClickItem>::iterator it = clickItems.begin();
		while(it != clickItems.end())
		{
			if((*it).action == DeleteItem)
			{
				std::cout << "DEBUG: Deleting clickArea " << (*it).rect.top << "\n";
				it = clickItems.erase(it);
			} else
				it++;
		}
	}

	void StatePlay::AddDynamicClickRegion(std::shared_ptr<Character> _ptr_char)
	{
		dynamicClickRegion.rect.top = int((_ptr_char->GetPlacement() - Game::GetPlayTurn())*60 + 70);
		dynamicClickRegion.clickedItem = _ptr_char;
		clickItems.push_back(dynamicClickRegion);
	}
}