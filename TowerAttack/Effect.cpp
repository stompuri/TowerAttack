#include "Effect.h"

namespace TA
{
	const std::string& Effect::getName() const
	{
		return m_name;
	}
	
	void Effect::load()
	{
		m_isLoaded = sf::Shader::isAvailable() && onLoad();
	}

	void Effect::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (m_isLoaded)
        {
            onDraw(target, states);
        }
        else
        {
            std::cout << "Failed to draw shader\n";
        }
    }
}