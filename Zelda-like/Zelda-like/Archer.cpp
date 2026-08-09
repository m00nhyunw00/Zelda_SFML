#include "Archer.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Arrow.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Archer::Archer(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::ARCHER, data, startPosition)
{
    isTripleShot = false;

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
    sprite->setScale({ 4.f,4.f });
    sprite->setPosition(startPosition);

    bodyCollider.SetSize({ 32.f, 24.f });
    bodyCollider.SetOffset({ 0.f, 12.f });
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

    AddPendingProjectile(CreateArrow(tripleShotTarget,GetSkillDamage(), Constants::DEFAULT_SCALE));

    tripleShotCount++;

    if (tripleShotCount >= 3)
    {
        isTripleShot = false;

        tripleShotCount = 0;

        tripleShotTarget = nullptr;
    }
}

bool Archer::HandleJobAnimation(float deltaTime)
{
    if (!isTripleShot)
    {
        return false;
    }

    animation.Play(
        "ARCHER_TRIPLE_SHOT"
    );

    animation.Update(
        *sprite,
        deltaTime
    );

    // 기본 Player 애니메이션 처리를 막음
    return true;
}


Projectile* Archer::CreateArrow(Creature* target, int damage, float scale)
{
    sf::Vector2f fireDirection;

    if (target != nullptr)
    {
        fireDirection =
            target->GetPosition() -
            GetPosition();

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
        scale
    );
}

void Archer::Attack(
    Creature* target)
{
    AddPendingProjectile(
        CreateArrow(
            target,
            GetDamage(),
            Constants::DEFAULT_SCALE
        )
    );
}

void Archer::UseSkill(
    Creature* target)
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
            Constants::DEFAULT_SCALE
        )
    );
}