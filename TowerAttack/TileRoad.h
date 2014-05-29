#pragma once

#include "Tile.h" // Parent class
#include "stdafx.h"
#include <cassert>

namespace TA
{
	class TileRoad : public Tile
	{
	public:
		TileRoad();
		TileRoad(unsigned int _type, int _x, int _y); // Constructor for different kind of road tiles
		~TileRoad();

		// For writing/reading to/from binary file
		TileRoad(std::istream & is); // Constructor for loading data from binary file
		void serialise(std::ostream & os ); // Method to save object into binary file

		void setX(unsigned int _x);
		void setY(unsigned int _y);
		int getX();
		int getY();
		unsigned int GetTileType();
		void SetPoints(unsigned int _points);
		unsigned int GetPoints();

	private:
		int x;
		int y;
		unsigned int tileType;
		unsigned int points; // Points given if an attacker unit gets destroyed on this tile
	};
}