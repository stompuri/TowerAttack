#include "StateSplash.h"
#include "StateMenu.h"
#include "stdafx.h"

namespace TA
{
	StateSplash StateSplash::m_SplashState;

	void StateSplash::Init()
	{
		if(debug) printf("StateSplash Init\n");
	}

	void StateSplash::Cleanup()
	{
		if(debug) printf("StateSplash Cleanup\n");
	}

	void StateSplash::Pause()
	{
		if(debug) printf("StateSplash Pause\n");
	}

	void StateSplash::Resume()
	{
		if(debug) printf("StateSplash Resume\n");
	}

	void StateSplash::HandleEvents(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed 
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed )
			{
				Game::ChangeState( StateMenu::Instance() );
			}
		}
	}

	void StateSplash::Update() 
	{

	}

	void StateSplash::Draw(sf::RenderWindow& window) 
	{
		sf::Texture image;
		if (!image.loadFromFile("resources/images/splashscreen.png"))
			throw(EXIT_FAILURE);

		// Create the sprite
		sf::Sprite sprite(image);
  
		window.draw(sprite);
		window.display();
	}
}