#pragma once

#include "Creature.h"
#include "Player.h"
#include "Monster.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "MonsterData.h"
#include "AnimationState.h"
#include "GaugeBar.h"
#include "Projectile.h"
#include <string>
#include <vector>

class Player;

class Monster : public Creature
{
private:
    MonsterType type;
    MonsterColor color;

    int exp;

    MonsterState monsterState;

    Player* target;

    float detectionRange;           // 플레이어 감지 범위
    float attackRange;              // 공격 시작 범위

    float maxAttackCooldown;
    float attackCooldown;

protected:
    GaugeBar* hpBar;
    bool showHpBar = true;

    std::vector<Projectile*> pendingProjectiles;

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

public:
    ~Monster() override;

    void Render(sf::RenderWindow& window);

    void AddPendingProjectile(Projectile* projectile);

    std::vector<Projectile*> TakePendingProjectiles();

    // Getter -------------------------------------------------

    MonsterType GetType() const { return type; }
    MonsterColor GetColor() const { return color; }
    int GetExp() const { return exp; }
    Player* GetTarget() { return target; }
    float GetAttakRange() { return attackRange; }

    // Setter -------------------------------------------------

    void SetType(MonsterType type) { this->type = type; }
    void SetColor(MonsterColor color) { this->color = color; }
    void SetTarget(Player* target) { this->target = target; }
    void SetShowHpBar(bool show) { showHpBar = show; }
    virtual void SetAttakRange(float range);
};