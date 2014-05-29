#pragma once

#include "CharacterSimple.h" // Game maintains a base class for simple attackers
#include "CharacterFast.h" // Game maintains a base class for fast attackers
#include "CharacterHeavy.h" // Game maintains a base class for heavy attackers
#include "GameObjectManager.h" // Game adds objects into GameObjectManager, which manages all the game objects
#include "TileManager.h" // Game calls TileManager which manages game map
#include "Player.h"

#include "Effects.h" // Game creates new shaders
#include "SFML/Window.hpp" // SFML library for window handling
#include "SFML/Graphics.hpp" // SFML library for graphics
#include <ctime>
#include <vector>
#include "stdafx.h"

namespace TA
{
	class GameState; // Forward declaration, as only pointers needed
	class Effect; // Forward declaration, as only pointers needed

	class Game
	{
	public:
		// Game is a singleton class
		~Game() { instanceFlag = false; }
		static Game* getInstance(); // Return a handle to the active Game object

		static void Init(const char* title, unsigned int width=800, unsigned int height=600); // Init game
		static void Cleanup(); // Clear the game states and set the game class instance as inactive
		
		static void InitGame();
		static GameObjectManager* GetGameObjectManager(); // Return handle to our GameObjectManager
		static TileManager* GetTileManager(); // Return handle to our TileManager
		static Player* GetPlayer(); // Return handle to Player
		static Effect & GetEffect(unsigned int _place); // Get pointer to the effect at certain position
		
		// Game state management integrated in this class:
		static void ChangeState(GameState* state); // Change state
		static void PushState(GameState* state); // Push state into state queue (to be able to return to current state after...)
		static void PopState(); // Change state to the previous state
		
		// Game loop:
		static void HandleEvents(); // Handles events: Input & other possible
		static void Update(); // Update things
		static void Draw(); // Draw things (game objects & game board)

		// Gameloop related methods
		static bool Running() { return isRunning; } // Boolean flag telling to the game loop if the game is running
		static void Quit() { isRunning = false; } // Set the running flag to false
		static void SaveGame(std::string _filename); // Save the game state of GameObjects
		static void LoadGame(std::string _filename); // Load the game state for GameObjects (clear the current state for GameObjects first)
		static sf::Time GetElapsedTime(); // Resets the game clock and returns the time value from the last reset

		// Getters & Setters
		static int getScreenWidth(); // Return the width of the game screen
		static int getScreenHeigth(); // Return the heigth of the game screen
		static sf::Vector2f GetViewPosition(); // Return the view position
		static sf::View GetView();
		static sf::RenderWindow & GetWindow();
		static CharacterSimple* GetBaseCSimple(); // Get pointer to simple base character
		static CharacterFast* GetBaseCFast(); // Get pointer to fast base character
		static CharacterHeavy* GetBaseCHeavy(); // Get pointer to heavy base character
		static bool GetGoMark();
		static sf::Text GetText();
		static int GetMoney();
		static std::string GetPlayPhase();
		static unsigned int GetPlayTurn();
		static unsigned int GetPlayRound();
		static void SetGoMark(bool _b);
		static void SetViewPosition(float _x, float _y); // Set position for view
		static void SetPlayPhase(std::string _phase);

		// Other methods
		static void ResetGame(); // Reset the current game data
		static unsigned int GetAttackPoints();
		static void ReduceAttackPoints(unsigned int _value);
		static void AddAttackPoints(unsigned int _value);
		static void AddMoney(unsigned int _value);
		static void ReduceMoney(unsigned int _value);
		static void NextTurn();
		static void ResetTurn();
		static void NextPhase();
		static void NextPlayRound();

	private:
		Game() { } // Singleton class: constructor as private
		static Game* ptr_game; // pointer to our singleton game class
		static bool instanceFlag; // is our singleton class active
		static bool isRunning; // boolean flag telling if the game is running or not

		static GameObjectManager g_gameObjectManager; // GameObjectManager (which handles objects of our game world)
		static TileManager g_tileManager; // Manages tiles in game maps
		static Player g_player; // Player data
		
		static unsigned int screenWidth; // game screen width
		static unsigned int screenHeigth; // game screen heigth
		static std::vector<Effect*> effects; // List of used shaders

		static CharacterSimple baseCSimple; // Base unit for simple characters
		static CharacterFast baseCFast; // Base unit for fast characters
		static CharacterHeavy baseCHeavy; // Base unit for heavy characters

		static sf::Font font1;
		static sf::Font font2;
		static sf::Text textTemp;
		static sf::Clock clock; // Clock to get current time
		static sf::Time elapsedTime; // Elapsed time (To get the time from the last game loop iteration)
		static sf::RenderWindow window; // Game window where we render stuff
		static sf::View view; // Game view (to manage view options, like position)
		static sf::Vector2f viewPosition; // Position of the view
		static std::vector<GameState*> states; // stack of game states
		static std::string playPhase; // Which phase going on in game play (attack, defend)
		static unsigned int playTurn; // Which turn going on in game play (1, 2, 3,...)
		static unsigned int playRound; // Which round going on in game play (1, 2, 3,...)
		static bool goMark; // Boolean telling if attackers can start moving towards the next cell
	};
}
