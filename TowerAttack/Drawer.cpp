#include "Drawer.h"
#include "StateMenu.h"
#include "Tower.h"
#include "stdafx.h"

namespace TA
{
	Drawer* Drawer::ptr_drawer;
	bool Drawer::instanceFlag;

	sf::RenderTexture Drawer::gamePlane;
	sf::RenderTexture Drawer::hudPlane;
	sf::RenderTexture Drawer::upgradePlane;

	bool Drawer::upgradeMenu;
	unsigned int Drawer::placement; // helps to draw units in the right position in attack panel

	sf::Sprite Drawer::spriteAtt1; // sprite for the object (visual presentation of the object)
	sf::Texture Drawer::imageAtt1;
	sf::Sprite Drawer::spriteAtt2; // sprite for the object (visual presentation of the object)
	sf::Texture Drawer::imageAtt2;
	sf::Sprite Drawer::spriteAtt3; // sprite for the object (visual presentation of the object)
	sf::Texture Drawer::imageAtt3;

	sf::Sprite Drawer::menuSprite;
	sf::Texture Drawer::menuImage;

	std::shared_ptr<sf::Text> Drawer::ptr_structure;
	std::shared_ptr<sf::Text> Drawer::ptr_armor;
	std::shared_ptr<sf::Text> Drawer::ptr_shield;
	std::shared_ptr<sf::Text> Drawer::ptr_speed;
	std::shared_ptr<sf::Text> Drawer::ptr_ammo;

	sf::RectangleShape Drawer::recInfo;
	sf::RectangleShape Drawer::recUnits;
	sf::RectangleShape Drawer::planArea;
	sf::RectangleShape Drawer::metaRec;
	sf::RectangleShape Drawer::attackRec;
	sf::Text Drawer::attackText;
	sf::RectangleShape Drawer::closeRec;
	std::shared_ptr<sf::Text> Drawer::ptr_text;
	sf::CircleShape Drawer::ball;
	sf::RectangleShape Drawer::upgRec;
	sf::CircleShape Drawer::shield;

	Drawer* Drawer::GetInstance()
	{
		if(!ptr_drawer) {
			ptr_drawer = new Drawer;
			instanceFlag = true;
		}

		return ptr_drawer;
	}

	void Drawer::Init()
	{
		if(debug) printf("Drawer Init\n");

		upgradeMenu = false;
		placement = 1;

		//if (!gamePlane.create(float(Game::getScreenWidth()), float(Game::getScreenHeigth())))
		if (!gamePlane.create(unsigned int(Game::getScreenWidth() * 2), unsigned int(Game::getScreenHeigth() * 2)))
				assert(false);
		if (!hudPlane.create(Game::getScreenWidth(), Game::getScreenHeigth()))
				assert(false);
		if (!upgradePlane.create(Game::getScreenWidth(), Game::getScreenHeigth()))
				assert(false);

		ptr_structure = std::make_shared<sf::Text>(Game::GetText());
		ptr_structure->setFillColor(sf::Color::Black);
		ptr_structure->setCharacterSize(16);
		ptr_armor = std::make_shared<sf::Text>(Game::GetText());
		ptr_armor->setFillColor(sf::Color::Red);
		ptr_armor->setCharacterSize(16);
		ptr_shield = std::make_shared<sf::Text>(Game::GetText());
		ptr_shield->setFillColor(sf::Color::Blue);
		ptr_shield->setCharacterSize(16);
		ptr_speed = std::make_shared<sf::Text>(Game::GetText());
		ptr_speed->setFillColor(sf::Color::Green);
		ptr_speed->setCharacterSize(16);
		ptr_ammo = std::make_shared<sf::Text>(Game::GetText());
		ptr_ammo->setFillColor(sf::Color::Red);
		ptr_ammo->setCharacterSize(18);

		metaRec.setSize(sf::Vector2f(12, 18));
		metaRec.setOutlineColor(sf::Color::Black);
		metaRec.setFillColor(sf::Color(255,255,255));
		metaRec.setOutlineThickness(1);

		planArea.setSize(sf::Vector2f(100, 480));
		planArea.setOutlineColor(sf::Color::Black);
		planArea.setFillColor(sf::Color(255,255,255));
		planArea.setOutlineThickness(1);
		planArea.setPosition(0, 40);

		attackText = Game::GetText();
		attackText.setString("Attack");
		attackText.setPosition(23, 473);
		attackRec.setSize(sf::Vector2f(80, 40));
		attackRec.setOutlineThickness(3);
		attackRec.setOutlineColor(sf::Color::Red);
		attackRec.setPosition(10, 465);

		shield.setOutlineColor(sf::Color::Blue);
		shield.setOutlineThickness(1.0f);
		shield.setFillColor(sf::Color::Transparent);
		shield.setRadius(40.0f);

		recInfo.setSize(sf::Vector2f(750, 30));
		recInfo.setOutlineColor(sf::Color::Black);
		recInfo.setFillColor(sf::Color(255,255,255));
		recInfo.setOutlineThickness(1);
		recInfo.setPosition(25, 0);

		recUnits.setSize(sf::Vector2f(750, 75));
		recUnits.setOutlineColor(sf::Color::Black);
		recUnits.setFillColor(sf::Color(255,255,255));
		recUnits.setOutlineThickness(1);
		recUnits.setPosition(25, 525);

		closeRec.setSize(sf::Vector2f(12, 15));
		closeRec.setOutlineColor(sf::Color::Black);
		closeRec.setFillColor(sf::Color(255,255,255));
		closeRec.setOutlineThickness(1);

		ptr_text = std::make_shared<sf::Text>(Game::GetText());
		ptr_text->setFillColor(sf::Color::Red);

		ball.setRadius(7.0f);
		ball.setOutlineColor(sf::Color::Black);
		ball.setFillColor(sf::Color::Black);

		upgRec.setSize(sf::Vector2f(100, 40));
		upgRec.setOutlineColor(sf::Color::Black);
		upgRec.setFillColor(sf::Color(255,255,255));
		upgRec.setOutlineThickness(1);
		upgRec.setPosition(300, 545);

		if(menuImage.loadFromFile("resources/images/upgrademenu.png") == false)
		{
			if(debug) std::cout << "DEBUG <GameObject::Init>: FAIL!\n";
		} else {
			menuSprite.setTexture(menuImage);
			if(debug) std::cout << "DEBUG <GameObject::Init>: SUCCESS!\n";
		}
		if(imageAtt1.loadFromFile("resources/images/texture_characterSimple.png") == false)
		{
			if(debug) std::cout << "DEBUG <GameObject::Init>: FAIL!\n";
		} else {
			spriteAtt1.setTexture(imageAtt1);
			if(debug) std::cout << "DEBUG <GameObject::Init>: SUCCESS!\n";
		}
		if(imageAtt2.loadFromFile("resources/images/texture_characterFast.png") == false)
		{
			if(debug) std::cout << "DEBUG <GameObject::Init>: FAIL!\n";
		} else {
			spriteAtt2.setTexture(imageAtt2);
			if(debug) std::cout << "DEBUG <GameObject::Init>: SUCCESS!\n";
		}
		if(imageAtt3.loadFromFile("resources/images/texture_characterHeavy.png") == false)
		{
			if(debug) std::cout << "DEBUG <GameObject::Init>: FAIL!\n";
		} else {
			spriteAtt3.setTexture(imageAtt3);
			if(debug) std::cout << "DEBUG <GameObject::Init>: SUCCESS!\n";
		}
	}

	void Drawer::Cleanup()
	{
		printf("Drawer Cleanup\n");
	}

	void Drawer::AddPlacement()
	{
		placement++;
	}

	void Drawer::SetUpgradeMenu(bool _value)
	{
		upgradeMenu = _value;
	}

	void Drawer::DrawCloseBox(sf::RenderTexture * _ptr_plane, unsigned int _x, unsigned int _y)
	{
		closeRec.setPosition(float(_x), float(_y));
		_ptr_plane->draw(closeRec);
		ptr_text->setString("X");
		ptr_text->setPosition(float(_x + 2), float(_y));
		ptr_text->setCharacterSize(13);
		ptr_text->setStyle(sf::Text::Bold);
		_ptr_plane->draw(*ptr_text);
	}

	void Drawer::DrawBall(unsigned int _x, unsigned int _y, bool _state)
	{
		ball.setPosition(float(_x), float(_y));
		if(_state)
			ball.setFillColor(sf::Color(sf::Color(111, 111, 111, 255)));
		else
			ball.setFillColor(sf::Color::Black);
		upgradePlane.draw(ball);
	}

	void Drawer::DrawUpgradeHUD()
	{
		DrawHUD();
		upgradeMenu = true;

		menuSprite.setPosition(228,10);
		upgradePlane.draw(menuSprite);
		
		DrawCloseBox(&upgradePlane, 515, 40);

		// Draw upgrade balls
		unsigned int _posX = 442;
		// Simple
		for(unsigned int i = 0; i < Game::GetBaseCSimple()->GetUpLevelSpeed(); i++) {
			DrawBall(_posX+i*24, 90, i < Game::GetBaseCSimple()->GetLockedUpLevelSpeed());
		}
		for(unsigned int i = 0; i < Game::GetBaseCSimple()->GetUpLevelArmor(); i++) {
			DrawBall(_posX+i*24, 114, i < Game::GetBaseCSimple()->GetLockedUpLevelArmor());
		}
		for(unsigned int i = 0; i < Game::GetBaseCSimple()->GetUpLevelShield(); i++) {
			DrawBall(_posX+i*24, 138, i < Game::GetBaseCSimple()->GetLockedUpLevelShield());
		}

		// Fast
		for(unsigned int i = 0; i < Game::GetBaseCFast()->GetUpLevelSpeed(); i++) {
			DrawBall(_posX+i*24, 206, i < Game::GetBaseCFast()->GetLockedUpLevelSpeed());
		}
		for(unsigned int i = 0; i < Game::GetBaseCFast()->GetUpLevelArmor(); i++) {
			DrawBall(_posX+i*24, 230, i < Game::GetBaseCFast()->GetLockedUpLevelArmor());
		}
		for(unsigned int i = 0; i < Game::GetBaseCFast()->GetUpLevelShield(); i++) {
			DrawBall(_posX+i*24, 254, i < Game::GetBaseCFast()->GetLockedUpLevelShield());
		}

		// Heavy
		for(unsigned int i = 0; i < Game::GetBaseCHeavy()->GetUpLevelSpeed(); i++) {
			DrawBall(_posX+i*24, 304, i < Game::GetBaseCHeavy()->GetLockedUpLevelSpeed());
		}
		for(unsigned int i = 0; i < Game::GetBaseCHeavy()->GetUpLevelArmor(); i++) {
			DrawBall(_posX+i*24, 328, i < Game::GetBaseCHeavy()->GetLockedUpLevelArmor());
		}
		for(unsigned int i = 0; i < Game::GetBaseCHeavy()->GetUpLevelShield(); i++) {
			DrawBall(_posX+i*24, 352, i < Game::GetBaseCHeavy()->GetLockedUpLevelShield());
		}
	}

	void Drawer::DrawHUD()
	{
		// Draw panel for information
		hudPlane.draw(recInfo);

		// Draw Turn/Phase information
		std::string _text = "Round " + std::to_string(Game::GetPlayRound());
		ptr_text->setString(_text);
		ptr_text->setCharacterSize(14);
		ptr_text->setPosition(40, 4);
		hudPlane.draw(*ptr_text);

		_text = "Turn " + std::to_string(Game::GetPlayTurn());
		ptr_text->setString(_text);
		ptr_text->setPosition(120, 4);
		hudPlane.draw(*ptr_text);

		_text = "Phase " + Game::GetPlayPhase();
		ptr_text->setString(_text);
		ptr_text->setPosition(220, 3);
		hudPlane.draw(*ptr_text);

		// Draw Money information
		_text = "Money " + std::to_string(Game::GetPlayer()->GetMoney());
		ptr_text->setString(_text);
		ptr_text->setPosition(465, 3);
		hudPlane.draw(*ptr_text);

		// Draw Score information
		_text = "Score " + std::to_string(Game::GetPlayer()->GetScore());
		ptr_text->setString(_text);
		ptr_text->setPosition(665, 3);
		hudPlane.draw(*ptr_text);

		// Draw plan phase stuff
		// Attacker select panel
		hudPlane.draw(planArea);

		// Draw attacker units in the panel
		std::vector<std::shared_ptr<GameObject>> _attackers = Game::GetGameObjectManager()->GetByType("Character");
		//if(debug) std::cout << "Attacker amount = " << _attackers.size() << "\n";
		for(std::vector<std::shared_ptr<GameObject>>::iterator it = _attackers.begin(); it != _attackers.end(); it++) {
			std::shared_ptr<Character> _ita = std::dynamic_pointer_cast<Character>(*it);
			if(!_ita->GetActivity()) {
				if(_ita->GetPlacement() >= Game::GetPlayTurn()) {
					if(_ita->GetType() == "CharacterSimple") {
						spriteAtt1.setScale(0.6f, 0.6f);
						spriteAtt1.setPosition(23, float(static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70)));
						hudPlane.draw(spriteAtt1);

						if(Game::GetPlayPhase() == "plan") {
							DrawCloseBox(&hudPlane, 70, float(static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70)));
						}
					} else if(_ita->GetType() == "CharacterFast") {
						spriteAtt2.setScale(0.6f, 0.6f);
						spriteAtt2.setPosition(23, float(static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70)));
						hudPlane.draw(spriteAtt2);
							
						if(Game::GetPlayPhase() == "plan") {
							DrawCloseBox(&hudPlane, 70, float(static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70)));
						}
					} else if(_ita->GetType() == "CharacterHeavy") {
						spriteAtt3.setScale(0.6f, 0.6f);
						spriteAtt3.setPosition(23, float(static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70)));
						hudPlane.draw(spriteAtt3);
							
						if(Game::GetPlayPhase() == "plan") {
							DrawCloseBox(&hudPlane, 70, static_cast<unsigned int>((_ita->GetPlacement() - placement)*60 + 70));
						}
					} else {
						if(debug) std::cout << "DEBUG: ERROR in drawing unit in attack panel\n";
					}
				}
			}
		}

		// Ready button
		if(Game::GetPlayPhase() == "plan") {
			UpdateAttackText();
			hudPlane.draw(attackRec);
			hudPlane.draw(attackText);
		}

		// Draw panel for buying units
		if(Game::GetPlayPhase() == "plan") {
			hudPlane.draw(recUnits);

			// Draw attacker units in the panel
			// Draw sprite (Simple)
			spriteAtt1.setScale(0.6f, 0.6f);
			spriteAtt1.setPosition(40, 535);
			hudPlane.draw(spriteAtt1);

			// Cost in attack points
			_text = "1";
			ptr_text->setCharacterSize(14);
			ptr_text->setString(_text);
			ptr_text->setPosition(43, 537);
			hudPlane.draw(*ptr_text);

			// Draw meta data
			DrawMetaSpeed(&hudPlane, Game::GetBaseCSimple()->GetSpeed(), sf::Vector2f(46, 576));
			DrawMetaShield(&hudPlane, Game::GetBaseCSimple()->GetShield(), sf::Vector2f(58, 576));
			DrawMetaArmor(&hudPlane, Game::GetBaseCSimple()->GetArmor(), sf::Vector2f(70, 576));
			DrawMetaStructure(&hudPlane, Game::GetBaseCSimple()->GetStructure(), sf::Vector2f(82, 576));

			// Draw sprite (Fast)
			spriteAtt2.setScale(0.6f, 0.6f);
			spriteAtt2.setPosition(115, 535);
			hudPlane.draw(spriteAtt2);
			// Cost in attack points
			_text = "2";
			ptr_text->setString(_text);
			ptr_text->setPosition(118, 537);
			hudPlane.draw(*ptr_text);

			// Draw meta data
			DrawMetaSpeed(&hudPlane, Game::GetBaseCFast()->GetSpeed(), sf::Vector2f(121, 576));
			DrawMetaShield(&hudPlane, Game::GetBaseCFast()->GetShield(), sf::Vector2f(133, 576));
			DrawMetaArmor(&hudPlane, Game::GetBaseCFast()->GetArmor(), sf::Vector2f(145, 576));
			DrawMetaStructure(&hudPlane, Game::GetBaseCFast()->GetStructure(), sf::Vector2f(157, 576));

			// Draw sprite (Heavy)
			spriteAtt3.setScale(0.6f, 0.6f);
			spriteAtt3.setPosition(190, 535);
			hudPlane.draw(spriteAtt3);
			
			_text = "3";
			ptr_text->setString(_text);
			ptr_text->setPosition(193, 537);
			hudPlane.draw(*ptr_text);
			
			// Draw meta data
			DrawMetaSpeed(&hudPlane, Game::GetBaseCHeavy()->GetSpeed(), sf::Vector2f(196, 576));
			DrawMetaShield(&hudPlane, Game::GetBaseCHeavy()->GetShield(), sf::Vector2f(208, 576));
			DrawMetaArmor(&hudPlane, Game::GetBaseCHeavy()->GetArmor(), sf::Vector2f(220, 576));
			DrawMetaStructure(&hudPlane, Game::GetBaseCHeavy()->GetStructure(), sf::Vector2f(232, 576));

			// Attack points
			_text = "Attack points " + std::to_string(Game::GetAttackPoints());
			ptr_text->setString(_text);
			ptr_text->setCharacterSize(16);
			ptr_text->setPosition(600, 530);
			hudPlane.draw(*ptr_text);

			// Money
			_text = "Money " + std::to_string(Game::GetMoney());
			ptr_text->setString(_text);
			ptr_text->setCharacterSize(16);
			ptr_text->setPosition(600, 550);
			if(upgradeMenu)
				upgradePlane.draw(*ptr_text);
			else
				hudPlane.draw(*ptr_text);

			hudPlane.draw(upgRec);
			_text = "Round " + std::to_string(Game::GetPlayRound());
			ptr_text->setString("Upgrade");
			ptr_text->setPosition(310, 553);
			ptr_text->setCharacterSize(20);
			ptr_text->setStyle(sf::Text::Bold);
			hudPlane.draw(*ptr_text);
		}

		
	}

	void Drawer::DrawAttacker(std::shared_ptr<Character> _attacker)
	{
		gamePlane.draw(_attacker->GetSprite());
		if(_attacker->GetShield() > 0) {
			// Draw shield
			shield.setPosition(_attacker->GetPosition().x-40.0f, _attacker->GetPosition().y-40.0f);
			gamePlane.draw(shield);
		}

		// Draw meta data
		DrawMetaSpeed(&gamePlane, _attacker->GetSpeed(), sf::Vector2f(_attacker->GetPosition().x-23, _attacker->GetPosition().y+13));
		DrawMetaShield(&gamePlane, _attacker->GetShield(), sf::Vector2f(_attacker->GetPosition().x-11, _attacker->GetPosition().y+13));
		DrawMetaArmor(&gamePlane, _attacker->GetArmor(), sf::Vector2f(_attacker->GetPosition().x+1, _attacker->GetPosition().y+13));
		DrawMetaStructure(&gamePlane, _attacker->GetStructure(), sf::Vector2f(_attacker->GetPosition().x+13, _attacker->GetPosition().y+13));
	}

	void Drawer::DrawTower(std::shared_ptr<Tower> _tower)
	{
		if(debug) std::cout << "Draw tower x = " << _tower->GetPosition().x << " y = " << _tower->GetPosition().y << "\n";
		gamePlane.draw(_tower->GetSprite());
		
		unsigned int _ammo = _tower->GetAmmoCount();
		unsigned int _clipSize = _tower->GetClipSize();

		// Draw meta data
		// Ammo information
		std::string _turretInfo = std::to_string(_ammo) + "/" + std::to_string(_clipSize);
		ptr_ammo->setString(_turretInfo);
		ptr_ammo->setPosition(_tower->GetPosition().x+25, _tower->GetPosition().y+25);
		gamePlane.draw(*ptr_ammo.get());

		// Loading info
		if(_ammo >= _clipSize && _tower->GetMetaText().getString() != "LOADING")
		{
			_tower->UpdateMetaText(_tower->GetPosition().x-40, _tower->GetPosition().y-10, "LOADING");
		}
	}

	void Drawer::DrawGameObject(std::shared_ptr<GameObject> _object)
	{
		gamePlane.draw(_object->GetSprite());
	}

	void Drawer::DrawText(std::shared_ptr<sf::Text> _text)
	{
		gamePlane.draw(*_text);
	}

	void Drawer::DrawMetaStructure(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos)
	{
		metaRec.setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(metaRec);

		ptr_structure->setString(std::to_string(_value));
		ptr_structure->setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(*ptr_structure);
	}

	void Drawer::DrawMetaArmor(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos)
	{
		metaRec.setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(metaRec);

		ptr_armor->setString(std::to_string(_value));
		ptr_armor->setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(*ptr_armor);
	}

	void Drawer::DrawMetaShield(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos)
	{
		metaRec.setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(metaRec);

		ptr_shield->setString(std::to_string(_value));
		ptr_shield->setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(*ptr_shield);
	}

	void Drawer::DrawMetaSpeed(sf::RenderTexture * _ptr_plane, unsigned int _value, sf::Vector2f _pos)
	{
		metaRec.setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(metaRec);

		ptr_speed->setString(std::to_string(_value));
		ptr_speed->setPosition(_pos.x, _pos.y);
		_ptr_plane->draw(*ptr_speed);
	}

	void Drawer::DrawTile(std::shared_ptr<TileRoad> _tile)
	{
		gamePlane.draw(_tile->GetSprite());
		// Draw tile data on tile
		std::shared_ptr<sf::Text> _ptr_text = std::make_shared<sf::Text>(Game::GetText());
		unsigned int _points = _tile->GetPoints();
		if(_points > 0) {
			_ptr_text->setString(std::to_string(_points));
			_ptr_text->setPosition(_tile->GetSpritePosition().x+20, _tile->GetSpritePosition().y+30);
			gamePlane.draw(*_ptr_text.get());
		}
	}

	void Drawer::ResetPlacement()
	{
		placement = 1;
	}

	void Drawer::InitDrawer(sf::RenderWindow& window)
	{
		gamePlane.clear(sf::Color::White);
		hudPlane.clear(sf::Color::Transparent);
		upgradePlane.clear(sf::Color::Transparent);
	}

	void Drawer::FinalizeDrawer(sf::RenderWindow& window)
	{
		gamePlane.display();
		sf::Sprite gameSprite(gamePlane.getTexture());
		window.draw(gameSprite);
		
		//Set view to default for drawing hud into correct place
		hudPlane.display();
		sf::Sprite hudSprite(hudPlane.getTexture());
		window.setView(window.getDefaultView());

		if(upgradeMenu) {
			upgradePlane.display();
			sf::Sprite upgradeSprite(upgradePlane.getTexture());
			Game::GetEffect(0).Update(hudPlane);
			window.draw(hudSprite, Game::GetEffect(0).getShader());
			window.draw(upgradeSprite);
		} else {
			window.draw(hudSprite);
		}
	}

	void Drawer::UpdateAttackText()
	{
		if(Game::GetAttackPoints() != 0) {
			attackRec.setOutlineColor(sf::Color::Black);
			attackRec.setOutlineThickness(1);
			attackText.setCharacterSize(9);
			attackText.setString("Use all\nattack points");
			attackText.setPosition(19, 474);
		} else {
			attackRec.setOutlineColor(sf::Color::Red);
			attackRec.setOutlineThickness(2);
			attackText.setCharacterSize(18);
			attackText.setString("Attack");
			attackText.setPosition(23, 473);
		}
	}
}