#pragma once

#include "Monster.h"
#include "MonsterData.h"
#include "MonsterColor.h"

class Lich : public Monster
{
public:
    Lich(
        const MonsterColor& color,
        const MonsterData& data,
        const sf::Vector2f& startPosition
    );

    void UpdateTypeLogic(float deltaTime) override;

    void Attack(Creature* target) override;

    void SetAttakRange(float range) override;

};