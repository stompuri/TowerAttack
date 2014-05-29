#include "Tile.h"

#include "SaveGameManager.h"

namespace TA
{
	Tile::Tile(void) : isLoaded(false)
	{
	}


	Tile::~Tile(void)
	{
	}

	Tile::Tile(std::istream & is) : isLoaded(false)
	{
		float spriteX, spriteY;
		is.read((char*)&spriteX, sizeof(float)); // Read sprite.position.x
		//std::cout << "DEBUG <Tile(is)>: Reading sprite position.x: " + std::to_string(spriteX) + "\n";
		is.read((char*)&spriteY, sizeof(float)); // Read sprite.position.x
		//std::cout << "DEBUG <Tile(is)>: Reading sprite position.y: " + std::to_string(spriteY) + "\n";

		//std::cout << "DEBUG <Tile(is)>: Reading file name\n";
		filename = SaveGameManager::ReadString(is);
		//std::cout << "\nDEBUG <Tile(is)>: File name read:\n" + filename + "\n";

		// Read the movable information
		// TODO
		is.read((char*)&movableLeft, sizeof(bool));
		//std::cout << "DEBUG <Tile(is)>: Reading movable(left): " + std::to_string(movableLeft) + "\n";
		is.read((char*)&movableDown, sizeof(bool));
		//std::cout << "DEBUG <Tile(is)>: Reading movable(down): " + std::to_string(movableDown) + "\n";
		is.read((char*)&movableRight, sizeof(bool));
		//std::cout << "DEBUG <Tile(is)>: Reading movable(right): " + std::to_string(movableRight) + "\n";
		is.read((char*)&movableUp, sizeof(bool));
		//std::cout << "DEBUG <Tile(is)>: Reading movable(up): " + std::to_string(movableUp) + "\n";

		Load(filename);
		assert(IsLoaded());
		SetSpritePosition(spriteX, spriteY);

		std::cout << "DEBUG <Tile(is)>: Data recovered (" << spriteX << ", " << spriteY << ")\n";
	}

	// Save the data to the file
	void Tile::serialise(std::ostream & os) // Write the data into the file
	{
		float spriteX = sprite.getPosition().x;
		float spriteY = sprite.getPosition().y;
		os.write((char*)&spriteX, sizeof(float)); // Save sprite.position.x
		//std::cout << "DEBUG <Tile::serialise>: Saving sprite position.x: " + std::to_string(spriteX) + "\n";
		os.write((char*)&spriteY, sizeof(float)); // Save sprite.position.y
		//std::cout << "DEBUG <Tile::serialise>: Saving sprite position.y: " + std::to_string(spriteY) + "\n";
	
		//std::cout << "DEBUG <Tile::serialise>: Saving filename: '" + filename + "'\n";
		SaveGameManager::SerialiseString(os, filename);
		//std::cout << "\nDEBUG <Tile::serialise>: Filename saved\n";

		// Save the movable information
		os.write((char*)&movableLeft, sizeof(bool));
		//std::cout << "DEBUG <Tile::serialise>: Saving tile movable information(left): " + std::to_string(movableLeft) + "\n";
		os.write((char*)&movableDown, sizeof(bool));
		//std::cout << "DEBUG <Tile::serialise>: Saving tile movable information(down): " + std::to_string(movableDown) + "\n";
		os.write((char*)&movableRight, sizeof(bool));
		//std::cout << "DEBUG <Tile::serialise>: Saving tile movable information(right): " + std::to_string(movableRight) + "\n";
		os.write((char*)&movableUp, sizeof(bool));
		//std::cout << "DEBUG <Tile::serialise>: Saving tile movable information(up): " + std::to_string(movableUp) + "\n";

		std::cout << "DEBUG <Tile::serialise>: All data serialized (" << spriteX << ", " << spriteY << ")\n";
	}

	// Load texture from a file
	void Tile::Load(std::string _filename)
	{
		if(image.loadFromFile(_filename) == false)
		{
			std::cout << "DEBUG <Tile::Load>: FAIL!\n";
			filename = "";
			isLoaded = false;
		}
		else
		{
			filename = _filename;
			sprite.setTexture(image);
			isLoaded = true;
			//std::cout << "DEBUG <Tile::Load>: SUCCESS!\n";
		}
	}

	void Tile::SetSpritePosition(float x, float y)
	{
		if(isLoaded)
		{
			//std::cout << "DEBUG <Tile::SetPosition>: Setting position " + std::to_string(x) + "/" + std::to_string(y) + "\n";
			sprite.setPosition(x,y);
		} else
			std::cout << "DEBUG <Tile::SetPosition>: FAIL!\n";
	}

	sf::Vector2f Tile::GetSpritePosition() const
	{
		if(isLoaded)
		{
			return sprite.getPosition();
		}
		return sf::Vector2f();
	}

	sf::Sprite& Tile::GetSprite()
	{
		return sprite;
	}

	bool Tile::IsLoaded() const
	{
		return isLoaded;
	}

	void Tile::setRotation(float _angle)
	{
		rotation = _angle;
		GetSprite().setRotation(rotation);
	}

	bool Tile::GetMovable(std::string _direction)
	{
		if (_direction == "up" && movableUp)
			return true;
		else if (_direction == "down" && movableDown)
			return true;
		else if (_direction == "left" && movableLeft)
			return true;
		else if (_direction == "right" && movableRight)
			return true;
		//std::cout << "DEBUG <Tile::GetMovable>: Tile not movable to " << _direction << "\n";
		return false;
	}

	void Tile::SetMovableDown(bool _value)
	{
		movableDown = _value;
	}
	void Tile::SetMovableUp(bool _value)
	{
		movableUp = _value;
	}
	void Tile::SetMovableLeft(bool _value)
	{
		movableLeft = _value;
	}
	void Tile::SetMovableRight(bool _value)
	{
		movableRight = _value;
	}
	bool* Tile::GetMovableDirections()
	{
		bool* pointer = new bool[4];
		pointer[0] = movableLeft;
		pointer[1] = movableDown;
		pointer[2] = movableRight;
		pointer[3] = movableUp;
		return pointer;
	}
}