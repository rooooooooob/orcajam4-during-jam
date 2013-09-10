#ifndef ORCA_BLOOD_HPP
#define ORCA_BLOOD_HPP

#include "Entity.hpp"

namespace orca
{

class World;

class Blood : public je::Entity
{
public:
	Blood(World * world, const sf::Vector2f& pos, const sf::Vector2f& velocity);

private:
	void update() override;
	void draw(sf::RenderTarget& target) const override;

	int lifetime;
	sf::Vector2f velocity;
	sf::Sprite sprite;
};

}

#endif // ORCA_BLOOD_HPP
