#ifndef ORCA_WORLD_HPP
#define ORCA_WORLD_HPP

#include "Level.hpp"

namespace orca
{

class Player;

class World : public je::Level
{
public:
	enum class Terrain
	{
		DeepWater,
		NormalWater,
		ShallowWater,
		Sand,
		Grass
	};
	World(je::Game * const game);
	~World();

	/**
	 * Resets the World at the end of the current update
	 */
	void reset();

	Terrain getTerrain(int x, int y) const;

private:
	void onUpdate() override;
	void onDraw(sf::RenderTarget& target) const override;
	void drawGUI(sf::RenderTarget& target) const override;
	void loadEntities(const std::string& layerName, const std::vector<EntityPrototype>& prototypes) override;
	void transformTiles(const std::string& layerName, int tilesAcross, int tilesHigh, unsigned  **tiles) override;

	/**
	 * Code to actually reset world, called after update finishes on the update
	 * that reset() is called
	 */
	void actuallyReset();

	Terrain **terrain;
	std::vector<EntityPrototype> prototypes;
	bool resetOnNextTurn;

	Player *player;

	sf::RectangleShape hpbar;
	sf::RectangleShape hpbarBack;

	int score;
};

}

#endif
