#include "Turret.h"

namespace TA
{
	Turret::Turret()
	{
		activePhase = "always";
		rotation = 0.0f;
		SetDeleteStatus(false);
		Load("resources/images/texture_turret.png");
		assert(IsLoaded());
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		std::cout << "DEBUG <Turret::Turret()>: Turret initialized\n";
	}

	Turret::Turret(float _x, float _y)
	{
		activePhase = "always";
		rotation = 0.0f;
		SetDeleteStatus(false);
		Load("resources/images/texture_turret.png");
		assert(IsLoaded());
		//std::cout << "DEBUG <Turret::Turret(_x, _y)>: height = " << GetSprite().getGlobalBounds().height << " width = " << GetSprite().getGlobalBounds().width << "\n";
		//std::cout << "DEBUG <Turret::Turret(_x, _y)>: Set position x = " << _x << " y = " << _y << "\n";
		
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		SetPosition(_x, _y);
		/*
		// Turn turret towards player character
		// Delta between x and y axis
		sf::Vector2f turretPos = GetPosition();
		sf::Vector2f targetPos = Game::GetGameObjectManager()->Get("Character_0_")->GetPosition();
		float deltaX = targetPos.x - turretPos.x;
		float deltaY = targetPos.y - turretPos.y;
		// Calculate the angle
		rotation = float(atan2(deltaY, deltaX) * 180 / M_PI);
		*/
		GetSprite().setRotation(rotation);

		std::cout << "DEBUG <Turret::Turret(_x, _y)>: Turret initialized\n";
	}

	Turret::~Turret()
	{
	}

	Turret::Turret(std::istream & is):GameObject(is) // Read the data from the file
	{
		std::cout << "DEBUG <Turret::Turret(is)>: ***** Loading data! *****\n";
		SetDeleteStatus(false);
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		is.read((char*)&rotation, sizeof(float)); // Read rotation
		GetSprite().setRotation(rotation);
		std::cout << "DEBUG <Turret::Turret(is)>: Reading rotation: " + std::to_string(rotation) + "\n";

		std::cout << "DEBUG <Turret::Turret(is)>: ***** Data loaded! *****\n";
	}

	void Turret::serialise(std::ostream & os) // Write the data into the file
	{
		std::cout << "DEBUG <Turret::serialise>: ***Saving Turret!\n";
		GameObject::serialise(os);

		os.write((char*)&rotation, sizeof(float)); // Save rotation
		std::cout << "DEBUG <Turret::serialise>: Saving rotation: " + std::to_string(rotation) + "\n";

		std::cout << "DEBUG <Turret::serialise>: ***Data saved!\n";
	}

	void Turret::Update(float elapsedTime)
	{
		//std::cout << "Turret update\n";
		// If no target defined, check if there's some target...
		if(target.expired())
			SelectClosestTarget();

		// Turn turret towards player character
		// Delta between x and y axis
		if(!target.expired())
		{
			//std::cout << "Turret update2\n";
			sf::Vector2f _turretPos = GetPosition();
			// Follow the target defined for the tower
			sf::Vector2f _targetPos = target._Get()->GetPosition();
			float deltaX = _targetPos.x - _turretPos.x;
			float deltaY = _targetPos.y - _turretPos.y;

			// Calculate the angle
			rotation = float(atan2(deltaY, deltaX) * 180 / M_PI);
		}

		GetSprite().setRotation(rotation);
	}

	std::string Turret::GetType()
	{
		return "Turret";
	}

	void Turret::SetTarget(std::weak_ptr<GameObject> _target)
	{
		target = _target;

		// Turn turret towards player character
		// Delta between x and y axis
		if(!target.expired())
		{
			//std::cout << "Turret update2\n";
			sf::Vector2f _turretPos = GetPosition();
			// Follow the target defined for the tower
			sf::Vector2f _targetPos = target._Get()->GetPosition();
			float deltaX = _targetPos.x - _turretPos.x;
			float deltaY = _targetPos.y - _turretPos.y;

			// Calculate the angle
			rotation = float(atan2(deltaY, deltaX) * 180 / M_PI);
		}

		GetSprite().setRotation(rotation);
	}

	void Turret::SelectClosestTarget()
	{
		// There's no valid target defined for the tower, go through the attackers & check if some of them is in range
		std::vector<std::shared_ptr<GameObject>> _targets = Game::GetGameObjectManager()->GetByType("Character");
		std::vector<std::shared_ptr<GameObject>>::iterator itr = _targets.begin();

		// Go through the targets & select the closest (active) one
		std::weak_ptr<GameObject> _selected;
		if(_targets.size() > 0) {
			itr = _targets.begin();
			// Jump over inactive targets
			while(itr != _targets.end() && !std::dynamic_pointer_cast<Character>(*itr)->GetActivity())
				itr++;

			// If no active targets were found, reset target and return
			if(itr == _targets.end()) {
				if(_selected.expired())
					target.reset();
				return;
			}

			// Some active target found, go on...
			_selected = *itr;
				
			// Check the distance to the first attacker
			sf::Vector2f _targetPos = itr->get()->GetPosition();
			float deltaX = abs(_targetPos.x - GetPosition().x);
			float deltaY = abs(_targetPos.y - GetPosition().y);
			float _dist = sqrt(deltaX*deltaX + deltaY*deltaY);
			// Jump to the next attacker
			itr++;

			// Go through the other attackers in range & select the closest one
			while(itr != _targets.end()) {
				// Check the distance
				_targetPos = itr->get()->GetPosition();
				deltaX = abs(_targetPos.x - GetPosition().x);
				deltaY = abs(_targetPos.y - GetPosition().y);

				if(sqrt(deltaX*deltaX + deltaY*deltaY) < _dist) {
					_selected = *itr;
					_dist = sqrt(deltaX*deltaX + deltaY*deltaY);
				}

				itr++;
			}
		}

		if(!_selected.expired()) {
			target = _selected;
		} else {
			target.reset();
		}
	}
}