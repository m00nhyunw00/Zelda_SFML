#pragma once

#include "Player.h"
#include "BladeWave.h"
#include <unordered_set>

class Warrior : public Player
{
private:
    bool isPowerStrike;
    std::unordered_set<Creature*> powerStrikeHitTargets;

    bool isBladeSweep;

public:
    Warrior(
        const std::string& name,
        const PlayerData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateJobLogic(float deltaTime) override;
    bool HandleJobAnimation(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
    void UseUltimate(Creature* target) override;

    void HitPowerStrike(Creature* target);
    bool IsPowerStrike() { return isPowerStrike; }

    void CreateBladeSweep();
};

