#pragma once

#include "Player.h"
#include "Constants.h"

class Mage : public Player
{
private:
 
public:
    Mage(
        const std::string& name,
        const PlayerData& basicData,
        const PlayerLevelData& levelData,
        const PlayerSaveData& saveData,
        const sf::Vector2f& startPosition
    );

    void UpdateJobLogic(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
    void UseUltimate(Creature* target) override;
};