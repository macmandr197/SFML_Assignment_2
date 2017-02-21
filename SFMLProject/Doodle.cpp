#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Doodle.hpp"


Textures::ID toTextureID(Doodle::Type type)
{
	switch (type)
	{
	case Doodle::DoodlePlayer:
		return Textures::DoodlePlayer;

	}
	return Textures::DoodlePlayer;
}

Doodle::Doodle(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Doodle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

unsigned int Doodle::getCategory() const
{
	switch (mType)
	{
	case DoodlePlayer:
		return Category::PlayerAircraft;
	}
}