#include "SolidTerrain.hpp"

#include "Random.hpp"
#include "World.hpp"
#include "Game.hpp"

namespace orca
{

SolidTerrain::SolidTerrain(World * world, const sf::Vector2f& pos, const std::string& name)
	:Entity(world, "SolidTerrain", pos, sf::Vector2i(0, 0))
{
	this->pos.y -= 32;
	const sf::Texture& tex = world->getGame().getTexManager().get(name);
	this->setDimensions(tex.getSize().x, tex.getSize().y);
	sprite.setTexture(tex);
	sprite.setPosition(this->pos);
}

void SolidTerrain::update()
{
}

void SolidTerrain::draw(sf::RenderTarget& target) const
{
	target.draw(sprite);
}

}
