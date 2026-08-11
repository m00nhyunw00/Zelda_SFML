#pragma once

#include "Monster.h"
#include "BossPhase.h"
#include "SlimeSpike.h"

class GiantSlime : public Monster
{
private:
    BossPhase currentPhase;

    // 기본 능력치 저장
    int baseDamage;
    int baseDefence;

    bool preparingRangedAttack; // 원거리 공격 관련

    float rangedAttackTimer;
    float rangedAttackInterval;

    float rangedAttackWindup;
    float rangedAttackWindupTimer;

    bool rangedAttackTriggered;

    bool spikeAttackActive = false;

    int currentSpikeStep = 0;

    int maxSpikeStep = 5;

    float spikeStepTimer = 0.f;
    float spikeStepInterval = 0.12f;

    float spikeSpacing = 60.f;

    // Scene에서 페이즈 진입을 한 번만 감지하기 위함
    bool phase2Entered;
    bool phase3Entered;

private:
    void UpdatePhase();

    void Phase1Logic(float deltaTime);
    void Phase2Logic(float deltaTime);
    void Phase3Logic(float deltaTime);

    void StartRangedAttack();
    void UpdateRangedAttack(float deltaTime);
    void UpdateSpikeAttack(float deltaTime);
    void FireRangedAttack();
    void SpawnSpikeStep(int step);

public:
    GiantSlime(const MonsterData& data, const sf::Vector2f& startPosition);

    ~GiantSlime() override = default;

    void UpdateTypeLogic(float deltaTime) override;

    void Attack(Creature* target) override;

    BossPhase GetPhase() const { return currentPhase; }

    bool IsPreparingRangedAttack() const { return preparingRangedAttack; }

    bool IsRangedAttackTriggered() const { return rangedAttackTriggered; }

    bool ConsumePhase2Entered();
    bool ConsumePhase3Entered();
};