#include "Character.h"

#include "SaveGameManager.h"
#include "Game.h" // We are using methods from the Game class
#include "Projectile.h" // We are creating a new instance of this class
#include "Drawer.h" // Adds placement for the units

namespace TA
{
	Character::Character()
	{
		ulSpeed.lockedLevel = 0;
		ulSpeed.totalLevel = 0;
		ulArmor.lockedLevel = 0;
		ulArmor.totalLevel = 0;
		ulShield.lockedLevel = 0;
		ulShield.totalLevel = 0;
	}

	/*Character::Character()
	{
		active = false;
		placement = 0;
		structure = 2;
		armor = 0;
		shield = 0;
		speed = 1;
		arrived = true;
		gameTurn = Game::GetPlayTurn();
		activePhase = "attack";
		rotation = 0.0f;
		moveClock.restart();
		Load("resources/images/texture_player.png");
		assert(IsLoaded());
		// Set origin, so rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		targetPosition = GetPosition();
		unsigned int _tileWidth = Game::GetTileManager()->GetTileWidth();
		targetPosition.x = targetPosition.x / _tileWidth;
		targetPosition.y = targetPosition.y / _tileWidth;
		if(debug) std::cout << "DEBUG <Character::Character()>: Character initialized\n";
	}

		
	Character::Character(unsigned int _placement)
	{
		active = false;
		placement = _placement;
		structure = 2;
		armor = 0;
		shield = 0;
		speed = 1;
		arrived = true;
		gameTurn = Game::GetPlayTurn();
		activePhase = "attack";
		rotation = 0.0f;
		moveClock.restart();
		Load("resources/images/texture_player.png");
		assert(IsLoaded());
		// Set origin, so rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		targetPosition = GetPosition();
		unsigned int _tileWidth = Game::GetTileManager()->GetTileWidth();
		targetPosition.x = targetPosition.x / _tileWidth;
		targetPosition.y = targetPosition.y / _tileWidth;
		if(debug) std::cout << "DEBUG <Character::Character()>: Character initialized\n";
	}*/

	Character::~Character()
	{
	}

	Character::Character(std::istream & is):GameObject(is) // Read data for the parent
	{
		if(debug) std::cout << "DEBUG <Character::Character(is)>: ***** Loading data! *****\n";
		is.read((char*)&rotation, sizeof(float)); // Read rotation
		GetSprite().setRotation(rotation);
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		is.read((char*)&maxVelocity, sizeof(float)); // Read maxVelocity
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		float velX, velY;
		is.read((char*)&velX, sizeof(float)); // Read velocity.x
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading velocity.x: " + std::to_string(velX) + "\n";
		is.read((char*)&velY, sizeof(float)); // Read velocity.Y
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading velocity.y: " + std::to_string(velY) + "\n";
		velocity.x = velX;
		velocity.y = velY;

		is.read((char*)&targetPosition.x, sizeof(float)); // Read targetPosition.x
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading targetPosition.x: " + std::to_string(targetPosition.x) + "\n";
		is.read((char*)&targetPosition.y, sizeof(float)); // Read targetPosition.y
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Reading targetPosition.y: " + std::to_string(targetPosition.y) + "\n";
	
		lastDirection = SaveGameManager::ReadString(is);
		if(debug) std::cout << "DEBUG <Character::Character(is)>: Read last direction: " + lastDirection + "\n";

		// Set origin, so the rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		if(debug) std::cout << "DEBUG <Character::Character(is)>: ***** Data loaded! *****\n";
	}

	void Character::serialise(std::ostream & os) // Write the data into the file
	{
		if(debug) std::cout << "DEBUG <Character::serialise>: ***Saving Character!\n";
		GameObject::serialise(os);

		os.write((char*)&rotation, sizeof(float)); // Save rotation
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&maxVelocity, sizeof(float)); // Save maxVelocity
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		float velX = velocity.x;
		float velY = velocity.y;
		os.write((char*)&velX, sizeof(float)); // Save velocity.x
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved velocity.x: " + std::to_string(velocity.x) + "\n";
		os.write((char*)&velY, sizeof(float)); // Save velocity.y
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved velocity.y: " + std::to_string(velocity.y) + "\n";

		os.write((char*)&targetPosition.x, sizeof(float)); // Write targetPosition.x
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved targetPosition.x: " + std::to_string(targetPosition.x) + "\n";
		os.write((char*)&targetPosition.y, sizeof(float)); // Write targetPosition.y
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved targetPosition.y: " + std::to_string(targetPosition.y) + "\n";

		SaveGameManager::SerialiseString(os, lastDirection); // Write lastDirection
		if(debug) std::cout << "DEBUG <Character::serialise>: Saved last direction: " + lastDirection + "\n";

		if(debug) std::cout << "DEBUG <Character::serialise>: ***Data saved!\n";
	}

	void Character::SetTargetPosition(unsigned int _x, unsigned int _y)
	{
		targetPosition.x = float(_x);
		targetPosition.y = float(_y);
	}

	void Character::TakeHit(unsigned int _damage)
	{
		unsigned int _leftDamage = _damage;
		if(debug) std::cout << "Left damage = " << _leftDamage << " shield = " << shield.currentValue << "\n";

		// 1) Reduce the shield according to the damage taken
		if(shield.currentValue > 0)
		{
			if(_damage > shield.currentValue)
			{
				_leftDamage -= shield.currentValue;
				shield.currentValue = 0;
			} else {
				_leftDamage = 0;
				shield.currentValue -= _damage;
			}
		}
		if(debug) std::cout << "Left damage = " << _leftDamage << " shield = " << shield.currentValue << " amor = " << armor << "\n";

		// 2) Reduce the armor according to the left damage
		if(_leftDamage > 0 && armor > 0)
		{
			if(_leftDamage > armor)
			{
				_leftDamage -= armor;
				armor = 0;
			} else {
				armor -= _leftDamage;
				_leftDamage = 0;
			}
		}
		if(debug) std::cout << "Left damage = " << _leftDamage << " armor = " << armor << " structure = " << structure << "\n";

		// 3) Reduce the structure according to the left damage
		if(armor <= 0)
			structure -= _leftDamage;

		if(structure <= 0) {
			SetDeleteStatus(true);
			// Check the cell where the attacker died and increase score accordingly
			sf::Vector2f _location = this->GetPosition();
			unsigned int _x = int(floor((_location.x+100.0f) / 100.0f));
			unsigned int _y = int(floor((_location.y+100.0f) / 100.0f));
			std::shared_ptr<TileRoad> _ptr_tile = std::dynamic_pointer_cast<TileRoad>(Game::GetTileManager()->Get(_x, _y));
			Game::GetPlayer()->AddScore(_ptr_tile->GetPoints());
			if(debug) std::cout << "DEBUG <Character::TakeHit> Score = " << Game::GetPlayer()->GetScore() << "\n";
		}
	}

	bool Character::GetActivity()
	{
		return active;
	}

	unsigned int Character::GetPlacement()
	{
		return placement;
	}

	unsigned int Character::GetStructure()
	{
		return structure;
	}

	unsigned int Character::GetArmor()
	{
		return armor;
	}

	unsigned int Character::GetShield()
	{
		return shield.currentValue;
	}


	unsigned int Character::GetSpeed()
	{
		return speed;
	}

	void Character::SetStructure(unsigned int _value)
	{
		structure = _value;
	}
	
	void Character::SetArmor(unsigned int _value)
	{
		armor = _value;
	}
	
	void Character::SetShield(unsigned int _value)
	{
		shield.baseValue = _value;
		shield.currentValue = shield.baseValue;
	}

	void Character::RechargeShield(unsigned int _value)
	{
		shield.currentValue += _value;
		if(shield.currentValue > shield.baseValue)
			shield.currentValue = shield.baseValue;
	}

	void Character::SetSpeed(unsigned int _value)
	{
		speed = _value;
	}

	void Character::SetTravelled(unsigned int _value)
	{
		travelledTiles = _value;
	}

	unsigned int Character::GetTravelled()
	{
		return travelledTiles;
	}

	unsigned int Character::GetCost()
	{
		return cost;
	}

	bool Character::GetArrived()
	{
		return arrived;
	}

	sf::Vector2f Character::GetTarget()
	{
		return targetPosition;
	}

	void Character::Update(float elapsedTime)
	{
		if(!active) 
		{ // && placement == 0 && ) {
			active = (placement <= Game::GetPlayTurn());
			if(active) {
				if(debug) std::cout << "ACTIVATED attacker, placement = " << placement << " game turn = " << Game::GetPlayTurn() << "\n";
				Drawer::AddPlacement();
			}
		}

		if(active) // No more waiting in the bay
		{
			sf::Vector2f _location = GetPosition();
			unsigned int _tileWidth = Game::GetTileManager()->GetTileWidth();
			_location.x = (_location.x+50)/_tileWidth;
			_location.y = (_location.y+50)/_tileWidth;

			if (gameTurn <= Game::GetPlayTurn()) // Next turn in the game, do some action
				//moveClock.getElapsedTime().asSeconds() > 0.3f) // Player can move on tile in 0.3sec
			{
				if (!arrived)
				{
					if(	targetPosition.x == _location.x && targetPosition.y == _location.y || // we have reached the target position
						velocity.x > 0 && targetPosition.x < _location.x || // we have moved over the target and moving away
						velocity.x < 0 && targetPosition.x > _location.x || // we have moved over the target and moving away
						velocity.y > 0 && targetPosition.y < _location.y || // we have moved over the target and moving away
						velocity.y < 0 && targetPosition.y > _location.y) // we have moved over the target and moving away
					{
						velocity.x = 0;
						velocity.y = 0;
						travelledTiles++;
						// Make sure the character object is located at the center of the target tile
						SetPosition(targetPosition.x*_tileWidth-50, targetPosition.y*_tileWidth-50);

						//commented1 if(debug) std::cout << "DEBUG<" << GetObjectId() << "> speed = travelledTiles: " << speed << " == " << travelledTiles << "\n";
						if(speed == travelledTiles)
						{
							if(debug) std::cout << "READY\n";
							if(shield.currentValue < shield.baseValue)
								RechargeShield();
							travelledTiles = 0;
							arrived = true; // We have arrived to next tile
							gameTurn++; // We'll move when the game turn reaches this...
							ready = true; // We're ready until the next turn
						} else {
							// FIND OUT THE NEXT DIRECTION & SET TARGET POSITION
							bool _next = SetNextDirection(_location);
						}
					}
				} else if(Game::GetGoMark() && gameTurn <= Game::GetPlayTurn()) {
					// WE'RE AT THE START TILE & GET THE START SIGNAL->START THE JOURNEY!
					//commented1 if(debug) std::cout << "DEBUG<Character " << GetObjectId() << ">: Start the journey (towards the next tile)!\n";

					// Make sure the character object is located at the center of the target tile
					SetPosition(targetPosition.x*_tileWidth-50, targetPosition.y*_tileWidth-50);
					bool _next = SetNextDirection(_location);
				}
			}

			// Move the sprite
			if (velocity.x != 0.0f || velocity.y != 0.0f)
				GetSprite().move(sf::Vector2f(velocity.x * elapsedTime, velocity.y * elapsedTime));
		}
	}

	bool Character::SetNextDirection(sf::Vector2f _location)
	{
		_location.x = float(floor(_location.x + 0.5));
		_location.y = float(floor(_location.y + 0.5));
		//commented1 if(debug) std::cout << "DEBUG <Character::SetNextDirection> x = " << _location.x << " y = " << _location.y << "\n";
		velocity.x = 0.0f;
		velocity.y = 0.0f;

		// Begin movement towards the next tile
		bool* _ptr_dirs = NULL;
		try {
			_ptr_dirs = Game::GetTileManager()->Get(int(targetPosition.x), int(targetPosition.y))->GetMovableDirections();
		} catch (std::exception& e) {
			if(debug) std::cout << "Exception <Character::SetNextDirection>: " << e.what() << "\n";
			Game::Quit();
		}
				
		// Count the amount of possible directions (if more than one, randomise where to go next)
		std::vector<std::string> _directions;
		std::string _direction = "";
		for(int i=0; i<4; i++)
		{
			if(_ptr_dirs[i])
			{
				switch(i)
				{
				case 0: 
					if(lastDirection != "left")
						_directions.push_back("left");
					break;
				case 1: 
					if(lastDirection != "down")
						_directions.push_back("down");
					break;
				case 2: 
					if(lastDirection != "right")
						_directions.push_back("right");
					break;
				case 3: 
					if(lastDirection != "up")
						_directions.push_back("up");
				}
						
			}
		}

		if(_directions.size() > 1) // more than one possible direction-->randomise one
		{
			unsigned int _randomiser = rand() % 2; // 2 possible directions
			if(_directions.size() > 2) { // 3 possible directions
				_randomiser = rand() % 3;
			}
			_direction = _directions.at(_randomiser);
		} else if(_directions.size() == 1) {
			_direction = _directions.front();
		} else { // No directions found --> we are at the end tile --> do the actions
			SetDeleteStatus(true);
			Game::GetPlayer()->AddScore(15);
			return false;
		}

		if(_direction == "left")
		{
			if(rotation != 180.0f) {
				rotation = 180.0f;
				GetSprite().setRotation(rotation);
			}

			//if(debug) std::cout << "DE: posX = " << GetSprite().getPosition().x << " posY = " << GetSprite().getPosition().x << " targetX = " << targetPosition.x << " targetY = " << targetPosition.y << "\n";
			if (Game::GetGameObjectManager()->CheckMovable((targetPosition.x-1)*100.0f, targetPosition.y*100.0f, this) &&
				Game::GetTileManager()->CheckMovable(int(_location.x), int(_location.y), "left") )
			{
				targetPosition.x -= 1;
				velocity.x = -50.0f * speed;
				lastDirection = "right";
			}

			moveClock.restart();
			//commented1 if(debug) std::cout << "Character: Pressed LEFT, speed = " << speed << "velocity = " << velocity.x << " rotation = " << rotation << "\n";
			//commented1 if(debug) std::cout << "DE: posX = " << GetSprite().getPosition().x << " posY = " << GetSprite().getPosition().x << " targetX = " << targetPosition.x << " targetY = " << targetPosition.y << "\n";
		}
		else if(_direction == "right")
		{
			if (rotation != 0.0f) {
				rotation = 0.0f;
				GetSprite().setRotation(rotation);
			}

			if (Game::GetGameObjectManager()->CheckMovable((targetPosition.x+1)*100.0f, targetPosition.y*100.0f, this) &&
				Game::GetTileManager()->CheckMovable(int(_location.x), int(_location.y), "right") )
			{
				targetPosition.x += 1;
				velocity.x = 50.0f * speed;
				lastDirection = "left";
			}
						
			moveClock.restart();
			//commented1 if(debug) std::cout << "DE: posX = " << GetSprite().getPosition().x << " posY = " << GetSprite().getPosition().x << " targetX = " << targetPosition.x << " targetY = " << targetPosition.y << "\n";
			//commented1 if(debug) std::cout << "DEBUG <Character>: Pressed RIGHT, speed = " << speed << " velocity = " << velocity.x << " rotation = " << rotation << "\n";
		}
		else if(_direction == "up")
		{
			if (rotation != 270.0f) {
				rotation = 270.0f;
				GetSprite().setRotation(rotation);
			}

			if (Game::GetGameObjectManager()->CheckMovable(targetPosition.x*100.0f, (targetPosition.y-1)*100.0f, this) &&
				Game::GetTileManager()->CheckMovable(int(_location.x), int(_location.y), "up") )
			{
				targetPosition.y -= 1;
				velocity.y = -50.0f * speed;
				lastDirection = "down";
			}

			moveClock.restart();
			//commented1 if(debug) std::cout << "DE: posX = " << GetSprite().getPosition().x << " posY = " << GetSprite().getPosition().x << " targetX = " << targetPosition.x << " targetY = " << targetPosition.y << "\n";
			//commented1 if(debug) std::cout << "DE: location.x = " << _location.x << " location.y = " << _location.y << "\n";
			//commented1 if(debug) std::cout << "DEBUG <Character>: Pressed UP, speed = " << speed << " velocity = " << velocity.y << " rotation = " << rotation << "\n";
		}
		else if(_direction == "down")
		{
			if (rotation != 90.0f) {
				rotation = 90.0f;
				GetSprite().setRotation(rotation);
			}

			if (Game::GetGameObjectManager()->CheckMovable(targetPosition.x*100.0f, (targetPosition.y+1)*100.0f, this) &&
				Game::GetTileManager()->CheckMovable(int(_location.x), int(_location.y), "down") )
			{
				targetPosition.y += 1;
				velocity.y = 50.0f * speed;
				lastDirection = "up";
			}
			moveClock.restart();
			//commented1 if(debug) std::cout << "Character: Pressed DOWN, speed = " << speed << " velocity = " << velocity.y << " rotation = " << rotation << "\n";
		}
		// New target position found & set
		arrived = false;
		return true;
	}

	void Character::SetPlacement(unsigned int _value)
	{
		placement = _value;
	}

	unsigned int Character::GetUpLevelSpeed()
	{
		return ulSpeed.totalLevel;
	}

	unsigned int Character::GetLockedUpLevelSpeed()
	{
		return ulSpeed.lockedLevel;
	}

	unsigned int Character::GetUpLevelArmor()
	{
		return ulArmor.totalLevel;
	}

	unsigned int Character::GetLockedUpLevelArmor()
	{
		return ulArmor.lockedLevel;
	}

	unsigned int Character::GetUpLevelShield()
	{
		return ulShield.totalLevel;
	}

	unsigned int Character::GetLockedUpLevelShield()
	{
		return ulShield.lockedLevel;
	}

	void Character::AddUpLevelSpeed(unsigned int _value)
	{
		ulSpeed.totalLevel += _value;
	}

	void Character::ReduceUpLevelSpeed(unsigned int _value)
	{
		ulSpeed.totalLevel -= _value;
	}

	void Character::AddUpLevelArmor(unsigned int _value)
	{
		ulArmor.totalLevel += _value;
	}

	void Character::ReduceUpLevelArmor(unsigned int _value)
	{
		ulArmor.totalLevel -= _value;
	}

	void Character::AddUpLevelShield(unsigned int _value)
	{
		ulShield.totalLevel += _value;
	}

	void Character::ReduceUpLevelShield(unsigned int _value)
	{
		ulShield.totalLevel -= _value;
	}

	void Character::ResetUpLevels()
	{
		ulArmor.totalLevel = 0;
		ulArmor.lockedLevel = 0;
		ulShield.totalLevel = 0;
		ulShield.lockedLevel = 0;
		ulSpeed.totalLevel = 0;
		ulSpeed.lockedLevel = 0;
	}

	void Character::LockUpLevels()
	{
		ulSpeed.lockedLevel = ulSpeed.totalLevel;
		ulArmor.lockedLevel = ulArmor.totalLevel;
		ulShield.lockedLevel = ulShield.totalLevel;
	}

	void Character::SetMetaText(sf::Text _text)
	{
		metaText = _text;
	}

	sf::Text Character::GetMetaText()
	{
		return metaText;
	}
}