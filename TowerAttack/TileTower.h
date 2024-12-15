#pragma once

#include "Tile.h" // Parent class
#include "stdafx.h"
#include <cassert>

namespace TA
{
	class TileTower : public Tile
	{
	public:
		TileTower();
		TileTower(int _x, int _y);
		~TileTower();

		// For writing/reading to/from binary file
		TileTower(std::istream& is); // Constructor for loading data from binary file
		void serialise(std::ostream& os); // Method to save object into binary file

		void setX(unsigned int _x);
		void setY(unsigned int _y);
		int getX();
		int getY();

	private:
		int x;
		int y;
	};
}