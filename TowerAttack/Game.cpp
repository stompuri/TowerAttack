#include "Game.h"
#include "GameState.h"
#include "Effect.h" // Game maintains a list of shaders
#include "Tower.h" // Game creates tower object(s)
#include "SaveGameManager.h" // Saving game
#include "Drawer.h"

namespace TA
{
	// Singleton class, so default initialization here...
	std::string Game::playPhase;
	sf::RenderWindow Game::window;
	sf::View Game::view;
	sf::Vector2f Game::viewPosition;
	GameObjectManager Game::g_gameObjectManager;
	TileManager Game::g_tileManager;
	Player Game::g_player;
	bool Game::isRunning;
	sf::Clock Game::clock;
	sf::Time Game::elapsedTime;
	std::map<std::string, GameObject*> g_gameObjects;
	Game* Game::ptr_game = NULL;
	bool Game::instanceFlag = false;
	unsigned int Game::screenWidth;
	unsigned int Game::screenHeigth;
	std::vector<GameState*> Game::states;
	unsigned int Game::playTurn;
	unsigned int Game::playRound;
	bool Game::goMark;
	sf::Font Game::font1;
	sf::Font Game::font2;
	sf::Text Game::textTemp;
	CharacterSimple Game::baseCSimple;
	CharacterFast Game::baseCFast;
	CharacterHeavy Game::baseCHeavy;
	std::vector<Effect*> Game::effects;

	void Game::Init(const char* title, unsigned int width, unsigned int height)
	{
		if (!font1.loadFromFile("resources/fonts/ArcadeClassic.ttf"))
		{
			std::cout << "DEBUG <Game::Init> Font load error\n";
		}
		if (!font2.loadFromFile("resources/fonts/arial.ttf"))
		{
			std::cout << "DEBUG <Game::Init> Font load error\n";
		}

		// select the font
		textTemp.setFont(font2); // font is a sf::Font
		// set the string to display
		std::string _tempTxt = "Temp";
		textTemp.setString(_tempTxt);
		// set the character size
		textTemp.setCharacterSize(18); // in pixels, not points!
		// set the color
		textTemp.setColor(sf::Color::Red);
		// set the text style
		textTemp.setStyle(sf::Text::Bold);

		if (!sf::Shader::isAvailable())
		{
			// shaders are not available...
			std::cout << "Shaders not available\n";
		} else {
			// Create the effects
			effects.push_back(new Grayer);
			std::size_t current = 0;

			// Initialize them
			for (std::size_t i = 0; i < effects.size(); ++i)
				effects[i]->load();
		}

		playRound = 1;
		playTurn = 1;
		playPhase = "plan";
		goMark = true;
		viewPosition.x = 0.0f;
		viewPosition.y = 0.0f;
		window.create(sf::VideoMode(width, height, 32), title);
		screenWidth = width;
		screenHeigth = height;
		window.setVerticalSyncEnabled(true);
		isRunning = true;

		// Set up the view
		view.reset(sf::FloatRect(0, 0, float(width), float(height)));
		view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

		// Set up save game manager
		SaveGameManager* _ptr_sgm = SaveGameManager::GetInstance();
		_ptr_sgm->Init(&g_gameObjectManager, &g_tileManager);

		// Set up drawer
		Drawer* _ptr_drawer = Drawer::GetInstance();
		_ptr_drawer->Init();
	}

	void Game::Cleanup()
	{
		// cleanup all the states
		while ( !states.empty() ) {
			states.back()->Cleanup();
			states.pop_back();
		}

		isRunning = false;
		window.close();
	}

	void Game::ChangeState(GameState* state)
	{
		// cleanup the current state
		if ( !states.empty() ) {
			states.back()->Cleanup();
			states.pop_back();
		}

		// store and init the new state
		states.push_back(state);
		states.back()->Init();
	}

	void Game::PopState()
	{
		// cleanup the current state
		if ( !states.empty() ) {
			states.back()->Cleanup();
			states.pop_back();
		}

		// resume previous state
		if ( !states.empty() ) {
			states.back()->Resume();
		}
	}

	void Game::HandleEvents() 
	{
		// let the state handle events
		states.back()->HandleEvents(window);
	}

	void Game::Update() 
	{
		// let the state update the Game
		states.back()->Update();
	}

	void Game::Draw() 
	{
		// Set view to point to current view position
		view.reset(sf::FloatRect(viewPosition.x, viewPosition.y, float(screenWidth), float(screenHeigth)));
		window.setView(view);
		
		states.back()->Draw(window);
	}

	sf::RenderWindow & Game::GetWindow()
	{
		return window;
	}

	sf::View Game::GetView()
	{
		return view;
	}

	sf::Time Game::GetElapsedTime()
	{
		return clock.restart();
	}

	Game* Game::getInstance()
	{
		if(!ptr_game) {
			ptr_game = new Game;
			instanceFlag = true;
		}

		return ptr_game;
	}

	GameObjectManager* Game::GetGameObjectManager()
	{
		return &g_gameObjectManager;
	}

	TileManager* Game::GetTileManager()
	{
		return &g_tileManager;
	}

	Player* Game::GetPlayer()
	{
		return &g_player;
	}

	void Game::SaveGame(std::string _filename)
	{
		bool result = true;
		std::cout << "Saving Game!\n";

		try {
			SaveGameManager::SaveGame(_filename);
		} catch (std::exception& e) {
			std::cout << "Exception <SaveGame>: " << e.what() << std::endl;
			result = false;
		}
		if(result)
			std::cout << "DEBUG <Game::SaveGame> Game saved!\n";
		else
			std::cout << "DEBUG <Game::SaveGame> Something went wrong while saving the game!\n";

		clock.restart();
	}

	void Game::LoadGame	(std::string _filename)
	{
		bool result = true;
		std::cout << "Loading Game!\n";

		try {
			SaveGameManager::LoadGame(_filename);
		} catch (std::exception& e) {
			std::cout << "Exception <LoadGame>: " << e.what() << std::endl;
			result = false;
		}
		if(result)
			std::cout << "DEBUG <Game::LoadGame> Game loaded!\n";
		else
			std::cout << "DEBUG <Game::LoadGame> Something went wrong while loading the game!\n";

		clock.restart();
	}

	int Game::getScreenWidth() { return screenWidth; }
	int Game::getScreenHeigth() { return screenHeigth; }

	void Game::SetViewPosition(float _x, float _y)
	{
		viewPosition.x = _x;
		viewPosition.y = _y;
	}

	sf::Vector2f Game::GetViewPosition()
	{
		return viewPosition;
	}

	std::string Game::GetPlayPhase()
	{
		return playPhase;
	}

	unsigned int Game::GetPlayTurn()
	{
		return playTurn;
	}

	void Game::NextPhase()
	{
		if(playPhase == "attack")
			playPhase = "defend";
		else
			playPhase = "attack";
	}

	void Game::SetPlayPhase(std::string _phase)
	{
		playPhase = _phase;
	}

	void Game::NextTurn()
	{
		playTurn++;
	}

	unsigned int Game::GetPlayRound()
	{
		return playRound;
	}

	void Game::NextPlayRound()
	{
		playRound++;
	}

	void Game::ResetTurn()
	{
		playTurn = 1;
	}

	void Game::SetGoMark(bool _value)
	{
		goMark = _value;
	}

	bool Game::GetGoMark()
	{
		return goMark;
	}

	sf::Text Game::GetText()//std::string _type)
	{
		/*
		if(_type == "Attacker")
			return textAttacker;
		else*/
			return textTemp;
	}

	unsigned int Game::GetAttackPoints()
	{
		return g_player.GetAttackPoints();
	}

	void Game::ReduceAttackPoints(unsigned int _value)
	{
		g_player.SetAttackPoints(g_player.GetAttackPoints() - _value);
	}

	void Game::AddAttackPoints(unsigned int _value)
	{
		g_player.AddAttackPoints(_value);
	}

	int Game::GetMoney()
	{
		return g_player.GetMoney();
	}

	void Game::ReduceMoney(unsigned int _value)
	{
		g_player.SetMoney(g_player.GetMoney() - _value);
	}

	void Game::AddMoney(unsigned int _value)
	{
		g_player.AddMoney(_value);
	}

	void Game::InitGame()
	{
		viewPosition.x = 0;
		viewPosition.y = 0;

		// Generate random map
		g_tileManager.GenerateMap();

		// DEMO placements:
		float _tileWidth = float(g_tileManager.GetTileWidth());

		// Place towers on the game board
		// 1st tower
		// 1) Get the tile for new tower
		sf::Vector2i _towerTile = g_tileManager.GetTowerPlacement();
		std::cout << "Add tower to x = " << _towerTile.x << " y = " << _towerTile.y << "\n";
		// 2) Create the new tower and add it in the game object manager
		std::shared_ptr<Tower> _tower(new Tower(_towerTile.x * _tileWidth - 50, _towerTile.y *_tileWidth - 50));
		_tower->SetObjectId(g_gameObjectManager.Add("Tower", _tower));

		// 2nd tower
		_towerTile = g_tileManager.GetTowerPlacement();
		std::cout << "Add tower2 to x = " << _towerTile.x << " y = " << _towerTile.y << "\n";
		// 2) Create the new tower and add it in the game object manager
		std::shared_ptr<Tower> _tower2(new Tower(_towerTile.x * _tileWidth - 50, _towerTile.y *_tileWidth - 50));
		_tower2->SetObjectId(g_gameObjectManager.Add("Tower", _tower2));
	}

	void Game::ResetGame()
	{
		g_gameObjectManager.ClearObjects();
		g_tileManager.ClearObjects();
		g_player.SetAttackPoints(4);
		playRound = 1;
		playTurn = 1;
		playPhase = "plan";
		goMark = true;

		// Format the base classes
		baseCSimple.SetStructure(1);
		baseCSimple.SetArmor(0);
		baseCSimple.SetShield(0);
		baseCSimple.SetSpeed(1);
		baseCSimple.ResetUpLevels();
		baseCFast.SetStructure(1);
		baseCFast.SetArmor(0);
		baseCFast.SetShield(0);
		baseCFast.SetSpeed(2);
		baseCFast.ResetUpLevels();
		baseCHeavy.SetStructure(1);
		baseCHeavy.SetArmor(1);
		baseCHeavy.SetShield(0);
		baseCHeavy.SetSpeed(1);
		baseCHeavy.ResetUpLevels();
	}

	CharacterSimple* Game::GetBaseCSimple()
	{
		return &baseCSimple;
	}
	CharacterFast* Game::GetBaseCFast()
	{
		return &baseCFast;
	}
	CharacterHeavy* Game::GetBaseCHeavy()
	{
		return &baseCHeavy;
	}

	Effect & Game::GetEffect(unsigned int _place)
	{
		if(effects.size() < _place) {
			std::cout << "DEBUG<Game::GetEffect>: No effect found at place " << _place << "\n";
			assert(false);
		}

		return *effects[_place];
	}
}