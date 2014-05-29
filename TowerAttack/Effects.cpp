////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Effects.h"

namespace TA
{
	////////////////////////////////////////////////////////////
	// "Grayer" fragment shader
	////////////////////////////////////////////////////////////
	bool Grayer::onLoad()
	{
		m_texture = new sf::RenderTexture();
		if (!m_texture->create(800, 600))
				assert(false);
		m_texture->clear(sf::Color::White);

		sf::Texture _texture;
		if (!_texture.loadFromFile("resources/images/upgrademenu.png"))
            return false;
        m_sprite.setTexture(_texture);

		// Load the shader
		if (!m_shader.loadFromFile("resources/grayer.frag", sf::Shader::Fragment))
			return false;
        m_shader.setParameter("texture", sf::Shader::CurrentTexture);

		return true;
	}

	void Grayer::Update(sf::RenderTexture & _texture)
	{
		m_texture = &_texture;
		m_sprite.setTexture(m_texture->getTexture());
		m_shader.setParameter("texture", sf::Shader::CurrentTexture);
	}

    void Grayer::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.shader = &m_shader;
        target.draw(m_sprite, states);
    }

	sf::Shader* Grayer::getShader()
	{
		return &m_shader;
	}
}