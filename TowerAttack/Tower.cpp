#include "Tower.h"
#include "Turret.h"

namespace TA
{
	Tower::Tower()
	{
		ready = false;
		clipSize = 2;
		gameTurn = Game::GetPlayTurn();
		activePhase = "defend";
		ammoCount = 0;
		shootFreq = 2.0f;
		shootDistance = 150.0f;
		rotation = 0.0f;
		shootClock.restart();
		SetDeleteStatus(false);
		Load("resources/images/texture_tower.png");
		assert(IsLoaded());

		metaText = Game::GetText();
		metaText.setColor(sf::Color::Red);
		metaText.setCharacterSize(18);
		metaText.setString("");

		//std::cout << "DEBUG <Tower::Tower()>: height = " << GetSprite().getGlobalBounds().height << " width = " << GetSprite().getGlobalBounds().width << "\n";
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		turret = std::make_shared<Turret>();
		turret->SetObjectId(Game::GetGameObjectManager()->Add("Turret", turret));

		std::cout << "DEBUG <Tower::Tower()>: Tower initialized\n";
	}

	Tower::Tower(float _x, float _y)
	{
		ready = false;
		clipSize = 2;
		gameTurn = Game::GetPlayTurn();
		activePhase = "defend";
		ammoCount = 0;
		shootFreq = 2.0f;
		shootDistance = 150.0f;
		rotation = 0.0f;
		shootClock.restart();
		SetDeleteStatus(false);
		Load("resources/images/texture_tower.png");
		assert(IsLoaded());

		metaText = Game::GetText();
		metaText.setColor(sf::Color::Red);
		metaText.setCharacterSize(18);
		metaText.setString("");
		
		std::cout << "DEBUG <Tower::Tower()>: height = " << GetSprite().getGlobalBounds().height << " width = " << GetSprite().getGlobalBounds().width << "\n";
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		SetPosition(_x, _y);
		turret = std::make_shared<Turret>(_x, _y);
		turret->SetObjectId(Game::GetGameObjectManager()->Add("Turret", turret));
		turretId = turret->GetObjectId();

		std::cout << "DEBUG <Tower::Tower(_x, _y)>: Tower initialized with turret id: " << turretId << "\n";
	}


	Tower::~Tower()
	{
	}

	Tower::Tower(std::istream & is):GameObject(is) // Read the data from the file
	{
		std::cout << "DEBUG <Tower::Tower(is)>: ***** Loading data! *****\n";

		is.read((char*)&ammoCount, sizeof(int)); // Read ammoCount
		std::cout << "DEBUG <Tower::Tower(is)>: Reading ammoCount: " + std::to_string(ammoCount) + "\n";
		is.read((char*)&rotation, sizeof(float)); // Read rotation
		std::cout << "DEBUG <Tower::Tower(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		GetSprite().setRotation(rotation);

		// Read turretId
		is.read((char*)&turretId, sizeof(unsigned int));
		std::cout << "DEBUG <Tower::Tower(is)>: Reading turretId: " << turretId << "\n";

		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		shootClock.restart();
		SetDeleteStatus(false);
		std::cout << "DEBUG <Tower::Tower(is)>: ***** Data loaded! *****\n";
	}

	void Tower::serialise(std::ostream & os) // Write the data into the file
	{
		std::cout << "DEBUG <Tower::serialise>: ***Saving Tower!\n";
		GameObject::serialise(os);

		std::cout << "DEBUG <Tower::serialise>: Saving ammoCount: " + std::to_string(ammoCount) + "\n";
		os.write((char*)&ammoCount, sizeof(int)); // Save ammoCount
		
		std::cout << "DEBUG <Tower::serialise>: Saving rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&rotation, sizeof(float)); // Save rotation

		// Save turretId (who shot this projectile)
		os.write((char*)&turretId, sizeof(unsigned int));
		std::cout << "DEBUG <Tower::serialise(os)>: Saving turretId " << turretId << "\n";
		
		std::cout << "DEBUG <Tower::serialise>: ***Data saved!\n";
	}

	void Tower::Update(float elapsedTime)
	{
		//std::cout << "DEBUG<Tower::Update>: rotation = " << turret->GetRotation() << "\n";
		// Shoot ammo towards player if player is close enough & shoot frequency allows
		// 1) Check if current target in range
		CheckTargetDistance();

		// 2) Select a possible new target, if no valid target selected
		SelectNewTarget();

		// 3) Shoot
		if ( !target.expired() && // Check for possible targets and shoot if there's such in range
			 !ready // Turret has not done an action for this turn
			) {
			if(ammoCount >= clipSize) {
				Reload();
			} else {
				sf::Vector2f _ammoPosition = GetPosition();
				//std::cout << "DEBUG<Tower::Update>: target = " << target._Get()->GetType() << " target.x = " << target._Get()->GetPosition().x << " target.y = " << target._Get()->GetPosition().y << "\n";
				std::shared_ptr<Projectile> _projectile(new Projectile( _ammoPosition, turret->GetRotation(), id));
				_projectile->SetObjectId(Game::GetGameObjectManager()->Add("Projectile", _projectile));
				ammoCount++;

				std::cout << "DEBUG<Tower::Update>: Turret shot an ammo, rotation = " << turret->GetRotation() << " ammoCount = " << ammoCount << "\n";
				std::cout << "DEBUG<Tower::Update>: Turret shot an ammo, id = " << id << " turretId = " << turret->GetObjectId() << "\n";
			}
		} else if(!ready) {
			Reload();
		}

		ready = true;
	}

	std::string Tower::GetType()
	{
		return "Tower";
	}

	void Tower::SetPosition(float _x, float _y)
	{
		if(IsLoaded())
		{
			std::cout << "DEBUG <Tower::SetPosition>: Setting position " + std::to_string(_x) + "/" + std::to_string(_y) + "\n";
			GetSprite().setPosition(_x, _y);
			if(turret != NULL && turret->IsLoaded())
				turret->GetSprite().setPosition(_x, _y);
		} else
			std::cout << "DEBUG <GameObject::SetPosition>: FAIL!\n";
	}

	void Tower::SetTurret(std::shared_ptr<GameObject> _turret)
	{
		turret = _turret;
	}

	unsigned int Tower::GetTurretId()
	{
		//std::cout << "DEBUG <Tower::GetTurredId()> turretId = " << turretId << "\n";
		return turretId;
	}

	float Tower::CheckTargetDistance()
	{
		// Check if there's valid target defined. Return -1 if not
		if(target.expired()) {
			//std::cout << "NO TARGET SPECIFIED\n";
			return -1;
		}
		
		// Valid target defined, check the distance. Return -1 if target has moved out of range
		sf::Vector2f targetPos = target._Get()->GetPosition();
		float deltaX = abs(targetPos.x - GetPosition().x);
		float deltaY = abs(targetPos.y - GetPosition().y);

		// Calculate the distance
		float _dist = sqrt(deltaX*deltaX + deltaY*deltaY);

		//std::cout << "DEBUG <Tower::CheckTargetDistance()>: distance = " << _dist << "\n";

		if(_dist > shootDistance) {
			std::cout << "TARGET MOVED OUT OF RANGE\n";
			return -1;
		} else
			return _dist;
	}

	void Tower::SelectNewTarget()
	{
		if (!target.expired() && target._Get()->GetDeleteStatus())
			target.reset();

		// Select a new target, if no target specified OR the specified target has moved out of range
		if (target.expired() || CheckTargetDistance() == -1) {
			// There's no valid target defined for the tower, go through the attackers & check if some of them is in range
			std::vector<std::weak_ptr<GameObject>> _targets = Game::GetGameObjectManager()->GetByDistanceFrom(GetPosition().x, GetPosition().y, shootDistance);

			// Go through the targets & select the closest one
			std::weak_ptr<GameObject> _selected;
			if(_targets.size() > 0) {
				std::vector<std::weak_ptr<GameObject>>::iterator itr = _targets.begin();
				_selected = *itr;
				
				// Check the distance to the first attacker
				sf::Vector2f _targetPos = itr->lock()->GetPosition();
				float deltaX = abs(_targetPos.x - GetPosition().x);
				float deltaY = abs(_targetPos.y - GetPosition().y);
				float _dist = sqrt(deltaX*deltaX + deltaY*deltaY);
				// Jump to the next attacker
				itr++;

				// Go through the other attackers in range & select the closest one || The one with smallest structure
				while(itr != _targets.end()) {
					// Check the distance
					_targetPos = itr->lock()->GetPosition();
					deltaX = abs(_targetPos.x - GetPosition().x);
					deltaY = abs(_targetPos.y - GetPosition().y);
					
					//TODO select the one with smallest structure

					if(sqrt(deltaX*deltaX + deltaY*deltaY) < _dist) {
						_selected = *itr;
						_dist = sqrt(deltaX*deltaX + deltaY*deltaY);
					}

					itr++;
				}
			}

			if(!_selected.expired()) {
				std::cout << "DEBUG <Tower::SelectTarget>: Seleteted new target!\n";
				target = _selected;
				std::shared_ptr<Turret> _ptr_turret;
				_ptr_turret = std::dynamic_pointer_cast<Turret>(turret);
				_ptr_turret->SetTarget(target);
				
			} else {
				target.reset();
			}
		}
	}

	std::weak_ptr<GameObject> Tower::GetTarget()
	{
		return target;
	}

	unsigned int Tower::GetAmmoCount()
	{
		return ammoCount;
	}

	unsigned int Tower::GetClipSize()
	{
		return clipSize;
	}

	void Tower::SetMetaText(sf::Text _text)
	{
		metaText = _text;
	}

	void Tower::UpdateMetaText(float _x, float _y, std::string _text)
	{
		metaText.setPosition(_x, _y);
		metaText.setString( _text);
	}

	sf::Text Tower::GetMetaText()
	{
		return metaText;
	}

	void Tower::Reload()
	{
		std::cout << "RELOAD\n";
		metaText.setString("");
		ammoCount = 0;
	}
}