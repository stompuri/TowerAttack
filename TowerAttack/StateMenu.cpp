#include "StateMenu.h"
#include "StatePlay.h"
#include "stdafx.h"

namespace TA
{
	sf::Sprite *StateMenu::p_sprite;
	sf::Texture StateMenu::image;
	StateMenu StateMenu::m_MenuState;
	StateMenu::MenuResult StateMenu::action;
	std::list<StateMenu::MenuItem> StateMenu::menuItems;

	void StateMenu::Init()
	{
		//Load menu image from file
		if (! image.loadFromFile("resources/images/mainmenu.png"))
			throw(EXIT_FAILURE);
		p_sprite = new sf::Sprite(image);

		// Set up clickable regions
		// Play - menu item coordinates
		MenuItem playButton;
		playButton.rect.top= 184;
		playButton.rect.height = 113;
		playButton.rect.left = 278;
		playButton.rect.width = 243;
		playButton.action = Play;

		// Exit - menu item coordinates
		MenuItem exitButton;
		exitButton.rect.top = 303;
		exitButton.rect.height = 106;
		exitButton.rect.left = 306;
		exitButton.rect.width = 192;
		exitButton.action = Exit;

		menuItems.push_back(playButton);
		menuItems.push_back(exitButton);
		printf("StateMenu Init\n");
	}

	void StateMenu::Cleanup()
	{
		printf("StateMenu Cleanup\n");
	}

	void StateMenu::Pause()
	{
		printf("StateMenu Pause\n");
	}

	void StateMenu::Resume()
	{
		printf("StateMenu Resume\n");
	}

	void StateMenu::HandleEvents(sf::RenderWindow& window)
	{
		sf::Event menuEvent;

		while (window.pollEvent(menuEvent))
		{
			if(menuEvent.type == sf::Event::MouseButtonPressed)
			{
				action = HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);
			}
			if(menuEvent.type == sf::Event::Closed)
			{
				action = Exit;
			}
		}
	}

	void StateMenu::Update() 
	{
		if (action == Exit) {
			std::cout << "Exiting...";
			Game::Quit();
		} else if (action == Play) {
			std::cout << "New game...";
			Game::InitGame();
			Game::GetElapsedTime();
			Game::ChangeState( StatePlay::Instance() );
			action = Nothing;
		}
	}

	void StateMenu::Draw(sf::RenderWindow& window) 
	{
		window.draw(*p_sprite);
		window.display();
	}

	StateMenu::MenuResult StateMenu::HandleClick(int x, int y)
	{
		std::list<MenuItem>::iterator it;

		for ( it = menuItems.begin(); it != menuItems.end(); it++)
		{
			sf::Rect<int> menuItemRect = (*it).rect;
			if( menuItemRect.top + menuItemRect.height > y
				&& menuItemRect.top < y 
				&& menuItemRect.left < x 
				&& menuItemRect.left + menuItemRect.width > x)
				{
					return (*it).action;
				}
		}

		return Nothing;
	}
}