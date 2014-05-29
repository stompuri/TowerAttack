#pragma once
#include "Effect.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

namespace TA
{
	class Grayer : public Effect
	{
	public :

		Grayer() : Effect("Grayer") {}

		bool onLoad();
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
		void Update(sf::RenderTexture & _texture);
		sf::Shader* getShader();

	private:
		sf::RenderTexture * m_texture;
		sf::Sprite m_sprite;
		sf::Shader m_shader;
	};
}