#include "StatePlayUpgrade.h"
#include "StateMenu.h"
#include "StatePlay.h"
#include "Drawer.h"
#include "stdafx.h"

namespace TA
{
	StatePlayUpgrade StatePlayUpgrade::m_PlayStateUpgrade;
	sf::Vector2f StatePlayUpgrade::viewPosition;

	std::list<StatePlayUpgrade::ClickItem> StatePlayUpgrade::clickItems; // List the different menu items
	StatePlayUpgrade::ClickResult StatePlayUpgrade::action;

	const int LVL1_LEFT = 439;
	const int LVL2_LEFT = 463;
	const int LVL3_LEFT = 487;
	const int LVL4_LEFT = 511;

	void StatePlayUpgrade::Init()
	{
		printf("StatePlayUpgrade Init\n");

		// Set up clickable regions
		ClickItem closeBox;
		closeBox.rect.top = 40;
		closeBox.rect.height = 15;
		closeBox.rect.left = 515;
		closeBox.rect.width = 12;
		closeBox.action = Close;
		closeBox.cost = -1;
		clickItems.push_back(closeBox);
		
		// Attribute areas
		//Simple
		ClickItem simpleSpeed;
		simpleSpeed.rect.top = 87;
		simpleSpeed.rect.height = 20;
		simpleSpeed.rect.left = LVL1_LEFT;
		simpleSpeed.rect.width = 20;
		simpleSpeed.action = SimpleSpeed;
		simpleSpeed.cost = 2;
		simpleSpeed.level = 1;
		clickItems.push_back(simpleSpeed);

		ClickItem simpleArmor;
		simpleArmor.rect.top = 111;
		simpleArmor.rect.height = 20;
		simpleArmor.rect.left = LVL1_LEFT;
		simpleArmor.rect.width = 20;
		simpleArmor.action = SimpleArmor;
		simpleArmor.cost = 2;
		simpleArmor.level = 1;
		clickItems.push_back(simpleArmor);

		ClickItem simpleShield;
		simpleShield.rect.top = 135;
		simpleShield.rect.height = 20;
		simpleShield.rect.left = LVL1_LEFT;
		simpleShield.rect.width = 20;
		simpleShield.action = SimpleShield;
		simpleShield.cost = 2;
		simpleShield.level = 1;
		clickItems.push_back(simpleShield);

		//Fast
		ClickItem fastSpeed1;
		fastSpeed1.rect.top = 203;
		fastSpeed1.rect.height = 20;
		fastSpeed1.rect.left = LVL1_LEFT;
		fastSpeed1.rect.width = 20;
		fastSpeed1.action = FastSpeed;
		fastSpeed1.cost = 1;
		fastSpeed1.level = 1;
		clickItems.push_back(fastSpeed1);

		ClickItem fastSpeed2;
		fastSpeed2.rect.top = 203;
		fastSpeed2.rect.height = 20;
		fastSpeed2.rect.left = LVL2_LEFT;
		fastSpeed2.rect.width = 20;
		fastSpeed2.action = FastSpeed;
		fastSpeed2.cost = 2;
		fastSpeed2.level = 2;
		clickItems.push_back(fastSpeed2);
		
		ClickItem fastSpeed3;
		fastSpeed3.rect.top = 203;
		fastSpeed3.rect.height = 20;
		fastSpeed3.rect.left = LVL3_LEFT;
		fastSpeed3.rect.width = 20;
		fastSpeed3.action = FastSpeed;
		fastSpeed3.cost = 3;
		fastSpeed3.level = 3;
		clickItems.push_back(fastSpeed3);

		ClickItem fastArmor1;
		fastArmor1.rect.top = 227;
		fastArmor1.rect.height = 20;
		fastArmor1.rect.left = LVL1_LEFT;
		fastArmor1.rect.width = 20;
		fastArmor1.action = FastArmor;
		fastArmor1.cost = 1;
		fastArmor1.level = 1;
		clickItems.push_back(fastArmor1);

		ClickItem fastArmor2;
		fastArmor2.rect.top = 227;
		fastArmor2.rect.height = 20;
		fastArmor2.rect.left = LVL2_LEFT;
		fastArmor2.rect.width = 20;
		fastArmor2.action = FastArmor;
		fastArmor2.cost = 2;
		fastArmor2.level = 2;
		clickItems.push_back(fastArmor2);

		ClickItem fastArmor3;
		fastArmor3.rect.top = 227;
		fastArmor3.rect.height = 20;
		fastArmor3.rect.left = LVL3_LEFT;
		fastArmor3.rect.width = 20;
		fastArmor3.action = FastArmor;
		fastArmor3.cost = 3;
		fastArmor3.level = 3;
		clickItems.push_back(fastArmor3);

		ClickItem fastShield1;
		fastShield1.rect.top = 251;
		fastShield1.rect.height = 20;
		fastShield1.rect.left = LVL1_LEFT;
		fastShield1.rect.width = 20;
		fastShield1.action = FastShield;
		fastShield1.cost = 1;
		fastShield1.level = 1;
		clickItems.push_back(fastShield1);

		ClickItem fastShield2;
		fastShield2.rect.top = 251;
		fastShield2.rect.height = 20;
		fastShield2.rect.left = LVL2_LEFT;
		fastShield2.rect.width = 20;
		fastShield2.action = FastShield;
		fastShield2.cost = 3;
		fastShield2.level = 2;
		clickItems.push_back(fastShield2);

		ClickItem fastShield3;
		fastShield3.rect.top = 251;
		fastShield3.rect.height = 20;
		fastShield3.rect.left = LVL3_LEFT;
		fastShield3.rect.width = 20;
		fastShield3.action = FastShield;
		fastShield3.cost = 5;
		fastShield3.level = 3;
		clickItems.push_back(fastShield3);

		//heavy
		ClickItem heavySpeed1;
		heavySpeed1.rect.top = 301;
		heavySpeed1.rect.height = 20;
		heavySpeed1.rect.left = LVL1_LEFT;
		heavySpeed1.rect.width = 20;
		heavySpeed1.action = HeavySpeed;
		heavySpeed1.cost = 2;
		heavySpeed1.level = 1;
		clickItems.push_back(heavySpeed1);

		ClickItem heavySpeed2;
		heavySpeed2.rect.top = 301;
		heavySpeed2.rect.height = 20;
		heavySpeed2.rect.left = LVL2_LEFT;
		heavySpeed2.rect.width = 20;
		heavySpeed2.action = HeavySpeed;
		heavySpeed2.cost = 2;
		heavySpeed2.level = 1;
		clickItems.push_back(heavySpeed2);

		ClickItem heavyArmor1;
		heavyArmor1.rect.top = 324;
		heavyArmor1.rect.height = 20;
		heavyArmor1.rect.left = LVL1_LEFT;
		heavyArmor1.rect.width = 20;
		heavyArmor1.action = HeavyArmor;
		heavyArmor1.cost = 1;
		heavyArmor1.level = 1;
		clickItems.push_back(heavyArmor1);

		ClickItem heavyArmor2;
		heavyArmor2.rect.top = 324;
		heavyArmor2.rect.height = 20;
		heavyArmor2.rect.left = LVL2_LEFT;
		heavyArmor2.rect.width = 20;
		heavyArmor2.action = HeavyArmor;
		heavyArmor2.cost = 2;
		heavyArmor2.level = 2;
		clickItems.push_back(heavyArmor2);

		ClickItem heavyArmor3;
		heavyArmor3.rect.top = 324;
		heavyArmor3.rect.height = 20;
		heavyArmor3.rect.left = LVL3_LEFT;
		heavyArmor3.rect.width = 20;
		heavyArmor3.action = HeavyArmor;
		heavyArmor3.cost = 3;
		heavyArmor3.level = 3;
		clickItems.push_back(heavyArmor3);

		ClickItem heavyArmor4;
		heavyArmor4.rect.top = 324;
		heavyArmor4.rect.height = 20;
		heavyArmor4.rect.left = LVL4_LEFT;
		heavyArmor4.rect.width = 20;
		heavyArmor4.action = HeavyArmor;
		heavyArmor4.cost = 3;
		heavyArmor4.level = 4;
		clickItems.push_back(heavyArmor4);

		ClickItem heavyShield1;
		heavyShield1.rect.top = 348;
		heavyShield1.rect.height = 20;
		heavyShield1.rect.left = LVL1_LEFT;
		heavyShield1.rect.width = 20;
		heavyShield1.action = HeavyShield;
		heavyShield1.cost = 1;
		heavyShield1.level = 1;
		clickItems.push_back(heavyShield1);

		ClickItem heavyShield2;
		heavyShield2.rect.top = 348;
		heavyShield2.rect.height = 20;
		heavyShield2.rect.left = LVL2_LEFT;
		heavyShield2.rect.width = 20;
		heavyShield2.action = HeavyShield;
		heavyShield2.cost = 1;
		heavyShield2.level = 2;
		clickItems.push_back(heavyShield2);

		ClickItem heavyShield3;
		heavyShield3.rect.top = 348;
		heavyShield3.rect.height = 20;
		heavyShield3.rect.left = LVL3_LEFT;
		heavyShield3.rect.width = 20;
		heavyShield3.action = HeavyShield;
		heavyShield3.cost = 2;
		heavyShield3.level = 3;
		clickItems.push_back(heavyShield3);

		ClickItem heavyShield4;
		heavyShield4.rect.top = 348;
		heavyShield4.rect.height = 20;
		heavyShield4.rect.left = LVL4_LEFT;
		heavyShield4.rect.width = 20;
		heavyShield4.action = HeavyShield;
		heavyShield4.cost = 3;
		heavyShield4.level = 4;
		clickItems.push_back(heavyShield4);
	}

	void StatePlayUpgrade::Cleanup()
	{
		printf("StatePlayUpgrade Cleanup\n");
	}

	void StatePlayUpgrade::Pause()
	{
		printf("StatePlayUpgrade Pause\n");
	}

	void StatePlayUpgrade::Resume()
	{
		printf("StatePlayUpgrade Resume\n");
	}

	void StatePlayUpgrade::HandleEvents(sf::RenderWindow& window)
	{
		sf::Event Event;

		while (window.pollEvent(Event))
		{
			
			if(Event.type == sf::Event::Closed)
			{
				Game::Quit();
			} else

			if(Event.type == sf::Event::MouseButtonPressed)
			{
				ClickItem _retu = HandleClick(Event.mouseButton.x,Event.mouseButton.y);
				
				if(_retu.action != Nothing)
				{
					if(_retu.action == Close) {
						std::cout << "DEBUG: Pressed Close\n";
						Drawer::SetUpgradeMenu(false);
						Game::ChangeState( StatePlay::Instance() );
					} else if(_retu.action == SimpleSpeed) {
						std::cout << "DEBUG: Pressed SimpleSpeed upgrade\n";
						UpgradeAttribute("Simple", "Speed", _retu);
					} else if(_retu.action == SimpleArmor) {
						UpgradeAttribute("Simple", "Armor", _retu);
					} else if(_retu.action == SimpleShield) {
						UpgradeAttribute("Simple", "Shield", _retu);
					} else if(_retu.action == FastSpeed) {
						UpgradeAttribute("Fast", "Speed", _retu);
					} else if(_retu.action == FastArmor) {
						UpgradeAttribute("Fast", "Armor", _retu);
					} else if(_retu.action == FastShield) {
						UpgradeAttribute("Fast", "Shield", _retu);
					} else if(_retu.action == HeavySpeed) {
						UpgradeAttribute("Heavy", "Speed", _retu);
					} else if(_retu.action == HeavyArmor) {
						UpgradeAttribute("Heavy", "Armor", _retu);
					} else if(_retu.action == HeavyShield) {
						UpgradeAttribute("Heavy", "Shield", _retu);
					}
				}
			} else

			if(Event.type == sf::Event::KeyPressed)
			{
				if(Event.key.code == sf::Keyboard::Escape) {
					std::cout << "DEBUG<StatePlayUpgrade>: ESC pressed\n";
					Game::ChangeState( StatePlay::Instance() );
				}
			}
		}
	}

	void StatePlayUpgrade::Update() 
	{
		
	}

	void StatePlayUpgrade::Draw(sf::RenderWindow& window) 
	{
		// Clear the screen (fill it with black color)
		window.clear(sf::Color::White);

		// Init drawer planes
		Drawer::InitDrawer(window);
		
		// Draw game map
		Game::GetTileManager()->Draw();

		// Draw the GameObjects
		Game::GetGameObjectManager()->Draw();

		// Draw HUD
		Drawer::DrawUpgradeHUD();

		// Combine draw planes
		Drawer::FinalizeDrawer(window);

		// Display window contents on screen
		window.display();
	}

	StatePlayUpgrade::ClickItem StatePlayUpgrade::HandleClick(int x, int y)
	{
		std::cout << "DEBUG<STatePlayUpgrade::HandleClick>: Click detected!\n";
		std::list<ClickItem>::iterator it;
		for ( it = clickItems.begin(); it != clickItems.end(); it++)
		{
			sf::Rect<int> clickItemRect = (*it).rect;
			if( clickItemRect.top + clickItemRect.height > y
				&& clickItemRect.top < y 
				&& clickItemRect.left < x 
				&& clickItemRect.left + clickItemRect.width > x)
				{
					return *it;
				}
		}

		ClickItem _retu;
		_retu.action = Nothing;
		return _retu;
	}

	void StatePlayUpgrade::UpgradeAttribute(std::string _unitType, std::string _attributeType, ClickItem _item)
	{
		Character* _ptr_unit = NULL;
		unsigned int _cost = _item.cost;
		
		if(_unitType == "Simple") {
			_ptr_unit = Game::GetBaseCSimple();
		} else if(_unitType == "Fast") {
			_ptr_unit = Game::GetBaseCFast();
		} else if(_unitType == "Heavy") {
			_ptr_unit = Game::GetBaseCHeavy();
		} else {
			std::cout << "DEBUG<StatePlayUpgrade::UpgradeAttribute>: ERROR! not such unitType(" << _unitType << ") defined!\n";
			return;
		}

		std::cout << "DEBUG: Pressed " << _unitType << " (uplevel = " << _ptr_unit->GetUpLevelSpeed() << ", lockedlevel = " << _ptr_unit->GetLockedUpLevelSpeed() << " cost = " << _cost << "\n";

		if(_attributeType == "Speed")
		{
			if(int(_ptr_unit->GetUpLevelSpeed()) < _item.level && Game::GetMoney() >= int(_cost)) {
				_ptr_unit->AddUpLevelSpeed(1);
				Game::ReduceMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, 1);
				_ptr_unit->SetSpeed(_ptr_unit->GetSpeed() + 1);
			} else if(_ptr_unit->GetUpLevelSpeed() == _item.level && _ptr_unit->GetLockedUpLevelSpeed() < _ptr_unit->GetUpLevelSpeed()) {
				_ptr_unit->ReduceUpLevelSpeed(1);
				Game::AddMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, -1);
				_ptr_unit->SetSpeed(_ptr_unit->GetSpeed() - 1);
			}
		} else if(_attributeType == "Armor") {
			if(int(_ptr_unit->GetUpLevelArmor()) < _item.level && Game::GetMoney() >= int(_cost)) {
				_ptr_unit->AddUpLevelArmor(1);
				Game::ReduceMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, 1);
				_ptr_unit->SetArmor(_ptr_unit->GetArmor() + 1);
			} else if(_ptr_unit->GetUpLevelArmor() == _item.level && _ptr_unit->GetLockedUpLevelArmor() < _ptr_unit->GetUpLevelArmor()) {
				_ptr_unit->ReduceUpLevelArmor(1);
				Game::AddMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, -1);
				_ptr_unit->SetArmor(_ptr_unit->GetArmor() - 1);
			}
		} else if(_attributeType == "Shield") {
			if(int(_ptr_unit->GetUpLevelShield()) < _item.level && Game::GetMoney() >= int(_cost)) {
				_ptr_unit->AddUpLevelShield(1);
				Game::ReduceMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, 1);
				_ptr_unit->SetShield(_ptr_unit->GetShield() + 1);
			} else if(_ptr_unit->GetUpLevelShield() == _item.level && _ptr_unit->GetLockedUpLevelShield() < _ptr_unit->GetUpLevelShield()) {
				_ptr_unit->ReduceUpLevelShield(1);
				Game::AddMoney(_cost);
				//upgrade all attackers in gameobjects
				Game::GetGameObjectManager()->Upgrade(_unitType, _attributeType, -1);
				_ptr_unit->SetShield(_ptr_unit->GetShield() - 1);
			}
		} else {
			std::cout << "DEBUG<StatePlayUpgrade::UpgradeAttribute>: ERROR! not such attribute type(" << _attributeType << ") defined!\n";
			return;
		}
	}
}