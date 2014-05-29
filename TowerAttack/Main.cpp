#include "Game.h"
#include "UnitTests.h"
#include "StateSplash.h"
#include "stdafx.h"

int main ()
{
	// Create a singleton instance from the TA::Game class
	TA::Game *game = TA::Game::getInstance();

	// Initialize the Game
	try
	{
		game->Init("Tower Attack");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		return 0;
	}

	// Do we want to execute the unit tests
	if(false)
	{
		// Route cout stream to unittests.txt file
		std::ofstream out("unittests_log.txt");
		std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

		TA::UnitTests *utests = new TA::UnitTests();
		utests->RunTests();

		std::cout.rdbuf(coutbuf); //reset to standard output again
	}

	// load the splash screen
	game->ChangeState( TA::StateSplash::Instance() );

	// gameloop
	while ( game->Running() )
	{
		game->HandleEvents();
		game->Update();
		game->Draw();
	}

	// cleanup the engine
	game->Cleanup();

	return 0;
}
