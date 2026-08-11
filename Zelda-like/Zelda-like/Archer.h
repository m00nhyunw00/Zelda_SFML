#pragma once

#include "Player.h"

class Archer : public Player
{
private:
    bool isTripleShot;
    bool isTitanArrow;

    int tripleShotCount;

    float tripleShotTimer;
    float tripleShotInterval;

    Creature* tripleShotTarget;

public:
    Archer(
        const std::string& name,
        const PlayerData& basicData,
        const PlayerLevelData& levelData,
        const PlayerSaveData& saveData,
        const sf::Vector2f& startPosition
    );

    Projectile* CreateArrow(Creature* target, int damage, float scale, ProjectileAttackType type);

    void UpdateJobLogic(float deltaTime) override;
    bool HandleJobAnimation(float deltaTime) override;

    void Attack(Creature* target) override;
    void UseSkill(Creature* target) override;
    void UseUltimate(Creature* target) override;
};

