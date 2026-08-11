#include "Skeleton.h"
#include "ResourceManager.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "SpriteUtil.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Skeleton::Skeleton(
    const MonsterColor& color,
    const MonsterData& data,
    const sf::Vector2f& startPosition
)
    : Monster(
        MonsterType::SKELETON,
        data,
        startPosition
    )
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Skeleton_Idle");

    animation.SetOwnerType("SKELETON");

    if (idleTexture == nullptr)
    {
        cerr << "Skeleton_Idle texture not found." << endl;

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

    sprite->setScale({
        Constants::DEFAULT_SCALE,
        Constants::DEFAULT_SCALE
        });

    sprite->setPosition(
        startPosition
    );

    attackCollider.SetSize({
        Constants::DEFAULT_COLLIDER_SIZE,
        data.attackRange
        });

    attackCollider.SetOrigin({
        Constants::DEFAULT_COLLIDER_SIZE / 2,
        data.attackRange
        });

    SetColor(color);    // 실제 몬스터 색상 타입 저장

    // 색상에 따른 능력치 + Sprite 색상
    switch (color)
    {
    case MonsterColor::BLUE:
        SetMaxHp(static_cast<int>(GetMaxHp() * 1.5f));
        SetHp(GetMaxHp());
        sprite->setColor(sf::Color(50, 90, 255));
        break;

    case MonsterColor::RED:
        SetDamage(static_cast<int>(GetDamage() * 1.4f));
        sprite->setColor(sf::Color(255, 45, 45));
        break;

    case MonsterColor::BLACK:
        SetDefence(static_cast<int>(GetDefence() * 1.5f));
        sprite->setColor(sf::Color(45, 45, 55));
        break;

    case MonsterColor::PURPLE:
        SetMoveSpeed(GetMoveSpeed() * 1.35f);
        sprite->setColor(sf::Color(210, 70, 255));
        break;

    case MonsterColor::NONE_COLOR:
        sprite->setColor(sf::Color(70, 255, 90));
        break;
    }
}

void Skeleton::UpdateTypeLogic(float deltaTime)
{

}

void Skeleton::Attack(Creature* target)
{
    if (target == nullptr || !target->IsActive())
    {
        return;
    }

    int randomDamageBoundary = static_cast<int>(GetDamage() * 0.2f);

    int randomDamage = 0;

    if (randomDamageBoundary > 0)
    {
        randomDamage = rand() % randomDamageBoundary - randomDamageBoundary / 2;
    }

    int finalDamage = GetDamage() + randomDamage - target->GetDefence();

    int realDamage = target->TakeDamage(finalDamage);

    cout << "Skeleton Attack : " << realDamage << endl;
}