#include "Projectile.h"
#include "Game.h"
#include "stdafx.h"
#include <cassert>

namespace TA
{
	Projectile::Projectile()
	{
		activePhase = "always";
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		maxVelocity = 600.0f;
		SetDeleteStatus(false);
		Load("resources/images/texture_ammo.png");
		assert(IsLoaded());
		GetSprite().scale(0.5f, 0.5f);
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		std::cout << "DEBUG<Projectile::Projectile(): Projectile initialized";
	}

	Projectile::Projectile(sf::Vector2f _position, float _rotation, unsigned int _parentId)
	{
		activePhase = "always";
		// Set velocity according to rotation
		double _x = 0;
		double _y = 0;
		_x = 500.0f * cos(abs(_rotation) * M_PI / 180.0f);
		_y = 500.0f * sin(abs(_rotation) * M_PI / 180.0f);
		if (_rotation < 0)
		{
			_y = -(_y);
		}
		velocity.x = float(_x);
		velocity.y = float(_y);

		parentId = _parentId;
		maxVelocity = 600.0f;
		rotation = _rotation;
		SetRotation(rotation);
		SetDeleteStatus(false);
		Load("resources/images/texture_ammo.png");
		assert(IsLoaded());
		GetSprite().scale(0.5f, 0.5f);
		GetSprite().setPosition(_position.x, _position.y);
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		std::cout << "DEBUG<Projectile::Projectile(sf::Vector2f, float): Projectile initialized. VelX = " << velocity.x << " VelY = " << velocity.y << "\n";
	}

	Projectile::~Projectile()
	{
	}

	Projectile::Projectile(std::istream & is):GameObject(is) // Read the data for the parent
	{
		std::cout << "DEBUG <Projectile::Projectile(is)>: ***Loading data!\n";
		is.read((char*)&rotation, sizeof(float)); // Read rotation
		GetSprite().setRotation(rotation);
		std::cout << "DEBUG <Projectile::Projectile(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		is.read((char*)&maxVelocity, sizeof(float)); // Read maxVelocity
		std::cout << "DEBUG <Projectile::Projectile(is)>: Reading maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		float velX, velY;
		is.read((char*)&velX, sizeof(float)); // Read velocity.x
		std::cout << "DEBUG <Projectile::Projectile(is)>: Reading velocity.x: " + std::to_string(velX) + "\n";
		is.read((char*)&velY, sizeof(float)); // Read velocity.Y
		std::cout << "DEBUG <Projectile::Projectile(is)>: Reading velocity.y: " + std::to_string(velY) + "\n";
		velocity.x = velX;
		velocity.y = velY;

		// Read parentId (who shot this projectile)
		is.read((char*)&parentId, sizeof(unsigned int));
		std::cout << "DEBUG <Projectile::Projectile(is)>: Reading parentId: " << parentId << "\n";

		GetSprite().scale(0.5f, 0.5f);
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		std::cout << "DEBUG <Projectile::Projectile(is)>: ***Data loaded!\n";
	}

	void Projectile::serialise(std::ostream & os) // Write the data into the file
	{
		std::cout << "DEBUG <Projectile::serialise>: ***Saving data!\n";
		GameObject::serialise(os);
		os.write((char*)&rotation, sizeof(float)); // Save rotation
		std::cout << "DEBUG <Projectile::serialise>: Saving rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&maxVelocity, sizeof(float)); // Save maxVelocity
		std::cout << "DEBUG <Projectile::serialise>: Saving maxVelocity: " + std::to_string(maxVelocity) + "\n";
		// sf::Vector2f is handled in two parts
		os.write((char*)&velocity.x, sizeof(float)); // Save velocity.x
		std::cout << "DEBUG <Projectile::serialise>: Saving velocity.x: " + std::to_string(velocity.x) + "\n";
		os.write((char*)&velocity.y, sizeof(float)); // Save velocity.y
		std::cout << "DEBUG <Projectile::serialise>: Saving velocity.y: " + std::to_string(velocity.y) + "\n";

		// Save parentId (who shot this projectile)
		os.write((char*)&parentId, sizeof(unsigned int));
		std::cout << "DEBUG <Projectile::Projectile(is)>: Saving parentId\n";

		std::cout << "DEBUG <Projectile::serialise>: ***Data saved!\n";
	}

	void Projectile::Update(float elapsedTime)
	{
		//std::cout << "DEBUG<Projectile::Update>\n";
		if (GetDeleteStatus())
			return;
		else
		{
			sf::Vector2f _location = this->GetPosition();
			//std::cout << "DEBUG<Projectile::Update> parentId = " << parentId << "\n";
			std::shared_ptr<GameObject> _shotBy = Game::GetGameObjectManager()->Get(parentId);
			//std::cout << "DEBUG<Projectile::Update>2\n";
			unsigned int _x = int(floor((_location.x+100.0f) / 100.0f));
			unsigned int _y = int(floor((_location.y+100.0f) / 100.0f));
			unsigned int _parentX = int(floor((_shotBy->GetPosition().x+100.0f) / 100.0f));
			unsigned int _parentY = int(floor((_shotBy->GetPosition().y+100.0f) / 100.0f));

			// COLLISSION DETECTION
			// Check if the projectile hits some game object (it goes over some tile/object which blocks its movement)			
			//std::cout << "DEBUG parentId = " << parentId << " _shotBy.type = " << _shotBy->GetType() << " _x = " << _x << " _y = " << _y << " parentX = " << _parentX << " parentY = " << _parentY << "\n"; 
			// Check that the tile has left the parent tile (so it doesn't hit the turret/character that shot it)
			if( _x != _parentX || _y != _parentY)
			{
				std::cout << "DEBUG _x = " << _x << " _y = " << _y << " parentX = " << _parentX << " parentY = " << _parentY << "\n"; 
				// Check if the projectile hits any valid game object
				std::vector<std::shared_ptr<GameObject>> _objects = Game::GetGameObjectManager()->GetByPosition(_location.x, _location.y);
				if(_objects.size() > 0)
				{
					// Not valid hits: 1) Object & same type of objects that shot this projectile, 2) Turret-objects
					// Go through _objects and check for valid hits
					for(std::vector<std::shared_ptr<GameObject>>::iterator it = _objects.begin(); it != _objects.end(); ++it)
					{
						if (it->get()->GetType() != "Projectile" && // Don't hit projectiles
							it->get()->GetType() != _shotBy->GetType() && // Don't hit the type you were shot from
							it->get()->GetType() != "Turret") // Don't hit the tower turret
						{
							std::cout << "DEBUG HIT! parentId = " << parentId << " type = " << it->get()->GetType() << " _shotBy.type = " << _shotBy->GetType() << "\n";
							SetDeleteStatus(true);

							// Inform attacker of the hit with damage parameter
							std::shared_ptr<Character> _ptr_attacker = std::dynamic_pointer_cast<Character>(*it);
							_ptr_attacker->TakeHit(1);

							return;
						}
					}
					// TODO Check if the projectile hits unpassable tile
					
				}
			}

			// mark projectile to be deleted if goes out of the game boundaries
			if(_location.x  + GetSprite().getGlobalBounds().width / 2 > 2 * Game::getScreenWidth() ||
				_location.x  - GetSprite().getGlobalBounds().width / 2 < -1 * Game::getScreenWidth() ||
				_location.y  + GetSprite().getGlobalBounds().height / 2 > 2* Game::getScreenHeigth() ||
				_location.y  - GetSprite().getGlobalBounds().height / 2 < -1 * Game::getScreenWidth() )
			{
				SetDeleteStatus(true);
				std::cout << "OUT OF BOuNDS parentID = " << parentId << "\n";
				return;
			}

			GetSprite().move(sf::Vector2f(velocity.x * elapsedTime, velocity.y * elapsedTime));
		}
	}

	std::string Projectile::GetType()
	{
		return "Projectile";
	}
}