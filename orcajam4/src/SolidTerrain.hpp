#ifndef JE_SOLIDTERRAIN_HPP
#define JE_SOLIDTERRAIN_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

namespace orca
{

class World;

class SolidTerrain : public je::Entity
{
public:
	SolidTerrain(World * world, const sf::Vector2f& pos, const std::string& name);

	void update() override;

	void draw(sf::RenderTarget& target) const override;

private:

	sf::Sprite sprite;
};

}

#endif // JE_TREE_HPP
