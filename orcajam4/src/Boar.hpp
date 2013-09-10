#ifndef BOAR_HPP_INCLUDED
#define BOAR_HPP_INCLUDED

#include "Entity.hpp"
#include "Animation.hpp"

namespace orca
{

class World;
class Player;

class Boar: public je::Entity
{

public:
    Boar (World * world, const sf::Vector2f& pos, Player *player);
    void draw(sf::RenderTarget& target) const override;
	void update() override;
	void chop();

private:
    enum StateType {Roam = 1, Follow = 2, Stare = 3, Charge = 4};
    je::Animation run;
    //je::Animation attack;
    Player* target;
    int health;
    int stareTime;
    StateType state;
    float chargeDir;
    float finalChargeDir;
    int timer;
    bool hasHitPlayerDuringCharge;
    World * world;

};
}


#endif // BOAR_HPP_INCLUDED
