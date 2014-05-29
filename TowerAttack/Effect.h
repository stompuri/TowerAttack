#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cassert>
#include <string>
#include "stdafx.h"

namespace TA
{
	////////////////////////////////////////////////////////////
	// Base class for effects
	////////////////////////////////////////////////////////////
	class Effect : public sf::Drawable
	{
	public :

		virtual ~Effect()
		{
		}

		const std::string& getName() const;

		void load();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
		// Virtual functions to be implemented in derived effects
		virtual bool onLoad() = 0;
		virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
		virtual sf::Shader* getShader() = 0;
		virtual void Update(sf::RenderTexture & _texture) = 0;

	protected :
		Effect(const std::string& name) : m_name(name),	m_isLoaded(false) {	}

	private :

		std::string m_name;
		bool m_isLoaded;
	};
}