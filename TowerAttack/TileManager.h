#pragma once

#include "Tile.h" // This class manages tile objects
//#include "Game.h"
#include "TileRoad.h"
#include "Serialise.h"

#include "stdafx.h"
#include <list>
#include <map>
#include <fstream>
#include <memory>

namespace TA
{
	class TileManager
	{
	public:
		TileManager();
		~TileManager();

		void GenerateMap(); // Create a random map
		void Add(unsigned int _x, unsigned int _y, std::shared_ptr<Tile>); // Add tile object into tile map
		void Remove(unsigned int _x, unsigned int _y); // Remove tile object from the tile map
		const unsigned int GetObjectCount(); // Return how many tiles there're in the tile map
		std::shared_ptr<Tile> Get(unsigned int _x, unsigned int _y); // Return a handle to the specific tile
		void ClearObjects(); // Clear tile objects from tileMap

		void Draw(); // Draw tiles
		void Update(sf::Time elapsedTime); // Update tiles

		void SaveGame(std::ofstream & ofs); // Save all the game tiles (serialise data into the binary file)

		bool CheckMovable(unsigned int _x, unsigned int _y, std::string _direction); // Checks if we can move to _direction from the tile _x, _y.

		sf::Vector2f GetStartPosition(); // Return position of the start tile
		sf::Vector2f GetEndPosition(); // Return position of the end tile
		unsigned int GetTileWidth(); // Return the width of one tile

		sf::Vector2i GetTowerPlacement();

	private:
		void AddTile(unsigned int _type); // Add tile by type into tileMap, 0 = straigth road, 1 = turning road
		
		typedef std::pair<unsigned int, unsigned int> Key;
		typedef std::map<Key, std::shared_ptr<Tile>> Mapa;
		Mapa tileMap; // Mapping tiles in game board
		Mapa towerPlaces; // Possible places for defence towers

		std::shared_ptr<Tile> Get(Key _key); // Get tile by key

		std::shared_ptr<TileRoad> lastPlacedTile; // Which tile was placed, when generating a new map
		std::string lastDirection; // Direction where we came, when generating a new map
		unsigned int tileWidth; // Width/Height of one tile

		std::vector<std::shared_ptr<TileRoad>> tileOrder; // Order in the tiles are inserted

		sf::Vector2f endTilePos; // Position of the end tile
	};
}
