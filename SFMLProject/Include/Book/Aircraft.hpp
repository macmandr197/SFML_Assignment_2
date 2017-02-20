#pragma once

#include <Book/Entity.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};


	public:
							Aircraft(Type type, const TextureHolder& textures);
							virtual unsigned int	getCategory() const;



	private:
		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type				mType;
		sf::Sprite			mSprite;
};


