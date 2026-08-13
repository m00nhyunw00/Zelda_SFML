#include "Warrior.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"
#include "Monster.h"
#include <iostream>

using namespace std;

Warrior::Warrior(
    const std::string& name,
    const PlayerData& basicData,
    const PlayerLevelData& levelData,
    const PlayerSaveData& saveData,
    const sf::Vector2f& startPosition
)
    : Player(
        name,
        PlayerType::WARRIOR,
        basicData,
        levelData,
        saveData,
        startPosition
    )
{
    isPowerStrike = false;
    isFlameBlade = false;

    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Player_Idle");

    animation.SetOwnerType("WARRIOR");

    if (idleTexture == nullptr)
    {
        cerr << "Player_Idle texture not found." << endl;
        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            3,
            4,
            0,
            0
        )
    );

    sprite->setScale({ Constants::DEFAULT_SCALE, Constants::DEFAULT_SCALE });
    sprite->setPosition(startPosition);

    attackCollider.SetSize({ Constants::DEFAULT_COLLIDER_SIZE * 2, Constants::WARRIOR_ATTACK_RANGE });
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE, Constants::WARRIOR_ATTACK_RANGE });
}

bool Warrior::HandleJobAnimation(float deltaTime)
{   
    // Blade Sweep ------------------------------

    if (isFlameBlade)
    {
        animation.Play("WARRIOR_BLADE_SWEEP");

        const bool animationFinished = animation.Update(*sprite, deltaTime);

        if (animationFinished)
        {
            isFlameBlade = false;

            animationState =
                CreatureState::IDLE;
        }

        return true;
    }

    // Power Strike ------------------------------

    if (isPowerStrike)
    {
        animation.Play(
            "WARRIOR_POWER_STRIKE"
        );

        const bool animationFinished =
            animation.Update(
                *sprite,
                deltaTime
            );

        if (animationFinished)
        {
            isPowerStrike = false;

            animationState =
                CreatureState::IDLE;

            powerStrikeHitTargets.clear();

            if (sprite != nullptr)
            {
                sprite->setColor(
                    sf::Color::White
                );
            }

            SetDefence(
                GetDefence() / 2
            );
        }

        return true;
    }

    return false;
}

void Warrior::UpdateJobLogic(float deltaTime)
{

}

void Warrior::Attack(Creature* target)
{
    if (target == nullptr)
    {
        return;
    }

    int random = rand() % 100;


    // 기본 공격력의 ±10% 랜덤 데미지
    int randomDamageBoundary = static_cast<int>(static_cast<float>(GetDamage()) * 0.2f);

    int randomDamage = 0;

    if (randomDamageBoundary > 0)
    {
        randomDamage = (rand() % randomDamageBoundary) - (randomDamageBoundary / 2);
    }

    // 방어력을 적용한 최종 데미지
    int finalDamage = GetDamage() + randomDamage - target->GetDefence();

    int realDamage = target->TakeDamage(finalDamage);
    AddUltimateGauge(realDamage);
    std::cout << realDamage  << " Guage Charged" << endl;
}

void Warrior::UseSkill(Creature* target)
{
    (void)target;

    if (isPowerStrike)
    {
        return;
    }

    // Power Strike 시작
    isPowerStrike = true;

    // 이번 스킬에서 맞은 몬스터 기록 초기화
    powerStrikeHitTargets.clear();

    if (sprite != nullptr)
    {
        sprite->setColor(sf::Color(170, 170, 170));
    }

    // 스킬 도중 방어력 3배
    SetDefence(GetDefence() * 3);
}

void Warrior::HitPowerStrike(Creature* target)
{
    if (!isPowerStrike || target == nullptr || !target->IsActive())
    {
        return;
    }

    // 이번 Power Strike에서 이미 판정받은 몬스터
    if (powerStrikeHitTargets.find(target) != powerStrikeHitTargets.end())
    {
        return;
    }

    int finalDamage = GetSkillDamage() - target->GetDefence();
    int realDamage = target->TakeDamage(finalDamage);
    AddUltimateGauge(realDamage * 2);
    std::cout << realDamage * 2 << " Guage Charged (X2 Buff)" << endl;

    cout << "Warrior Power Strike" << endl;

    // 명중했든 회피했든 한 번 판정받은 몬스터
    powerStrikeHitTargets.insert(target);
}

void Warrior::UseUltimate(Creature* target)
{
    (void)target;

    if (isFlameBlade)
    {
        return;
    }

    isFlameBlade = true;

    CreateFlameBlade();

    ResetUltimateGauge();
}

void Warrior::CreateFlameBlade()
{
    const int projectileCount = 21;

    // 전체 부채꼴 각도
    const float totalAngle = 120.f;

    // 현재 바라보는 방향을 각도로 변환
    float centerAngle = 0.f;

    switch (facingDirection)
    {
    case Direction::RIGHT:
        centerAngle = 0.f;
        break;

    case Direction::DOWN:
        centerAngle = 90.f;
        break;

    case Direction::LEFT:
        centerAngle = 180.f;
        break;

    case Direction::UP:
        centerAngle = -90.f;
        break;
    }

    // -60도 ~ +60도
    const float startAngle = centerAngle - totalAngle / 2.f;

    const float angleInterval = totalAngle / static_cast<float>(projectileCount - 1);

    for (int i = 0; i < projectileCount; i++)
    {
        const float angle = startAngle + angleInterval * i;

        const float radian = angle * 3.141592f / 180.f;

        sf::Vector2f direction =
        {
            std::cos(radian),
            std::sin(radian)
        };

        BladeWave* bladeWave =
            new BladeWave(
                this,
                GetPosition(),
                direction,
                GetUltimateDamage(),
                Constants::DEFAULT_SCALE
            );

        AddPendingProjectile(bladeWave);
    }
}