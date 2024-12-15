#include "TileManager.h"
#include "Game.h"
#include "Drawer.h"

namespace TA
{
	TileManager::TileManager()
	{
		lastPlacedTile = NULL;
		lastDirection = "";
		tileWidth = 100;
		endTilePos.x = NULL;
		endTilePos.y = NULL;
	}

	TileManager::~TileManager()
	{
		// Use lambda to delete each tile object if the Manager is deleted
		std::for_each(tileMap.begin(),tileMap.end(),
			[] (std::pair<Key, std::shared_ptr<TA::Tile>> p) { p.second.reset(); }
			);

		std::for_each(towerPlaces.begin(), towerPlaces.end(),
			[](std::pair<Key, std::shared_ptr<TA::Tile>> p) { p.second.reset(); }
		);

		// std::cout << "DEBUG <TileManager::~TileManager>: Tiles deleted\n";
	}

	void TileManager::GenerateMap()
	{
		/* Random map generator
		First saves the tile amounts in two dimensional array.
		Then picks up the tiles in random order & place them in the map.
		We have the starting tile (straigth road) at position x=1, y=3

		Tile type: 0 1 (0 = straigth road, 1 = turning road)
		Amount:     3 3
		*/
		
		unsigned int _tileAmount = 7;
		//commented1 std::cout << "DEBUG <TileManager::GenerateMap>: tileArray = ";
		unsigned int _tileArray[2][2];
		for(int i=0; i<2; i++)
		{
			//commented1 std::cout << "tileArray[1]["<<i<<"] = 1\n";
			_tileArray[0][i] = i;
			_tileArray[1][i] = _tileAmount;
			//commented1 std::cout << _tileArray[1][i] << " ";
		}
		//commented1 std::cout << "\n";

		// Set the start tile
		unsigned int _x = 1;
		unsigned int _y = 6;
		std::shared_ptr<TileRoad> _tile(new TileRoad(99, _x, _y)); // Type of the tile, x, y
		Add(_x, _y, _tile);
		lastPlacedTile = _tile;
		lastDirection = "";

		// initialize random seed
		srand (int(time(NULL)));

		// Loop until all tiles have been placed from the array
		while (_tileArray[1][0] + _tileArray[1][1] != 0)
		{
			// Randomise a number 0/1 (choose the type of the tile: straight/turning)
			unsigned int _choice = rand() % 2;
			
			// Add the tile into tile map & reduce the tile amount from the array
			if (_choice == 0 && _tileArray[1][0] > 0)
			{
				// Straigth road selected, add it into gameboard
				AddTile(0);
				_tileArray[1][0]--;
			} else {
				// Turning road selected, add it into gameboard
				AddTile(1);
				_tileArray[1][1]--;
			}
		}
		
		// Place the end tile
		std::cout << "DEBUG <TileManager::GenerateMap> lastDirection = " << lastDirection << " last.x = " << lastPlacedTile->getX() << " last.y = " << lastPlacedTile->getY() << " type = " << lastPlacedTile->GetTileType() << "\n";
		_x = lastPlacedTile->getX();
		_y = lastPlacedTile->getY();
		if(lastPlacedTile->GetMovable("right") && lastDirection != "right")
			_x++;
		else if(lastPlacedTile->GetMovable("up") && lastDirection != "up")
			_y--;
		else if(lastPlacedTile->GetMovable("left") && lastDirection != "left")
			_x--;
		else if(lastPlacedTile->GetMovable("down") && lastDirection != "down")
			_y++;

		std::shared_ptr<TileRoad> _endTile(new TileRoad(100, _x, _y)); // Type of the tile, x, y
		// Rotate end tile
		if(lastPlacedTile->GetMovable("right") && lastDirection != "right")
			_endTile->setRotation(180.0f);
		else if(lastPlacedTile->GetMovable("up") && lastDirection != "up")
			_endTile->setRotation(90.0f);
		else if(lastPlacedTile->GetMovable("left") && lastDirection != "left")
			_endTile->setRotation(0.0f);
		else if(lastPlacedTile->GetMovable("down") && lastDirection != "down")
			_endTile->setRotation(270.0f);

		Add(_x, _y, _endTile);
		lastPlacedTile = _endTile;
		lastDirection = "";

		// Get the max & min values for tiles
		int _maxX = 0;
		int _maxY = 0;
		int _minX = 100;
		int _minY = 100;
		Mapa::iterator itr = tileMap.begin();
		std::vector<Key> tileList;
		while(itr != tileMap.end())
		{
			TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(itr->second.get());
			//std::cout << "Tile x = " << _ptr_tile->getX() << " y = " << _ptr_tile->getY() << " type = " << _ptr_tile->GetTileType() << "\n";
			tileList.push_back(Key(_ptr_tile->getX(), _ptr_tile->getY()));
			if (_ptr_tile->getX() < _minX) _minX = _ptr_tile->getX();
			if (_ptr_tile->getX() > _maxX) _maxX = _ptr_tile->getX();
			if (_ptr_tile->getY() < _minY) _minY = _ptr_tile->getY();
			if (_ptr_tile->getY() > _maxY) _maxY = _ptr_tile->getY();
			itr++;
		}
		
		//std::cout << "DEBUG1 Tile minX = " << _minX << " minY = " << _minY << " maxX = " << _maxX << " maxY = " << _maxY << "\n";

		// Go through the map tiles & set sprite positions according to tile x, y coordinates
		// +fix the key (delete the old tileMap input & add a new one)
		Mapa tempTileMap; // temp version of tilemap

		std::vector<Key>::iterator vit = tileList.begin();
		while(vit != tileList.end())
		{
			Key oldKey (vit->first, vit->second);
			TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(tileMap[oldKey].get());

			_ptr_tile->setX(_ptr_tile->getX()-(_minX-1));
			_ptr_tile->setY(_ptr_tile->getY()-(_minY-1));
			//std::cout << "DEBUG2 Tile x = " << _ptr_tile->getX() << " y = " << _ptr_tile->getY() << "\n";
			_ptr_tile->SetSpritePosition(float(_ptr_tile->getX()*tileWidth-50), float(_ptr_tile->getY()*tileWidth-50));

			Key newKey (_ptr_tile->getX(), _ptr_tile->getY());
			tempTileMap.insert(std::make_pair(newKey, tileMap[oldKey]));

			vit++;
		}

		// Swap tileMap and tempTileMap
		tileMap.clear();
		itr = tempTileMap.begin();
		while(itr != tempTileMap.end())
		{
			tileMap.insert(std::make_pair(itr->first, itr->second));
			itr++;
		}
		tempTileMap.clear();
		
		// print map for debugging
		itr = tileMap.begin();
		while(itr != tileMap.end())
		{
			TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(itr->second.get());
			//std::cout << "Tile x = " << _ptr_tile->getX() << " y = " << _ptr_tile->getY() << " type = " << _ptr_tile->GetTileType() << "\n";
			itr++;
		}

		// Set the points for tiles (for scoring points)
		// Go through the route from end towards start and set points: 10-8-6-4-3-2-1 (15 for reaching finish)
		unsigned int _points[] = {10,8,6,4,3,2,1};
		unsigned int _counter = 0;
		std::vector<std::shared_ptr<TileRoad>>::reverse_iterator it = tileOrder.rbegin();
		while(_counter < 7)
		{
			it->get()->SetPoints(_points[_counter]);
			_counter++;
			//std::cout << "<DEBUG TileManager::GenerateMap> Tile x = " << it->get()->getX() << " y = " << it->get()->getY() << " points = " << it->get()->GetPoints() << "\n";

			it++;
		}

		std::cout << "DEBUG <TileManager::GenerateMap>: Map generated\n";
	}

	void TileManager::AddTile(unsigned int _type)
	{
		unsigned int _x, _y;
		// If placing the first tile, randomise the start position
		if(lastPlacedTile == NULL)
		{
			//commented1 std::cout << "DEBUG <TileManager::AddTile>: Placing the first road tile\n";
			// generate int number between 1 and 8
			_x = rand() % 8 + 1;
			// generate int number between 1 and 6
			_y = rand() % 6 + 1;
		} else {
			// If not first tile, place the tile next to previous tile
			_x = lastPlacedTile->getX();
			_y = lastPlacedTile->getY();

			//commented1 std::cout << "DEBUG <TileManager::AddTile>: Placing next road tile next to x=" << _x << " y=" << _y << "\n";

			std::string _direction = "up";
			// Randomise direction where we go next
			unsigned int _dir = rand() % 4;

			// If the randomized direction is not movable, try the next direction
			for(int i=0; i<4; i++)
			{
				// 0 = left, 1 = down, 2 = right, 3 = up
				if (_dir == 0)
					_direction = "left";
				else if (_dir == 1)
					_direction = "down";
				else if (_dir == 2)
					_direction = "right";
				else if (_dir == 3)
					_direction = "up";

				if (lastPlacedTile->GetMovable(_direction) && _direction != lastDirection)
					break;
				_dir++;
				if (_dir > 3)
					_dir = 0;
			}

			// Now we know which side of lastPlacedTile we will place the new tile
			// Mark up the direction where we came from (so we don't go back there)
			if (_direction == "left") {
				_x -= 1;
				lastDirection = "right";
			} else if (_direction == "down") {
				_y += 1;
				lastDirection = "up";
			} else if (_direction == "right") {
				_x += 1;
				lastDirection = "left";
			} else if (_direction == "up") {
				_y -= 1;
				lastDirection = "down";
			}
		}

		// We know the place, but we don't know how we rotate it
		unsigned int _tileType = _type;

		if (_type == 0) {
			// 1) We have a straigth road tile
			if (lastDirection == "left" || lastDirection == "right")
				_tileType = 0;
			else
				_tileType = 1;
		} else {
			// 2) We have a turning road tile
			if (lastDirection == "left")
				_tileType = rand() % 2 + 4; // _tileType = 4/5
			else if (lastDirection == "right")
				_tileType = rand() % 2 + 2; // _tileType = 2/3
			else if (lastDirection == "up")
				_tileType = 3; // _tileType = 3
			else if (lastDirection == "down")
				_tileType = 2; // _tileType = 2
		}

		std::shared_ptr<TileRoad> _tile(new TileRoad(_tileType, _x, _y));
		Add(_x, _y, _tile);
		lastPlacedTile = _tile;
	}

	void TileManager::Add(unsigned int _x, unsigned int _y, std::shared_ptr<Tile> _ptr_tile)
	{
		Key _key (_x, _y);
		tileMap.insert(std::make_pair(_key, _ptr_tile));
		std::shared_ptr<TileRoad> _tile = std::dynamic_pointer_cast<TileRoad>(_ptr_tile);
		tileOrder.push_back(_tile);
		std::cout << "DEBUG <TileManager::Add>: Tile added with key (x, y): " << _x << " " << _y << "\n";
	}

	// Remove tile object from the tile map
	void TileManager::Remove(unsigned int _x, unsigned int _y)
	{
		Key _key(_x, _y);
		tileMap.erase(_key);
		////commented1 std::cout << "DEBUG <TileManager::Remove>: Tile remove with key (x, y): " << _x << " " << _y << "\n";
	}

	void TileManager::Draw()
	{
		// Go through all the gameobjects in gameobjects map and draw the sprites in them
		// Draw first other game objects, then projectiles on top of them
		Mapa::const_iterator itr = tileMap.begin();
		while(itr != tileMap.end())
		{
			// std::cout << "DEBUG <TileManager::Add>: Tile drawn (" << itr->second->GetSpritePosition().x << ", " << itr->second->GetSpritePosition().y << "\n"; 
			Drawer::DrawTile(std::dynamic_pointer_cast<TileRoad>(itr->second));
			itr++;
		}
	}

	const unsigned int TileManager::GetObjectCount()
	{
		return tileMap.size();
	}

	void TileManager::SaveGame(std::ofstream & ofs) throw (...)
	{
		Mapa::const_iterator itr = tileMap.begin();

		// Save the amount of Tiles
		int amount = tileMap.size();
		ofs.write((char *)&amount, sizeof(amount));

		while(itr != tileMap.end())
		{
			// Save (serialise) the tile object
			itr->second->serialise(ofs);
			itr++;
		}

		////commented1 std::cout << "DEBUG <TileManager::SaveGame>: Saved tiles'!\n";
	}

	std::shared_ptr<Tile> TileManager::Get(unsigned int _x, unsigned int _y)
	{
		Mapa::const_iterator results = tileMap.find(Key(_x, _y));
		if(results == tileMap.end() ) {
			std::cout << "DEBUG <TileManager::Get>: Didn't find tile from x=" << _x << " y=" << _y << "\n";
			assert(false);
			return NULL;
		}

		////commented1 std::cout << "DEBUG <TileManager::Get>: Found tile from x=" << _x << " y=" << _y << "\n";
		return results->second;
	}

	std::shared_ptr<Tile> TileManager::Get(Key _key)
	{
		Mapa::const_iterator results = tileMap.find(_key);
		if(results == tileMap.end() ) {
			//commented1 std::cout << "DEBUG <TileManager::Get>: Didn't find tile\n";
			return NULL;
		}
		return results->second;
	}

	/* CheckMovable
	   Method checks if there's a tile in the asked coordinates and if there's
	   checks if that tile is movable to asked direction.
	   Return true/false
	*/
	bool TileManager::CheckMovable(unsigned int _x, unsigned int _y, std::string _direction)
	{
		// Get the tile from given position
		std::shared_ptr<Tile> _tile = Get(_x, _y);

		// If there's no tile at that position, it's not movable
		if (_tile == NULL) {
			//commented1 std::cout << "DEBUG <TileManager::CheckMovable>: Tile not found from x=" << _x << " y=" << _y << "\n";
			return false;
		}
		////commented1 std::cout << "DEBUG <TileManager::CheckMovable>: Tile found, movable = " << _tile->GetMovable(_direction) << "\n";
		// If there's a tile in the given position, return if it's possible to move to given direction
		return _tile->GetMovable(_direction);
	}

	/* GetStartPosition -
	   Method returns the location of the start tile as sf::Vector2f
	   The location is returned as pair of unsigned int values (even if in float format)
	   In case of multiple start tiles, returns the first one found.
	   In case of no found start tiles, returns vector with x=9999, y=9999
	   Return sf::Vector2f
	*/
	sf::Vector2f TileManager::GetStartPosition()
	{
		sf::Vector2f _returnValue(9999, 9999);
		Mapa::const_iterator itr = tileMap.begin();
		while(itr != tileMap.end())
		{
			TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(itr->second.get());
			if(_ptr_tile->GetTileType() == 99)
			{
				_returnValue.x = float(_ptr_tile->getX());
				_returnValue.y = float(_ptr_tile->getY());
				//std::cout << "DEBUG<TileManager::GetStartPosition>: Return x = " << _returnValue.x << " y = " << _returnValue.y << "\n";
				return _returnValue;
			}
			itr++;
		}
		
		return _returnValue;
	}

	sf::Vector2f TileManager::GetEndPosition()
	{
		if(endTilePos.x == NULL) {
			sf::Vector2f _returnValue(9999, 9999);
			Mapa::const_iterator itr = tileMap.begin();
			while(itr != tileMap.end())
			{
				TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(itr->second.get());
				if(_ptr_tile->GetTileType() == 100)
				{
					_returnValue.x = float(_ptr_tile->getX());
					_returnValue.y = float(_ptr_tile->getY());
					return _returnValue;
				}
				itr++;
			}
			endTilePos.x = _returnValue.x;
			endTilePos.y = _returnValue.y;
		}
		
		return endTilePos;
	}

	unsigned int TileManager::GetTileWidth()
	{
		return tileWidth;
	}

	void TileManager::ClearObjects()
	{
		tileMap.clear();
		//commented1 std::cout << "DEBUG <TileManager::LoadGame>: Cleared tile map!\n";
	}

	/* GetTowerPlacement -
	   List all the tiles that are next to the road and empty
	   and pick randomly one of those tiles.
	   Return tile coordinates in sf::vector2i format
	*/
	sf::Vector2i TileManager::GetTowerPlacement()
	{
		sf::Vector2i _retu = sf::Vector2i(0, 0);

		typedef std::map<Key, std::string> Mapb;
		Mapb _tempList;

		// 1) List all possible tiles
		Mapa::iterator itr = tileMap.begin();
		while(itr != tileMap.end())
		{
			TileRoad* _ptr_tile = dynamic_cast<TileRoad*>(itr->second.get());

			// Jump over the start tile and the end tile
			if(_ptr_tile->GetTileType() != Game::TileType::StartTile && _ptr_tile->GetTileType() != Game::TileType::EndTile)
			{
				// Go through the road and check the tiles around it
				// & Create a list of possible turret places (next to road)
				Key _findKey;
				int x = 0; int y = 0;

				// Check the tile up, left, right, down
				//  x
				// xox
				//  x
				for(int y=-1; y<2; y++) {
					if(y==-1) {
						x = 0;
					} else if(y==0) {
						x = -1;
					} else if(y==1) {
						x = 0;
					}

					_findKey = Key(_ptr_tile->getX()-x, _ptr_tile->getY()-y);
					std::cout << "TileManager:GetTowerPlacement - X = " << _findKey.first << " Y = " << _findKey.second << "\n";

					// 1) No tile is found, 2) this tile has not been inserted yet in templist, 3) No other objects in this tile
					if(	Get(_findKey) == NULL &&
						_tempList.find(_findKey) == _tempList.end() &&
						Game::GetGameObjectManager()->GetByPosition(int(_findKey.first), int(_findKey.second)).size() == 0 &&
						towerPlaces.find(_findKey) == towerPlaces.end()
						)
					{
						//std::cout << "TileManager: " << _findKey.first << "/" << _findKey.second << " size = " << Game::GetGameObjectManager()->GetByPosition(int(_findKey.first), int(_findKey.second)).size() << "\n";
						_tempList.insert(std::make_pair(_findKey, std::to_string(_ptr_tile->getX()-x) + std::to_string(_ptr_tile->getY()-y)));
					}

					if(y==0) {
						x = 1;
						_findKey = Key(_ptr_tile->getX()-x, _ptr_tile->getY()-y);
						if(	Get(_findKey) == NULL &&
							_tempList.find(_findKey) == _tempList.end() &&
							Game::GetGameObjectManager()->GetByPosition(int(_findKey.first), int(_findKey.second)).size() == 0)
						{
							//std::cout << "TileManager: " << _findKey.first << "/" << _findKey.second << " size = " << Game::GetGameObjectManager()->GetByPosition(int(_findKey.first), int(_findKey.second)).size() << "\n";
							_tempList.insert(std::make_pair(_findKey, std::to_string(_ptr_tile->getX()-x) + std::to_string(_ptr_tile->getY()-y)));
						}
					}
				}
			}
			
			itr++;
		}

		/*Mapb::iterator _itr = _tempList.begin();
		while(_itr != _tempList.end())
		{	
			//commented1 std::cout << "Tile x = " << _itr->first.first << "/" << _itr->first.second << " - " << _itr->second << "\n";
			_itr++;
		}*/

		// 2) Pick one possible tile randomly
		// initialize random seed
		srand (int(time(NULL)));
		unsigned int _randomizer = rand() % _tempList.size();

		/*
		unsigned int _randomX = rand() % 7 + 1; // generate random number between 1 and 7
		unsigned int _randomY = rand() % 6; // generate random number between 0 and 5
		while (Get(_randomX, _randomY) != NULL)
		{
			_randomX = rand() % 7 + 1;
			_randomY = rand() % 6;
		}
		_retu = sf::Vector2i(_randomX, _randomY);
		*/
		Mapb::iterator _itr = _tempList.begin();
		for(unsigned int i=0; i<_randomizer; i++)
		{	
			_itr++;
		}
		_retu = sf::Vector2i(_itr->first.first, _itr->first.second);

		std::shared_ptr<TileTower> _tile(new TileTower(_itr->first.first, _itr->first.second));
		towerPlaces.insert(std::make_pair(Key(_itr->first.first, _itr->first.second), _tile));

		return _retu;
	}
}