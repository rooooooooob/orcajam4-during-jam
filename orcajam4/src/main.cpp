#include "Game.hpp"
#include "World.hpp"
#include "TexManager.hpp"

int main()
{
	je::Game game(640, 480, 60);
	game.getTexManager().setPath("orcajam4/img/");
	game.setLevel(new orca::World(&game));
    return game.execute();
}
