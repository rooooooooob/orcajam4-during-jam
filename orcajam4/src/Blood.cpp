#include "Blood.hpp"

#include "World.hpp"
#include "Random.hpp"
#include "Game.hpp"

namespace orca
{

Blood::Blood(World * world, const sf::Vector2f& pos, const sf::Vector2f& velocity)
	:Entity(world, "Blood", pos, sf::Vector2i(4, 4))
	,lifetime(je::random(90) + 30)
	,velocity(velocity)
{
	sprite.setTexture(world->getGame().getTexManager().get("blood.png"));
	int offset = je::random(3) * 4;
	sprite.setTextureRect(sf::IntRect(offset, 0, 4, 4));
	sprite.setPosition(pos);
}

void Blood::update()
{
	if (lifetime > 45)
	{
		sprite.setPosition(pos);
		pos += velocity;
	}

	if (--lifetime <= 0)
		this->destroy();
}

void Blood::draw(sf::RenderTarget& target) const
{
	target.draw(sprite);
}

}
