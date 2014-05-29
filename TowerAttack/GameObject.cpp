#include "GameObject.h"

#include "SaveGameManager.h"

namespace TA
{
	GameObject::GameObject() : isLoaded(false), toBeDeleted(false), velocity(0.0f, 0.0f), maxVelocity(2000.0f)
	{
	}

	GameObject::~GameObject()
	{
	}

	// Read the data from the file
	GameObject::GameObject(std::istream & is) : isLoaded(false), toBeDeleted(false)
	{
		// Read id
		is.read((char*)&id, sizeof(unsigned int));
		if(debug) std::cout << "DEBUG <GameObject(is)>: Reading objects id: " << id << "\n";

		float spriteX, spriteY;
		is.read((char*)&spriteX, sizeof(float));
		if(debug) std::cout << "DEBUG <GameObject(is)>: Reading sprite position.x: " + std::to_string(spriteX) + "\n";
		is.read((char*)&spriteY, sizeof(float));
		if(debug) std::cout << "DEBUG <GameObject(is)>: Reading sprite position.y: " + std::to_string(spriteY) + "\n";

		if(debug) std::cout << "DEBUG <GameObject(is)>: Reading file name\n";
		filename = SaveGameManager::ReadString(is);
		if(debug) std::cout << "\nDEBUG <GameObject(is)>: File name read:\n" + filename + "\n";

		Load(filename);
		assert(IsLoaded());
		SetPosition(spriteX, spriteY);

		if(debug) std::cout << "DEBUG <GameObject(is)>: Data recovered!\n";
	}

	// Save the game data as binary into a file
	void GameObject::serialise(std::ostream & os)
	{
		// Save objects ID
		os.write((char*)&id, sizeof(unsigned int));
		if(debug) std::cout << "DEBUG <GameObject::serialise(is)>: Saving objects id " << id << "\n";

		float spriteX = sprite.getPosition().x;
		float spriteY = sprite.getPosition().y;
		os.write((char*)&spriteX, sizeof(float));
		if(debug) std::cout << "DEBUG <GameObject::serialise>: Saving sprite position.x: " + std::to_string(spriteX) + "\n";
		os.write((char*)&spriteY, sizeof(float));
		if(debug) std::cout << "DEBUG <GameObject::serialise>: Saving sprite position.y: " + std::to_string(spriteY) + "\n";
	
		if(debug) std::cout << "DEBUG <GameObject::serialise>: Saving filename: '" + filename + "'\n";
		SaveGameManager::SerialiseString(os, filename);
		if(debug) std::cout << "\nDEBUG <GameObject::serialise>: Filename saved\n";

		if(debug) std::cout << "DEBUG <GameObject::serialise>: All data serialized!\n";
	}

	// Load texture from a file
	void GameObject::Load(std::string _filename)
	{
		if(image.loadFromFile(_filename) == false)
		{
			if(debug) std::cout << "DEBUG <GameObject::Load>: FAIL!\n";
			toBeDeleted = true;
			filename = "";
			isLoaded = false;
		}
		else
		{
			toBeDeleted = false;
			filename = _filename;
			sprite.setTexture(image);
			isLoaded = true;
			if(debug) std::cout << "DEBUG <GameObject::Load>: SUCCESS! Filename = " << filename << "\n";
		}
	}

	void GameObject::SetPosition(float x, float y)
	{
		if(isLoaded)
		{
			//if(debug) std::cout << "DEBUG <GameObject::SetPosition>: Setting position " + std::to_string(x) + "/" + std::to_string(y) + "\n";
			sprite.setPosition(x,y);
		} else
			if(debug) std::cout << "DEBUG <GameObject::SetPosition>: FAIL!\n";
	}

	sf::Vector2f GameObject::GetPosition() const
	{
		if(isLoaded)
		{
			return sprite.getPosition();
		}
		return sf::Vector2f();
	}

	sf::Sprite& GameObject::GetSprite()
	{
		return sprite;
	}

	bool GameObject::IsLoaded() const
	{
		return isLoaded;
	}

	bool GameObject::GetDeleteStatus() const
	{
		return toBeDeleted;
	}

	void GameObject::SetDeleteStatus(bool status)
	{
		toBeDeleted = status;
	}

	sf::Vector2f GameObject::GetVelocity() const
	{
		return velocity;
	}

	float GameObject::GetRotation() const
	{
		return rotation;
	}

	void GameObject::SetVelocity(sf::Vector2f _velocity)
	{
		velocity = _velocity;
	}

	void GameObject::SetRotation(float _rotation)
	{
		rotation = _rotation;
	}

	void GameObject::SetObjectId(unsigned int _id)
	{
		id = _id;
		if(debug) std::cout << "DEBUG<GameObject::SetObjectId>: id set to " << id << "\n";
	}

	unsigned int GameObject::GetObjectId() const
	{
		return id;
	}

	std::string GameObject::GetActivePhase()
	{
		return activePhase;
	}

	bool GameObject::GetReadiness()
	{
		return ready;
	}

	void GameObject::SetReadiness(bool _value)
	{
		ready = _value;
	}
}