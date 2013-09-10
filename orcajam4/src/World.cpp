#include "World.hpp"

#include <iostream>
#include <sstream>
#include <cassert>
#include "Random.hpp"
#include "Player.hpp"
#include "Boar.hpp"
#include "Tree.hpp"
#include "SolidTerrain.hpp"
#include "Bush.hpp"
#include "Raft.hpp"
#include "Game.hpp"

const int HP_BAR_WIDTH = 192;
const int HP_BAR_HEIGHT = 24;

namespace orca
{

World::World(je::Game * const game)
	:Level(game, "orcajam4/levels/world.tmx")
	,terrain(nullptr)
	,resetOnNextTurn(false)
	,player(nullptr)
	,score(0)
{
	hpbar.setPosition(32, 16);
	hpbar.setSize(sf::Vector2f(HP_BAR_WIDTH, HP_BAR_HEIGHT));
	hpbar.setFillColor(sf::Color::Red);

	hpbarBack.setPosition(32, 16);
	hpbarBack.setSize(sf::Vector2f(HP_BAR_WIDTH, HP_BAR_HEIGHT));
	hpbarBack.setFillColor(sf::Color::Black);
	hpbarBack.setOutlineColor(sf::Color::White);
	hpbarBack.setOutlineThickness(2);

	this->setCameraBounds(sf::Rect<int>(0, 0, 640, 480));
	this->loadMap("orcajam4/levels/world.tmx");

	this->reset();
}

World::~World()
{
	if (terrain != nullptr)
	{
		for (int i = 0; i < getWidth() / 16; ++i)
			delete[] terrain[i];
		delete[] terrain;
	}
}

void World::reset()
{
	resetOnNextTurn = true;
}

World::Terrain World::getTerrain(int x, int y) const
{
	assert(x >= 0 && x < getWidth() && y >= 0 && y < getHeight());
	return terrain[x / 16][y / 16];
}

void World::onUpdate()
{
	if (resetOnNextTurn)
		this->actuallyReset();
	if (player)
	{
		hpbar.setSize(sf::Vector2f((player->getHp() / 100.f) * HP_BAR_WIDTH, hpbar.getSize().y));
	}
	++score;
	std::stringstream ss;
	ss << "Score: " << score;
	getGame().setTitle(ss.str());
}

void World::onDraw(sf::RenderTarget& target) const
{
}

void World::drawGUI(sf::RenderTarget& target) const
{
	if (player)
	{
		target.draw(hpbarBack);
		target.draw(hpbar);
	}
}

void World::loadEntities(const std::string& layerName, const std::vector<EntityPrototype>& prototypes)
{
	this->prototypes = prototypes;
}

void World::transformTiles(const std::string& layerName, int tilesAcross, int tilesHigh, unsigned **tiles)
{
	if (terrain == nullptr)
	{
		terrain = new Terrain*[getWidth() / 16];
		for (int i = 0; i < getWidth() / 16; ++i)
		{
			terrain[i] = new Terrain[getHeight() / 16];
			for (int j = 0; j < getHeight() / 16; ++j)
				terrain[i][j] = Terrain::DeepWater;
		}
	}
	std::cout << "World::transformTiles()\n";
	//	make a copy of the tiles to refer to while transforming
	unsigned int **untransformed = new unsigned int*[tilesAcross];
	for (int i = 0; i < tilesAcross; ++i)
	{
		untransformed[i] = new unsigned int[tilesHigh];
		for (int j = 0; j < tilesHigh; ++j)
			untransformed[i][j] = tiles[i][j];
	}
	/**		modify tiles here (this is your job Max)		**/
	if (layerName == "terrain")
	{
	    enum TileType { Water = 0, Sand = 10,Grass = 34 };

		for (int x = 0; x < tilesAcross; ++x)
		{
			for (int y = 0; y < tilesHigh; ++y)
			{
                if (untransformed[x][y] == Sand)
                {
                	terrain[x][y] = Terrain::Sand;
                    if ((untransformed[x - 1][y] == Water) && (untransformed[x + 1][y] == Water) && (untransformed[x][y + 1] == Water))
                    {
                        tiles[x][y] = 16;
                    }
                    else if ((untransformed[x - 1][y] == Water) && (untransformed[x + 1][y] == Water) && (untransformed[x][y - 1] == Water))
                    {
                        tiles[x][y] = 15;
                    }
                    else if ((untransformed[x][y - 1] == Water) && (untransformed[x][y + 1] == Water) && (untransformed[x + 1][y] == Water))
                    {
                        tiles[x][y] = 8;
                    }
                    else if ((untransformed[x][y - 1] == Water) && (untransformed[x][y + 1] == Water) && (untransformed[x - 1][y] == Water))
                    {
                        tiles[x][y] = 7;
                    }
                    else if ((untransformed[x - 1][y] == Water) && (untransformed[x][y - 1] == Water))
                    {
                        tiles[x][y] = 1;
                    }
                    else if ((untransformed[x - 1][y] == Water) && (untransformed[x][y + 1] == Water))
                    {
                        tiles[x][y] = 17;
                    }
                    else if ((untransformed[x + 1][y] == Water) && (untransformed[x][y - 1] == Water))
                    {
                        tiles[x][y] = 3;
                    }
                    else if ((untransformed[x + 1][y] == Water) && (untransformed[x][y + 1] == Water))
                    {
                        tiles[x][y] = 19;
                    }
                    else if (untransformed[x + 1][y] == Water)
                    {
                        tiles[x][y] = 11;
                    }
                    else if (untransformed[x - 1][y] == Water)
                    {
                        tiles[x][y] = 9;
                    }
                    else if (untransformed[x][y + 1] == Water)
                    {
                        tiles[x][y] = 18;
                    }
                    else if (untransformed[x][y - 1] == Water)
                    {
                        tiles[x][y] = 2;
                    }
                }
                else if (untransformed[x][y] == Grass)
                {
                	terrain[x][y] = Terrain::Grass;
                    if ((untransformed[x - 1][y] == Sand) && (untransformed[x + 1][y] == Sand) && (untransformed[x][y + 1] == Sand))
                    {
                        tiles[x][y] = 32;
                    }
                    else if ((untransformed[x - 1][y] == Sand) && (untransformed[x + 1][y] == Sand) && (untransformed[x][y - 1] == Sand))
                    {
                        tiles[x][y] = 31;
                    }
                    else if ((untransformed[x][y - 1] == Sand) && (untransformed[x][y + 1] == Sand) && (untransformed[x + 1][y] == Sand))
                    {
                        tiles[x][y] = 24;
                    }
                    else if ((untransformed[x][y - 1] == Sand) && (untransformed[x][y + 1] == Sand) && (untransformed[x - 1][y] == Sand))
                    {
                        tiles[x][y] = 23;
                    }
                    else if ((untransformed[x - 1][y] == Sand) && (untransformed[x][y - 1] == Sand))
                    {
                        tiles[x][y] = 25;
                    }
                    else if ((untransformed[x - 1][y] == Sand) && (untransformed[x][y + 1] == Sand))
                    {
                        tiles[x][y] = 41;
                    }
                    else if ((untransformed[x + 1][y] == Sand) && (untransformed[x][y - 1] == Sand))
                    {
                        tiles[x][y] = 27;
                    }
                    else if ((untransformed[x + 1][y] == Sand) && (untransformed[x][y + 1] == Sand))
                    {
                        tiles[x][y] = 43;
                    }
                    else if (untransformed[x + 1][y] == Sand)
                    {
                        tiles[x][y] = 35;
                    }
                    else if (untransformed[x - 1][y] == Sand)
                    {
                        tiles[x][y] = 33;
                    }
                    else if (untransformed[x][y + 1] == Sand)
                    {
                        tiles[x][y] = 42;
                    }
                    else if (untransformed[x][y - 1] == Sand)
                    {
                        tiles[x][y] = 26;
                    }
                    else if (untransformed[x + 1][y + 1] == Sand)
                    {
                        tiles[x][y] = 28;
                    }
                    else if (untransformed[x + 1][y - 1] == Sand)
                    {
                        tiles[x][y] = 44;
                    }
                    else if (untransformed[x - 1][y + 1] == Sand)
                    {
                        tiles[x][y] = 30;
                    }
                    else if (untransformed[x + 1][y + 1] == Sand)
                    {
                        tiles[x][y] = 46;
                    }
                    else
                    {
                        unsigned short int rand = je::random(20);

                        if (rand == 0)
                        {
                            tiles[x][y] = 5;
                        }
                        else if (rand == 1)
                        {
                            tiles[x][y] = 12;
                        }
                        else if (rand == 2)
                        {
                            tiles[x][y] = 14;
                        }
                        else if (rand == 3)
                        {
                            tiles[x][y] = 21;
                        }
                    }
                }
			}
		}
	}
	else if (layerName == "water")
	{
	    enum WaterType { Shallow = 70, Normal = 78, Deep = 71 };

		for (int x = 1; x < tilesAcross - 1; ++x)
		{
			for (int y = 1; y < tilesHigh - 1; ++y)
			{
                if (untransformed[x][y] == Shallow)
                {
                	terrain[x][y] = Terrain::ShallowWater;
                    if ((untransformed[x - 1][y] == Normal) && (untransformed[x][y - 1] == Normal))
                    {
                        tiles[x][y] = 82;
                    }
                    else if ((untransformed[x - 1][y] == Normal) && (untransformed[x][y + 1] == Normal))
                    {
                        tiles[x][y] = 90;
                    }
                    else if ((untransformed[x + 1][y] == Normal) && (untransformed[x][y - 1] == Normal))
                    {
                        tiles[x][y] = 84;
                    }
                    else if ((untransformed[x + 1][y] == Normal) && (untransformed[x][y + 1] == Normal))
                    {
                        tiles[x][y] = 92;
                    }
                    else if (untransformed[x + 1][y] == Normal)
                    {
                        tiles[x][y] = 93;
                    }
                    else if (untransformed[x - 1][y] == Normal)
                    {
                        tiles[x][y] = 85;
                    }
                    else if (untransformed[x][y + 1] == Normal)
                    {
                        tiles[x][y] = 91;
                    }
                    else if (untransformed[x][y - 1] == Normal)
                    {
                        tiles[x][y] = 83;
                    }
                }
                else if (untransformed[x][y] == Normal)
                {
                	terrain[x][y] = Terrain::NormalWater;
                    if ((untransformed[x - 1][y] == Deep) && (untransformed[x][y - 1] == Deep))
                    {
                        tiles[x][y] = 114;
                    }
                    else if ((untransformed[x - 1][y] == Deep) && (untransformed[x][y + 1] == Deep))
                    {
                        tiles[x][y] = 122;
                    }
                    else if ((untransformed[x + 1][y] == Deep) && (untransformed[x][y - 1] == Deep))
                    {
                        tiles[x][y] = 116;
                    }
                    else if ((untransformed[x + 1][y] == Deep) && (untransformed[x][y + 1] == Deep))
                    {
                        tiles[x][y] = 124;
                    }
                    else if (untransformed[x + 1][y] == Deep)
                    {
                        tiles[x][y] = 125;
                    }
                    else if (untransformed[x - 1][y] == Deep)
                    {
                        tiles[x][y] = 117;
                    }
                    else if (untransformed[x][y + 1] == Deep)
                    {
                        tiles[x][y] = 123;
                    }
                    else if (untransformed[x][y - 1] == Deep)
                    {
                        tiles[x][y] = 115;
                    }
                }
				else
					terrain[x][y] == Terrain::DeepWater;
            }
        }
	}
	for (int i = 0; i < tilesAcross; ++i)
		delete[] untransformed[i];
	delete[] untransformed;
}

void World::actuallyReset()
{
	this->clearEntities();

    player = new Player(this, sf::Vector2f(90 * 16 - 8, 200 * 16 - 8));
	this->addEntity(player);
	this->addEntity(new Boar(this, sf::Vector2f(100 * 16 - 8, 200 * 16 - 8), player));
	this->addEntity(new Raft(this, sf::Vector2f(92 * 16, 123 * 16), 0));

	for (const je::Level::EntityPrototype& obj : prototypes)
	{
		/*std::cout << "----------------ENTITY-------------";
		std::cout << "id: " << obj.id
				  << "\npos: (" << obj.x << "," << obj.y << ")"
				  << "\nname: " << obj.name
				  << "\ntype: " << obj.type << "\n\n";
		std::cout << "-----------------------------------";*/
		const sf::Vector2f pos(obj.x, obj.y);
		if (obj.id == 65)
		{
			je::Entity *entity = new Tree(this, pos);
			this->addEntity(entity);
		}
		else if (obj.id == 130)
		{
			je::Entity *entity = new SolidTerrain(this, pos, "rock.png");
			this->addEntity(entity);
		}
		else if (obj.id == 131)
		{
			je::Entity *entity = new Bush(this, pos);
			this->addEntity(entity);
		}
		else if (obj.id == 132)
        {
            je::Entity *entity = new Boar (this, pos, player);
            this->addEntity (entity);
        }
	}

	if (tileLayers.count("terrain"))
	{
		tileLayers["terrain"]->setDepth(10);
	}

	if (tileLayers.count("water"))
	{
		tileLayers["water"]->setDepth(20);
	}

	resetOnNextTurn = false;
}

}
