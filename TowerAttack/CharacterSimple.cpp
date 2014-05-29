#include "CharacterSimple.h"
#include "Game.h" // We are using methods from the Game class
#include "Projectile.h" // We are creating a new instance of this class
#include "SaveGameManager.h"

namespace TA
{
	CharacterSimple::CharacterSimple()
	{
		ready = false;
		travelledTiles = 0;
		active = false;
		placement = 0;
		structure = 1;
		armor = 0;
		shield.baseValue = 0;
		shield.currentValue = 0;
		speed = 1;
		cost = 1;
		arrived = true;
		gameTurn = Game::GetPlayTurn();
		activePhase = "attack";
		rotation = 0.0f;
		moveClock.restart();
		Load("resources/images/texture_characterSimple.png");
		assert(IsLoaded());
		// Set origin, so rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		targetPosition = GetPosition();
		unsigned int _tileWidth = Game::GetTileManager()->GetTileWidth();
		targetPosition.x = targetPosition.x / _tileWidth;
		targetPosition.y = targetPosition.y / _tileWidth;
		std::cout << "DEBUG <CharacterSimple::Character()>: Character initialized\n";
	}
		
	CharacterSimple::CharacterSimple(unsigned int _placement)
	{
		ready = false;
		travelledTiles = 0;
		active = false;
		placement = _placement;
		structure = Game::GetBaseCSimple()->GetStructure();
		armor = Game::GetBaseCSimple()->GetArmor();
		shield.baseValue = Game::GetBaseCSimple()->GetShield();
		shield.currentValue = shield.baseValue;
		speed = Game::GetBaseCSimple()->GetSpeed();
		std::cout << "Speed set to " << speed << "\n";
		cost = 1;
		arrived = true;
		gameTurn = Game::GetPlayTurn();
		activePhase = "attack";
		rotation = 0.0f;
		moveClock.restart();
		Load("resources/images/texture_characterSimple.png");
		assert(IsLoaded());
		// Set origin, so rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		GetSprite().setScale(0.6f, 0.6f);
		targetPosition = GetPosition();
		unsigned int _tileWidth = Game::GetTileManager()->GetTileWidth();
		targetPosition.x = targetPosition.x / _tileWidth;
		targetPosition.y = targetPosition.y / _tileWidth;
		std::cout << "DEBUG <CharacterSimple::Character()>: Character initialized\n";
	}

	CharacterSimple::~CharacterSimple()
	{
	}

	CharacterSimple::CharacterSimple(std::istream & is):Character(is) // Read data for the parent
	{
		std::cout << "DEBUG <CharacterSimple::Character(is)>: ***** Loading data! *****\n";
		is.read((char*)&rotation, sizeof(float)); // Read rotation
		GetSprite().setRotation(rotation);
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		is.read((char*)&maxVelocity, sizeof(float)); // Read maxVelocity
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		float velX, velY;
		is.read((char*)&velX, sizeof(float)); // Read velocity.x
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading velocity.x: " + std::to_string(velX) + "\n";
		is.read((char*)&velY, sizeof(float)); // Read velocity.Y
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading velocity.y: " + std::to_string(velY) + "\n";
		velocity.x = velX;
		velocity.y = velY;

		is.read((char*)&targetPosition.x, sizeof(float)); // Read targetPosition.x
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading targetPosition.x: " + std::to_string(targetPosition.x) + "\n";
		is.read((char*)&targetPosition.y, sizeof(float)); // Read targetPosition.y
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Reading targetPosition.y: " + std::to_string(targetPosition.y) + "\n";
	
		lastDirection = SaveGameManager::ReadString(is);
		std::cout << "DEBUG <CharacterSimple::Character(is)>: Read last direction: " + lastDirection + "\n";

		// Set origin, so the rotation works like it should...
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		std::cout << "DEBUG <CharacterSimple::Character(is)>: ***** Data loaded! *****\n";
	}

	void CharacterSimple::serialise(std::ostream & os) // Write the data into the file
	{
		std::cout << "DEBUG <CharacterSimple::serialise>: ***Saving Character!\n";
		Character::serialise(os);

		os.write((char*)&rotation, sizeof(float)); // Save rotation
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&maxVelocity, sizeof(float)); // Save maxVelocity
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		float velX = velocity.x;
		float velY = velocity.y;
		os.write((char*)&velX, sizeof(float)); // Save velocity.x
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved velocity.x: " + std::to_string(velocity.x) + "\n";
		os.write((char*)&velY, sizeof(float)); // Save velocity.y
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved velocity.y: " + std::to_string(velocity.y) + "\n";

		os.write((char*)&targetPosition.x, sizeof(float)); // Write targetPosition.x
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved targetPosition.x: " + std::to_string(targetPosition.x) + "\n";
		os.write((char*)&targetPosition.y, sizeof(float)); // Write targetPosition.y
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved targetPosition.y: " + std::to_string(targetPosition.y) + "\n";

		SaveGameManager::SerialiseString(os, lastDirection); // Write lastDirection
		std::cout << "DEBUG <CharacterSimple::serialise>: Saved last direction: " + lastDirection + "\n";

		std::cout << "DEBUG <CharacterSimple::serialise>: ***Data saved!\n";
	}


	/*void CharacterSimple::Update(float elapsedTime)
	{
		if(!active) 
		{ // && placement == 0 && ) {
			active = (placement <= Game::GetPlayTurn());
			if(active) std::cout << "ACTIVATED attacker(simple)" << placement << " placement = " << placement << " game turn = " << Game::GetPlayTurn() << "\n";
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

						std::cout << "speed = travelledTiles: " << speed << " == " << travelledTiles << "\n";
						if(speed == travelledTiles)
						{
							std::cout << "READY\n";
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
					std::cout << "DEBUG<Character>: Start the journey (towards the next tile)!\n";

					// Make sure the character object is located at the center of the target tile
					SetPosition(targetPosition.x*_tileWidth-50, targetPosition.y*_tileWidth-50);
					bool _next = SetNextDirection(_location);
				}
			}

			// Move the sprite
			if (velocity.x != 0.0f || velocity.y != 0.0f)
				GetSprite().move(sf::Vector2f(velocity.x * elapsedTime, velocity.y * elapsedTime));
		}
	}*/

	std::string CharacterSimple::GetType()
	{
		return "CharacterSimple";
	}
}