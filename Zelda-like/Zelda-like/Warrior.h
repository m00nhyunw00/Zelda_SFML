#pragma once

#include "Player.h"

class Warrior : public Player
{
public:
    Warrior(
        const std::string& name,
        const PlayerData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateLogic(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
};

