#include "Player.h"

namespace TA {
	Player::Player()
	{
		score = 0;
		money = 2;
		attackPoints = 4;
	}

	Player::~Player()
	{

	}

	void Player::AddScore(unsigned int _points)
	{
		score += _points;
	}

	void Player::AddAttackPoints(unsigned int _value)
	{
		attackPoints += _value;
	}

	void Player::AddMoney(unsigned int _money) {
		money += _money;
	}

	unsigned int Player::GetScore()
	{
		return score;
	}

	unsigned int Player::GetMoney()
	{
		return money;
	}

	unsigned int Player::GetAttackPoints()
	{
		return attackPoints;
	}

	void Player::SetScore(unsigned int _points)
	{
		score = _points;
	}

	void Player::SetMoney(unsigned int _money) {
		money = _money;
	}

	void Player::SetAttackPoints(unsigned int _value) {
		attackPoints = _value;
	}
}
