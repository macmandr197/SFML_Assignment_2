#pragma once

#include <Book/Entity.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Platform : public Entity
{
public:
	enum Type
	{
		platform,
	};


public:
	Platform(Type type, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;



private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type				mType;
	sf::Sprite			mSprite;
};