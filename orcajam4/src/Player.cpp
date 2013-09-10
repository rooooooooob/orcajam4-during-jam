#include "Player.hpp"
#include "Tree.hpp"
#include "Boar.hpp"

#include "TexManager.hpp"

#include "World.hpp"
#include "Game.hpp"
#include "Trig.hpp"
#include "Level.hpp"
#include "Raft.hpp"

#include <iostream>

static const int DROWNING_ANIMATION_TIME = 60;


static const float RAFT_ACCEL = 0.005;
static const float RAFT_DEACCEL = 0.001;
static const float RAFT_TURNING_SPEED = 0.5;

namespace orca
{

Player::Player(World * world, const sf::Vector2f& pos)
	:Entity(world, "Player", pos, sf::Vector2i(16, 16), sf::Vector2i(-8, -8))
	,world(world)
	,controller(world->getGame().getInput())
	,legs(world->getGame().getTexManager().get("player_legs.png"), 16, 16, 5)
	,walking(world->getGame().getTexManager().get("player_walking.png"), 16, 16, 6)
	,attacking(world->getGame().getTexManager().get("player_attacking.png"), 16, 16, 10, false)
	,drowning(world->getGame().getTexManager().get("player_drowning.png"), 16, 16, 8, false)
	,speed(2)
	,state(State::Walking)
	,timer(-1)
	,hp(100)
	,raftDirection(0)
	,raftVelocity(0)
{
	legs.apply([](sf::Sprite& sprite){
		sprite.setOrigin(8, 8);
	});
	walking.apply([](sf::Sprite& sprite){
		sprite.setOrigin(4, 8);
	});
	attacking.apply([](sf::Sprite& sprite){
		sprite.setOrigin(4, 8);
	});
	drowning.apply([](sf::Sprite& sprite){
		sprite.setOrigin(8, 8);
	});

	stunned.setTexture(world->getGame().getTexManager().get("player.png"));
	stunned.setOrigin(4, 8);

	raft.setTexture(world->getGame().getTexManager().get("raft.png"));
	raft.setOrigin(16, 16);

	controller.setKeybinds("up", {je::Controller::Bind(sf::Keyboard::Up), je::Controller::Bind(sf::Keyboard::W)});
	controller.setKeybinds("left", {je::Controller::Bind(sf::Keyboard::Left), je::Controller::Bind(sf::Keyboard::A)});
	controller.setKeybinds("down", {je::Controller::Bind(sf::Keyboard::Down), je::Controller::Bind(sf::Keyboard::S)});
	controller.setKeybinds("right", {je::Controller::Bind(sf::Keyboard::Right), je::Controller::Bind(sf::Keyboard::D)});
	controller.setKeybinds("attack", {je::Controller::Bind(sf::Mouse::Button::Left)});
	controller.setKeybinds("use_raft", {je::Controller::Bind(sf::Keyboard::E)});
	depth = -10;
}

void Player::draw(sf::RenderTarget& target) const
{
	switch (state)
	{
		case State::Walking:
			legs.draw(target);
			walking.draw(target);
			break;
		case State::Attacking:
			legs.draw(target);
			attacking.draw(target);
			break;
		case State::Stunned:
			legs.draw(target);
			target.draw(stunned);
			break;
		case State::Rafting:
			target.draw(raft);
			break;
		case State::Drowning:
			drowning.draw(target);
			break;
	}
}

void Player::update()
{
	float mouseAim = -je::pointDirection(pos, level->getCursorPos());
	bool isMoving = false;
	sf::Vector2f dir(0, 0);
	if (controller.isActionHeld("left"))
	{
		dir.x -= 1;
		isMoving = true;
	}
	if (controller.isActionHeld("right"))
	{
		dir.x += 1;
		isMoving = true;
	}
	if (controller.isActionHeld("up"))
	{
		dir.y -= 1;
		isMoving = true;
	}
	if (controller.isActionHeld("down"))
	{
		dir.y += 1;
		isMoving = true;
	}
	dir = je::lengthdir(speed, je::pointDirection(sf::Vector2f(0, 0), dir));

	switch (state)
	{
		case State::Walking:
			{
				int px = pos.x;	//	for lambda
				int py = pos.y;
				int dx = dir.x;
				int dy = dir.y;
				if (isMoving &&
					!level->testCollision(this, "Tree", dir.x, dir.y) &&
					!level->testCollision(this, "SolidTerrain", dir.x, dir.y))
				{
					if (world->getTerrain(pos.x, pos.y) == World::Terrain::ShallowWater)
					{
						pos.x += dir.x / 3;
						pos.y += dir.y / 3;
					}
					else
						pos += dir;
					walking.advanceFrame();
					legs.advanceFrame();
					legs.apply([dx, dy, px, py](sf::Sprite& sprite){
						sprite.setPosition(px, py);
						sprite.setRotation(je::pointDirection(0, 0, dx, dy));
					});
				}
				else
				{
					legs.apply([mouseAim, px, py](sf::Sprite& sprite){
						sprite.setPosition(px, py);
						sprite.setRotation(mouseAim);
					});
				}
				walking.apply([mouseAim, px, py](sf::Sprite& sprite){
					sprite.setPosition(px, py);
					sprite.setRotation(mouseAim);
				});
				if (world->getTerrain(pos.x, pos.y) == World::Terrain::NormalWater)
				{
					timer = DROWNING_ANIMATION_TIME;
					state = State::Drowning;
					drowning.apply([px, py](sf::Sprite& sprite){
						sprite.setPosition(px, py);
					});
				}
				if (controller.isActionPressed("attack"))
				{
					attacking.reset();
					attacking.apply([mouseAim, px, py](sf::Sprite& sprite){
						sprite.setPosition(px, py);
						sprite.setRotation(mouseAim);
					});
					state = State::Attacking;
					break;
				}
				else if (controller.isActionPressed("use_raft"))
				{
					Raft *raft = (Raft*) world->testCollision(this, "Raft");
					if (raft)
					{
						state = State::Rafting;
						raft->destroy();
						break;
					}
				}
			}
			break;
		case State::Attacking:
			if (attacking.isFinished())
			{
				state = State::Walking;
			}
			else
            {
				attacking.advanceFrame();
				std::vector<je::Entity*> hitList;
				//std::cout << "x coor: " << je::lengthdirX (8, mouseAim) << "  y coor: " << je::lengthdirY (8, mouseAim) << "\n";
				world->findCollisions(hitList, this, "Tree", je::lengthdirX (8, mouseAim), je::lengthdirY (8, mouseAim));
				for (je::Entity* tree : hitList)
                {
                    std::cout << "chop";
                    ((Tree*)tree)->chop();
                }

                world->findCollisions(hitList, this, "Boar", je::lengthdirX (8, mouseAim), je::lengthdirY (8, mouseAim));
				for (je::Entity* tree : hitList)
                {
                    std::cout << "chop";
                    ((Boar*)tree)->chop();
                }
            }
			break;
		case State::Stunned:
			stunned.setPosition(pos);
			stunned.setRotation(mouseAim);
			break;
		case State::Rafting:
			if (controller.isActionHeld("right"))
			{
				raftDirection -= RAFT_TURNING_SPEED;
			}
			if (controller.isActionHeld("left"))
			{
				raftDirection += RAFT_TURNING_SPEED;
			}
			if (controller.isActionHeld("up"))
			{
				raftVelocity += RAFT_ACCEL;
			}
			if (controller.isActionHeld("down"))
			{
				raftVelocity -= RAFT_ACCEL;
			}

			if (raftVelocity > RAFT_DEACCEL)
				raftVelocity -= RAFT_DEACCEL;
			else if (raftVelocity < -RAFT_DEACCEL)
				raftVelocity += RAFT_DEACCEL;
			else
				raftVelocity = 0;

			if (raftVelocity > 1)
				raftVelocity = 1;
			else if (raftVelocity < -1)
				raftVelocity = -1;

			raft.setPosition(pos);
			raft.setRotation(-raftDirection);
			pos += je::lengthdir(raftVelocity, raftDirection);

			if (world->getTerrain(pos.x, pos.y) == World::Terrain::Sand ||
				controller.isActionPressed("use_raft"))
			{
				world->addEntity(new Raft(world, prevPos - je::lengthdir(raftVelocity * 4, raftDirection), raftDirection));
				raftVelocity = 0;
				state = State::Walking;
				break;
			}

			break;
		case State::Drowning:
			drowning.advanceFrame();
			if (drowning.isFinished())
				hp -= 2;
			/*if (timer == 0)
			{
				world->reset();
			}*/
			break;
		default:
			break;
	}
	if (timer >= 0)
		--timer;
	level->setCameraPosition(pos);
	if (hp <= 0)
		world->reset();
	prevPos = pos;
}

void Player::damage(int amount)
{
	hp -= amount;
}

int Player::getHp() const
{
	return hp;
}

}
