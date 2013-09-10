#include "Boar.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Game.hpp"
#include "Trig.hpp"
#include "Blood.hpp"
#include "Random.hpp"
#include <iostream>

namespace orca
{

Boar::Boar (World * world, const sf::Vector2f& pos, Player *player) :
    Entity (world, "Boar", pos, sf::Vector2i(16, 16)),
    run (world->getGame().getTexManager().get("boar.png"), 16, 16, 10),
    target(player), health(50), stareTime (0), state (Roam), chargeDir(0),
    timer(0), hasHitPlayerDuringCharge(false), world(world)
{
	depth = -5;
    run.apply([](sf::Sprite& sprite){
        sprite.setOrigin(8, 8);
    });
}

void Boar::draw(sf::RenderTarget& target) const
{
    run.draw (target);
}

void Boar::update()
{
    if (health <= 0)
    {
        this->destroy();
    }
    int px = pos.x;
    int py = pos.y;
    run.apply([px, py](sf::Sprite& sprite){
        sprite.setPosition(px, py);
    });

    sf::Vector2f playerPos = target->getPos();
    switch (state)
    {
        case Roam:
        {
            if (je::pointDistance (pos, target->getPos()) <= 320)
                state = Follow;
            break;
        }
        break;

        case Follow:
		{
		    int dist = je::pointDistance (pos, playerPos);
		    if ((dist > 320))
            {
                state = Roam;
            }
			float direction = je::pointDirection(pos, playerPos);

			run.apply ([direction] (sf::Sprite& sprite)
			{
			   sprite.setRotation (-direction);
			});

			pos += je::lengthdir(1, je::pointDirection(pos, playerPos));
			run.advanceFrame();

			if (je::pointDistance (pos, playerPos) <= 112)
			{
				state = Stare;
				stareTime = 0;
			}
		}
        break;

        case Stare:
		{
			if (stareTime < 100)
			{
				float direction = je::pointDirection(pos, playerPos);

				run.apply ([direction] (sf::Sprite& sprite)
				{
				   sprite.setRotation (-direction);
				});

				++stareTime;
			}
			else
			{
				state = Charge;
				chargeDir = je::pointDirection(pos, playerPos);
				timer = 120;
				hasHitPlayerDuringCharge = false;
			}
		}
		break;

        case Charge:
		{
			int dist = je::pointDistance(pos, playerPos);

			sf::Vector2f velocity = je::lengthdir(2.5, chargeDir);
			if (!hasHitPlayerDuringCharge)
				velocity += je::lengthdir(3, je::pointDirection(pos, playerPos));

			float vDir = je::pointDirection(velocity);

			if (dist > 8)
				finalChargeDir = vDir;

			pos += velocity;

			run.apply([vDir](sf::Sprite& sprite){
				sprite.setRotation(-vDir);
			});
			run.advanceFrame();
			run.advanceFrame();

			//sf::Rect<float> boaR (pos, sf::Vector2f (16, 16));

			//if (boaR.contains(playerPos))
			if (dist < 16)
			{
				hasHitPlayerDuringCharge = true;
				target->damage(3);
				chargeDir = finalChargeDir;
				for (int r = 0; r < 4; ++r)
					world->addEntity(new Blood(world, pos, je::lengthdir(je::randomf(4), finalChargeDir - 10 + je::randomf(20))));
				for (int r = 0; r < 2; ++r)
					world->addEntity(new Blood(world, pos, je::lengthdir(je::randomf(2), je::randomf(360))));
			}
			if (timer == 0)
			{
				state = Roam;
			}
		}
		break;
    }
	if (timer >= 0)
		--timer;
}

void Boar::chop ()
{
	for (int r = 0; r < 3; ++r)
		world->addEntity(new Blood(world, pos, je::lengthdir(je::randomf(5), je::pointDirection(target->getPos(), pos))));
	for (int r = 0; r < 2; ++r)
		world->addEntity(new Blood(world, pos, je::lengthdir(je::randomf(2), je::randomf(360))));
    health -= 1;
}

}
