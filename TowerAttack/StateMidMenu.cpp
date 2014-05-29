#include "StateMidMenu.h"
#include "StateMenu.h"
#include "StatePlay.h"
#include "stdafx.h"

namespace TA
{
	sf::Sprite *StateMidMenu::p_sprite;
	sf::Texture StateMidMenu::image;
	StateMidMenu StateMidMenu::m_MenuState;
	StateMidMenu::MenuResult StateMidMenu::action;
	std::list<StateMidMenu::MenuItem> StateMidMenu::menuItems;

	void StateMidMenu::Init()
	{
		//Load menu image from file
		if (! image.loadFromFile("resources/images/midmenu.png"))
			throw(EXIT_FAILURE);
		p_sprite = new sf::Sprite(image);

		// Set up clickable regions
		// Play - menu item coordinates
		MenuItem playButton;
		playButton.rect.top= 185;
		playButton.rect.height = 113;
		playButton.rect.left = 278;
		playButton.rect.width = 243;
		playButton.action = Play;

		// Exit - menu item coordinates
		MenuItem exitButton;
		exitButton.rect.top = 417;
		exitButton.rect.height = 106;
		exitButton.rect.left = 303;
		exitButton.rect.width = 192;
		exitButton.action = Exit;

		MenuItem mainMenuButton;
		exitButton.rect.top = 304;
		exitButton.rect.height = 104;
		exitButton.rect.left = 232;
		exitButton.rect.width = 341;
		exitButton.action = Mainmenu;

		menuItems.push_back(playButton);
		menuItems.push_back(mainMenuButton);
		menuItems.push_back(exitButton);
		printf("StateMidMenu Init\n");
	}

	void StateMidMenu::Cleanup()
	{
		printf("StateMidMenu Cleanup\n");
	}

	void StateMidMenu::Pause()
	{
		printf("StateMidMenu Pause\n");
	}

	void StateMidMenu::Resume()
	{
		printf("StateMidMenu Resume\n");
	}

	void StateMidMenu::HandleEvents(sf::RenderWindow& window)
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

	void StateMidMenu::Update() 
	{
		if (action == Exit) {
			std::cout << "Exiting...";
			Game::Quit();
		} else if (action == Play) {
			std::cout << "Resuming...";
			Game::GetElapsedTime();
			Game::ChangeState( StatePlay::Instance() );
			action = Nothing;
		} else if (action == Mainmenu) {
			std::cout << "Main menu...";
			Game::ResetGame();
			Game::ChangeState( StateMenu::Instance() );
			action = Nothing;
		}
	}

	void StateMidMenu::Draw(sf::RenderWindow& window) 
	{
		window.draw(*p_sprite);
		window.display();
	}

	StateMidMenu::MenuResult StateMidMenu::HandleClick(int x, int y)
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