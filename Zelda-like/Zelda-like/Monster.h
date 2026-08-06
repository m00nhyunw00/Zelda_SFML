#pragma once

#include "Creature.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "MonsterData.h"
#include <string>

class Monster : public Creature
{
private:
    MonsterType type;
    MonsterColor color;

    int exp;

private:
    sf::Vector2f AIMovement(float deltaTime);
    void HandleAnimation(const sf::Vector2f& direction, float deltaTime);

protected:
    Monster(
        MonsterType type,
        const MonsterData& data,
        const sf::Vector2f& position
    );

    void UpdateLogic(float deltaTime) override final;
    virtual void UpdateTypeLogic(float deltaTime) = 0;

    virtual void Attack(Creature* target) override = 0;

public:
    ~Monster() override = default;

    // Getter -------------------------------------------------

    MonsterType GetType() const { return type; }
    MonsterColor GetColor() const { return color; }
    int GetExp() const { return exp; }

    // ---------------------------------------------------------

    // Setter -------------------------------------------------

    void SetType(MonsterType type) { this->type = type; }
    void SetColor() { this->color = color; }
};