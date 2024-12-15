#include "TileTower.h"

namespace TA
{
	TileTower::TileTower()
	{
		rotation = 0.0f;
		x = 0;
		y = 0;
		Load("resources/images/texture_tile_empty.png");
		assert(IsLoaded());
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
		std::cout << "DEBUG<TileTower::TileTower(): Empty tile initialized\n";
	}

	TileTower::TileTower(int _x, int _y)
	{
		x = _x;
		y = _y;

		Load("resources/images/texture_tile_empty.png");
		rotation = 0.0f;

		GetSprite().setRotation(rotation);
		assert(IsLoaded());
		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);
	}

	TileTower::~TileTower()
	{
	}


	TileTower::TileTower(std::istream& is) : Tile(is) // Read data for the parent (Tile)
	{
		//std::cout << "DEBUG <TileTower::TileTower(is)>: ***Loading data!\n";

		is.read((char*)&rotation, sizeof(float)); // Read rotation
		//std::cout << "DEBUG <TileTower::TileTower(is)>: Reading rotation: " + std::to_string(rotation) + "\n";
		GetSprite().setRotation(rotation);

		is.read((char*)&x, sizeof(unsigned int)); // Read position x
		//std::cout << "DEBUG <TileTower::TileTower(is)>: Reading x: " + std::to_string(x) + "\n";
		is.read((char*)&y, sizeof(unsigned int)); // Read position y
		//std::cout << "DEBUG <TileTower::TileTower(is)>: Reading y: " + std::to_string(y) + "\n";

		GetSprite().setOrigin(GetSprite().getGlobalBounds().height / 2, GetSprite().getGlobalBounds().width / 2);

		std::cout << "DEBUG <TileTower::TileTower(is)>: Data loaded (" << x << ", " << y << ")\n";
	}

	void TileTower::serialise(std::ostream& os) // Write the data into the file
	{
		//std::cout << "DEBUG <TileTower::serialise>: ***Saving data!\n";
		Tile::serialise(os);
		os.write((char*)&rotation, sizeof(float)); // Save rotation
		//std::cout << "DEBUG <TileTower::serialise>: Saving rotation: " + std::to_string(rotation) + "\n";
		os.write((char*)&x, sizeof(unsigned int)); // Save position x
		//std::cout << "DEBUG <TileTower::serialise>: Saving x: " + std::to_string(x) + "\n";
		os.write((char*)&y, sizeof(unsigned int)); // Save position y
		//std::cout << "DEBUG <TileTower::serialise>: Saving y: " + std::to_string(y) + "\n";

		std::cout << "DEBUG <TileTower::serialise>: Data saved (" << x << ", " << y << ")\n";
	}

	void TileTower::setX(unsigned int _x)
	{
		x = _x;
	}

	void TileTower::setY(unsigned int _y)
	{
		y = _y;
	}

	int TileTower::getX()
	{
		return x;
	}

	int TileTower::getY()
	{
		return y;
	}
}