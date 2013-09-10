#include "Raft.hpp"

#include "Game.hpp"
#include "World.hpp"

namespace orca
{

Raft::Raft(World * world, const sf::Vector2f& pos, float angle)
	:Entity(world, "Raft", pos, sf::Vector2i(16, 16), sf::Vector2i(-8, -8))
	,sprite()
{
	sprite.setTexture(world->getGame().getTexManager().get("raft_empty.png"));
	sprite.setOrigin(16, 16);
	sprite.setPosition(pos);
	sprite.setRotation(-angle);

	/*if (world->getTerrain(pos.x, pos.y) == World::Terrain::Sand)
	{
		sf::Vector2f temp;
		for (int i = 0; i < 360; i += 36)
		{
			temp = pos + je::lengthdir(16, i);
			if (world->getTerrain(temp.x, temp.y))
			{
				pos = temp;
				break;
			}
		}
	}*/
}

void Raft::update()
{
}

void Raft::draw(sf::RenderTarget& target) const
{
	target.draw(sprite);
}

}
