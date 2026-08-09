#pragma once

#include "Player.h"

class Archer : public Player
{
private:
    bool isTripleShot;

    int tripleShotCount;

    float tripleShotTimer;
    float tripleShotInterval;

    Creature* tripleShotTarget;

public:
    Archer(
        const std::string& name,
        const PlayerData& data,
        const sf::Vector2f& startPosition
    );

    Projectile* CreateArrow(Creature* target, int damage, float scale);

    void UpdateJobLogic(float deltaTime) override;
    bool HandleJobAnimation(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
};

