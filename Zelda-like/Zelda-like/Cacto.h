#pragma once

#include "Monster.h"
#include "MonsterData.h"
#include "MonsterColor.h"

class Cacto : public Monster
{
public:
    Cacto(
        const MonsterColor& color,
        const MonsterData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateTypeLogic(float deltaTime) override;

    void Attack(Creature* target) override;
};
