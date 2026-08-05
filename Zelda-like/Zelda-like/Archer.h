#pragma once

#include "Player.h"

class Archer : public Player
{
public:
    Archer(
        const std::string& name,
        const PlayerData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateLogic(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
};

