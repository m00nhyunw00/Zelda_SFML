#pragma once

#include "Player.h"

class Mage : public Player
{
private:
 
public:
    Mage(
        const std::string& name,
        const PlayerData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateJobLogic(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
    void UseUltimate(Creature* target) override;
};

