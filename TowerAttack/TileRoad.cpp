#include "TileRoad.h"

namespace TA
{
	TileRoad::TileRoad()
	{
		rotation = 0.0f;
		x = 0;
		y = 0;
		points = 0;
		movableDown = true;
		movableUp = true;
		movableLeft = true;
		movableRight = true;
		Load("resources/images/texture_tile_empty.png");
		assert(IsLoaded());
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		std::cout << "DEBUG<TileRoad::TileRoad(): Empty tile initialized\n";
	}

	TileRoad::TileRoad(unsigned int _type, int _x, int _y)
	{
		/* _type = 0 - Straight horizontal
		   _type = 1 - Straight vertical
		   _type = 2 - Turn left/down
		   _type = 3 - Turn down/right
		   _type = 4 - Turn right/up
		   _type = 5 - Turn up/left
		   _type = 99 - Start tile
		   _type = 100 - End tile
		*/
		x = _x;
		y = _y;
		points = 0;
		movableDown = false;
		movableUp = false;
		movableLeft = false;
		movableRight = false;
		tileType = _type;

		if (_type == 0) {
			Load("resources/images/texture_tile_road_straigth.png");
			rotation = 0.0f;
			movableLeft = true;
			movableRight = true;
		} else if (_type == 1) {
			Load("resources/images/texture_tile_road_straigth.png");
			rotation = 90.0f;
			movableDown = true;
			movableUp = true;
		} else if (_type == 2) {
			Load("resources/images/texture_tile_road_turn.png");
			rotation = 180.0f;
			movableDown = true;
			movableRight = true;
		} else if (_type == 3) {
			Load("resources/images/texture_tile_road_turn.png");
			rotation = 90.0f;
			movableUp = true;
			movableRight = true;
		} else if (_type == 4) {
			Load("resources/images/texture_tile_road_turn.png");
			rotation = 0.0f;
			movableUp = true;
			movableLeft = true;
		} else if (_type == 5) {
			Load("resources/images/texture_tile_road_turn.png");
			rotation = 270.0f;
			movableDown = true;
			movableLeft = true;
		} else if (_type == 99) {
			Load("resources/images/texture_tile_road_begin.png");
			rotation = 0.0f;
			movableRight = true;
		} else if (_type == 100) {
			Load("resources/images/texture_tile_road_end.png");
			rotation = 0.0f;
		} else {
			Load("resources/images/texture_tile_empty.png");
			rotation = 0.0f;
		}

		GetSprite().setRotation(rotation);
		assert(IsLoaded());
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		//std::cout << "DEBUG<TileRoad::TileRoad(): Road tile initialized, x = " << _x << " y = " << _y << "\n";
	}

	TileRoad::~TileRoad()
	{
	}


	TileRoad::TileRoad(std::istream & is) : Tile(is) // Read data for the parent (Tile)
	{
		//std::cout << "DEBUG <TileRoad::TileRoad(is)>: ***Loading data!\n";

		is.read((char*)&rotation, sizeof(float)); // Read rotation
		//std::cout << "DEBUG <TileRoad::TileRoad(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		GetSprite().setRotation(rotation);

		is.read((char*)&x, sizeof(unsigned int)); // Read position x
		//std::cout << "DEBUG <TileRoad::TileRoad(is)>: Reading x: " + std::to_string(x) + "\n";
		is.read((char*)&y, sizeof(unsigned int)); // Read position y
		//std::cout << "DEBUG <TileRoad::TileRoad(is)>: Reading y: " + std::to_string(y) + "\n";
	
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		std::cout << "DEBUG <TileRoad::TileRoad(is)>: Data loaded (" << x << ", " << y << ")\n";
	}

	void TileRoad::serialise(std::ostream & os) // Write the data into the file
	{
		//std::cout << "DEBUG <TileRoad::serialise>: ***Saving data!\n";
		Tile::serialise(os);
		os.write((char*)&rotation, sizeof(float)); // Save rotation
		//std::cout << "DEBUG <TileRoad::serialise>: Saving rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&x, sizeof(unsigned int)); // Save position x
		//std::cout << "DEBUG <TileRoad::serialise>: Saving x: " + std::to_string(x) + "\n";
		os.write((char*)&y, sizeof(unsigned int)); // Save position y
		//std::cout << "DEBUG <TileRoad::serialise>: Saving y: " + std::to_string(y) + "\n";

		std::cout << "DEBUG <TileRoad::serialise>: Data saved (" << x << ", " << y << ")\n";
	}

	void TileRoad::setX(unsigned int _x)
	{
		x = _x;
	}

	void TileRoad::setY(unsigned int _y)
	{
		y = _y;
	}

	int TileRoad::getX()
	{
		return x;
	}

	int TileRoad::getY()
	{
		return y;
	}

	unsigned int TileRoad::GetTileType()
	{
		return tileType;
	}

	void TileRoad::SetPoints(unsigned int _points)
	{
		points = _points;
	}

	unsigned int TileRoad::GetPoints()
	{
		return points;
	}
}