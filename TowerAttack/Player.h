#pragma once
#include "stdafx.h"

namespace TA {
	class Player
	{
	public:
		Player();
		~Player();

		void SetScore(unsigned int _score);
		void SetMoney(unsigned int _money);
		void SetAttackPoints(unsigned int _value);
		void AddScore(unsigned int _points);
		void AddMoney(unsigned int _money);
		void AddAttackPoints(unsigned int _value);
		unsigned int GetScore();
		unsigned int GetMoney();
		unsigned int GetAttackPoints();

	private:
		unsigned int score; // Points for attacker
		unsigned int money; // Points for upgrading units
		unsigned int attackPoints; // Points for buying units
	};
}

