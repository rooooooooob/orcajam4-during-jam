#ifndef ORCA_RAFT_HPP
#define ORCA_RAFT_HPP

#include "Entity.hpp"

namespace orca
{

class World;

class Raft : public je::Entity
{
public:
	Raft(World * world, const sf::Vector2f& pos, float angle);

private:
	void update() override;
	void draw(sf::RenderTarget& target) const override;

	sf::Sprite sprite;
};

}

#endif // ORCA_RAFT_HPP
