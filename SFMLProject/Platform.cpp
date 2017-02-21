#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Book/Platform.hpp"



Textures::ID toTextureID(Platform::Type type)
{
	switch (type)
	{
	case Platform::platform:
		return Textures::platform;

	}
	return Textures::platform;
}

Platform::Platform(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(0.f, 0.f);
}

void Platform::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Platform::getCategory() const
{
	switch (mType)
	{
	case platform:
		return Category::PlayerAircraft;
	}
}