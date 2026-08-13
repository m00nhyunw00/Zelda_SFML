#pragma once

#include "Monster.h"
#include "MonsterData.h"
#include "MonsterColor.h"

class Skeleton : public Monster
{
public:
    Skeleton(
        const MonsterColor& color,
        const MonsterData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateTypeLogic(float deltaTime) override;

    void Attack(Creature* target) override;
};
