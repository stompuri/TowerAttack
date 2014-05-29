#include "StatePause.h"
#include "StateMenu.h"
#include "StatePlay.h"
#include "stdafx.h"

namespace TA
{
	StatePause StatePause::m_PauseState;

	void StatePause::Init()
	{

	}

	void StatePause::Cleanup()
	{
		printf("CMenuState Cleanup\n");
	}

	void StatePause::Pause()
	{
		printf("CMenuState Pause\n");
	}

	void StatePause::Resume()
	{
		printf("CMenuState Resume\n");
	}

	void StatePause::HandleEvents(sf::RenderWindow& window)
	{
		// Process events
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit
			if (Event.type == sf::Event::Closed)
			{
				Game::Quit();
			}

			if(Event.type == sf::Event::KeyPressed)
			{
				if(Event.key.code == sf::Keyboard::Escape) // Exit to menu screen 'Esc'
					Game::ChangeState( StateMenu::Instance() );
				if(Event.key.code == sf::Keyboard::P) // Pause key 'P'
				{
					Game::GetElapsedTime();
					Game::ChangeState( StatePlay::Instance() );
				}
			}
		}
	}

	void StatePause::Update() 
	{

	}

	void StatePause::Draw(sf::RenderWindow& window) 
	{

	}
}