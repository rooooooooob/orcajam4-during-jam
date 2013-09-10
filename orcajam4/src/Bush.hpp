#ifndef JE_BUSH_HPP
#define JE_BUSH_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace orca
{

class World;

class Bush : public je::Entity
{
public:
	Bush(World * world, const sf::Vector2f& pos);

	void update() override;

	void draw(sf::RenderTarget& target) const override;

private:

	sf::Sprite sprite;
	int hp;
};

}

#endif // JE_TREE_HPP
