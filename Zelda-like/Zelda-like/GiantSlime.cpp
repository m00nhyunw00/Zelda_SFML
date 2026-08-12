#include "GiantSlime.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "SpriteUtil.h"
#include <iostream>

using namespace std;

using namespace std;

GiantSlime::GiantSlime(
    const MonsterData& data,
    const sf::Vector2f& startPosition
)
    : Monster(
        MonsterType::GIANT_SLIME,
        data,
        startPosition
    )
{
    // Sprite / Animation --------------------------------------------

    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("GiantSlime_Idle");

    animation.SetOwnerType("GIANT_SLIME");

    if (idleTexture == nullptr)
    {
        cerr << "GiantSlime_Idle texture not found." << endl;

        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            1,
            6,
            0,
            0
        )
    );

    // Giant Slime이므로 일반 몬스터보다 크게 
    sprite->setScale({
        Constants::DEFAULT_SCALE * 2.f,
        Constants::DEFAULT_SCALE * 2.f
        });

    sprite->setPosition(startPosition);

    // Collider --------------------------------------------

    bodyCollider.SetSize({
        180.f,
        140.f
        });

    bodyCollider.SetOffset({
    0.f,
    5.f
        });

    bodyCollider.UpdatePosition(startPosition);

    attackCollider.SetSize({
        data.attackRange,
        data.attackRange
        });

    attackCollider.SetOrigin({
        data.attackRange / 2.f,
        data.attackRange
        });

    attackCollider.SetOffset({
    0.f,
    5.f
        });

    attackCollider.UpdatePosition(startPosition);

    // Base Stat --------------------------------------------

    baseDamage = GetDamage();
    baseDefence = GetDefence();

    // Phase --------------------------------------------

    currentPhase = BossPhase::PHASE_1;

    phase2Entered = false;
    phase3Entered = false;

    // Ranged Attack --------------------------------------------

    preparingRangedAttack = false;

    rangedAttackTimer = 0.f;

    // 8초마다 원거리 패턴 시작
    rangedAttackInterval = 8.f;

    // 공격 전 2초간 예고
    rangedAttackWindup = 2.f;

    rangedAttackWindupTimer = 0.f;

    rangedAttackTriggered = false;

    spikeAttackActive = false;

    currentSpikeStep = 0;

    maxSpikeStep = 6;

    spikeStepTimer = 0.f;

    spikeStepInterval = 0.12f;

    spikeSpacing = 70.f;
}


void GiantSlime::Attack(Creature* target)
{
    if (target == nullptr || !target->IsActive())
    {
        return;
    }

    target->TakeDamage(GetDamage());
}

void GiantSlime::UpdatePhase()
{
    if (GetMaxHp() <= 0)
    {
        return;
    }

    const float hpRatio = static_cast<float>(GetHp()) / static_cast<float>(GetMaxHp());

    // Phase 3 ----------------------------------------------
    // HP 1/3 이하

    if (hpRatio <= 1.f / 3.f &&
        currentPhase != BossPhase::PHASE_3)
    {
        currentPhase = BossPhase::PHASE_3;

        phase3Entered = true;

        // Phase 2의 공격력 유지
        SetDamage(static_cast<int>(baseDamage * 1.4f));

        // 방어력 증가
        SetDefence(static_cast<int>(baseDefence * 1.5f));

        // 몸을 붉게 변경
        if (sprite != nullptr)
        {
            sprite->setColor(
                sf::Color(
                    255,
                    70,
                    70
                )
            );
        }

        return;
    }


    // Phase 2 ----------------------------------------------
    // HP 2/3 이하

    if (hpRatio <= 2.f / 3.f && currentPhase == BossPhase::PHASE_1)
    {
        currentPhase = BossPhase::PHASE_2;

        phase2Entered = true;

        // 공격력 증가
        SetDamage(static_cast<int>(baseDamage * 1.4f));

        return;
    }
}

void GiantSlime::Phase1Logic(float deltaTime)
{
    // Phase 1은 특별한 패턴 없음.
    // 기존 Monster의 추적 + 근접공격 그대로 사용

    (void)deltaTime;
}

void GiantSlime::Phase2Logic(float deltaTime)
{
    UpdateRangedAttack(deltaTime);
}

void GiantSlime::Phase3Logic(float deltaTime)
{
    UpdateRangedAttack(deltaTime);
}

void GiantSlime::UpdateTypeLogic(float deltaTime)
{
    rangedAttackTriggered = false;

    UpdatePhase();

    switch (currentPhase)
    {
    case BossPhase::PHASE_1:
        Phase1Logic(deltaTime);
        break;

    case BossPhase::PHASE_2:
        Phase2Logic(deltaTime);
        break;

    case BossPhase::PHASE_3:
        Phase3Logic(deltaTime);
        break;
    }

    // 진행 중인 Spike 공격 갱신
    UpdateSpikeAttack(deltaTime);
}

void GiantSlime::UpdateRangedAttack(float deltaTime)
{
    // 현재 원거리 공격 준비 중
    if (preparingRangedAttack)
    {
        rangedAttackWindupTimer += deltaTime;

        if (rangedAttackWindupTimer >= rangedAttackWindup)
        {
            FireRangedAttack();

            preparingRangedAttack = false;

            rangedAttackWindupTimer = 0.f;
            rangedAttackTimer = 0.f;
        }

        return;
    }

    // 평소에는 다음 원거리 공격까지 시간 측정
    rangedAttackTimer += deltaTime;

    if (rangedAttackTimer >= rangedAttackInterval)
    {
        StartRangedAttack();
    }
}

void GiantSlime::UpdateSpikeAttack(float deltaTime)
{
    if (!spikeAttackActive)
    {
        return;
    }

    spikeStepTimer += deltaTime;

    if (spikeStepTimer < spikeStepInterval)
    {
        return;
    }

    spikeStepTimer = 0.f;

    // 현재 단계의 8방향 Spike 생성
    SpawnSpikeStep(currentSpikeStep);

    currentSpikeStep++;

    // 모든 단계 생성 완료
    if (currentSpikeStep > maxSpikeStep)
    {
        spikeAttackActive = false;

        currentSpikeStep = 0;

        // 원거리 공격이 완전히 끝났으므로 다시 이동
        SetCanMove(true);
        SetCanMove(true);
    }
}

void GiantSlime::StartRangedAttack()
{
    preparingRangedAttack = true;

    rangedAttackWindupTimer = 0.f;

    // 추격 중지
    SetCanMove(false);
    SetCanAttack(false);

    // 공격 예고
    animationState = CreatureState::IDLE;
}

void GiantSlime::FireRangedAttack()
{
    spikeAttackActive = true;

    currentSpikeStep = 1;

    spikeStepTimer = 0.f;
}

void GiantSlime::SpawnSpikeStep(int step)
{
    const float distance = spikeSpacing * static_cast<float>(step);

    const sf::Vector2f directions[8] =
    {
        {  0.f, -1.f },   // 위
        {  0.f,  1.f },   // 아래
        { -1.f,  0.f },   // 왼쪽
        {  1.f,  0.f },   // 오른쪽

        { -0.7071f, -0.7071f }, // 좌상
        {  0.7071f, -0.7071f }, // 우상
        { -0.7071f,  0.7071f }, // 좌하
        {  0.7071f,  0.7071f }  // 우하
    };

    for (const sf::Vector2f& direction : directions)
    {
        const sf::Vector2f spikePosition =
        {
            GetPosition().x +
            direction.x * distance,

            GetPosition().y +
            direction.y * distance
        };

        SlimeSpike* spike =
            new SlimeSpike(
                this,
                spikePosition,
                GetDamage()
            );

        AddPendingProjectile(spike);
    }
}

bool GiantSlime::ConsumePhase2Entered()
{
    if (!phase2Entered)
    {
        return false;
    }

    phase2Entered = false;

    return true;
}

bool GiantSlime::ConsumePhase3Entered()
{
    if (!phase3Entered)
    {
        return false;
    }

    phase3Entered = false;

    return true;
}