#pragma once

#include <SFML/System.hpp> // We are using stuff from there
#include "stdafx.h"
#include <cassert>
#include <memory>

namespace TA
{
	class AI
	{
	public:
		AI();
		~AI();

		void Update(sf::Time elapsedTime); // Let the AI make decissions
	};
}