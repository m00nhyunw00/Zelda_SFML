#include "Archer.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "ProjectileType.h"
#include "Arrow.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Archer::Archer(
    const std::string& name,
    const PlayerData& basicData,
    const PlayerLevelData& levelData,
    const PlayerSaveData& saveData,
    const sf::Vector2f& startPosition
)
    : Player(
        name,
        PlayerType::ARCHER,
        basicData,
        levelData,
        saveData,
        startPosition
    )
{
    isTripleShot = false;
    isTitanArrow = false;

    tripleShotCount = 0;

    tripleShotTimer = 0.f;

    tripleShotInterval = 0.12f;

    tripleShotTarget = nullptr;

    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Player_Idle");

    animation.SetOwnerType("ARCHER");

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

    attackCollider.SetSize({ Constants::DEFAULT_COLLIDER_SIZE, Constants::WARRIOR_ATTACK_RANGE / 1.5f });
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE / 2, Constants::WARRIOR_ATTACK_RANGE / 1.5f });
}

void Archer::UpdateJobLogic(float deltaTime)
{
    if (!isTripleShot)
    {
        return;
    }

    tripleShotTimer += deltaTime;

    if (tripleShotTimer < tripleShotInterval)
    {
        return;
    }

    tripleShotTimer = 0.f;

    AddPendingProjectile(CreateArrow(tripleShotTarget,GetSkillDamage(), Constants::DEFAULT_SCALE, ProjectileAttackType::SKILL));

    tripleShotCount++;

    if (tripleShotCount >= 3)
    {
        isTripleShot = false;

        tripleShotCount = 0;

        tripleShotTarget = nullptr;
    }

    animationState = CreatureState::IDLE;
}

bool Archer::HandleJobAnimation(float deltaTime)
{
    // ±Ã±Ø±â
    if (isTitanArrow)
    {
        animation.Play(
            "ARCHER_TITAN_ARROW"
        );

        const bool animationFinished =
            animation.Update(
                *sprite,
                deltaTime
            );

        if (animationFinished)
        {
            isTitanArrow = false;
            animationState = CreatureState::IDLE;
        }

        return true;
    }

    // ½ºÅ³
    if (isTripleShot)
    {
        animation.Play(
            "ARCHER_TRIPLE_SHOT"
        );

        animation.Update(
            *sprite,
            deltaTime
        );

        return true;
    }

    return false;
}


Projectile* Archer::CreateArrow(Creature* target, int damage, float scale, ProjectileAttackType type)
{
    sf::Vector2f fireDirection;

    if (target != nullptr)
    {
        fireDirection = target-> GetPosition() - GetPosition();

        const float length =
            std::sqrt(
                fireDirection.x * fireDirection.x +
                fireDirection.y * fireDirection.y
            );

        if (length > 0.f)
        {
            fireDirection.x /= length;
            fireDirection.y /= length;
        }
    }
    else
    {
        switch (facingDirection)
        {
        case Direction::UP:
            fireDirection = { 0.f, -1.f };
            break;

        case Direction::DOWN:
            fireDirection = { 0.f, 1.f };
            break;

        case Direction::LEFT:
            fireDirection = { -1.f, 0.f };
            break;

        case Direction::RIGHT:
            fireDirection = { 1.f, 0.f };
            break;
        }
    }

    return new Arrow(
        this,
        GetPosition(),
        fireDirection,
        damage,
        scale,
        type
    );
}

void Archer::Attack(
    Creature* target)
{
    AddPendingProjectile(
        CreateArrow(
            target,
            GetDamage(),
            Constants::DEFAULT_SCALE,
            ProjectileAttackType::NORMAL
        )
    );
}

void Archer::UseSkill(Creature* target)
{
    if (isTripleShot)
    {
        return;
    }

    isTripleShot = true;

    tripleShotCount = 1;
    tripleShotTimer = 0.f;

    tripleShotTarget = target;

    AddPendingProjectile(
        CreateArrow(
            tripleShotTarget,
            GetSkillDamage(),
            Constants::DEFAULT_SCALE, 
            ProjectileAttackType::SKILL
        )
    );
}

void Archer::UseUltimate(Creature* target)
{
    if (isTitanArrow)
    {
        return;
    }

    isTitanArrow = true;

    Projectile* titanArrow =
        CreateArrow(
            target,
            GetUltimateDamage(),
            Constants::DEFAULT_SCALE * 0.3f,
            ProjectileAttackType::ULTIMATE
        );


    titanArrow->SetPiercing(true);
    titanArrow->SetUltimateProjectile(true);

    AddPendingProjectile(titanArrow);

    ResetUltimateGauge();
}