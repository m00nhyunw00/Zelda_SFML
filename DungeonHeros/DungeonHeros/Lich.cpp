#include "Lich.h"
#include "ResourceManager.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "SpriteUtil.h"
#include "Constants.h"
#include "LichFireball.h"
#include <iostream>

using namespace std;

Lich::Lich(
    const MonsterColor& color,
    const MonsterData& data,
    const sf::Vector2f& startPosition
)
    : Monster(
        MonsterType::LICH,
        data,
        startPosition
    )
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Lich_Idle");

    animation.SetOwnerType("LICH");

    if (idleTexture == nullptr)
    {
        cerr << "Lich_Idle texture not found." << endl;

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
        data.attackRange,
        data.attackRange
        });

    attackCollider.SetOrigin({
        data.attackRange / 2,
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
        sprite->setColor(sf::Color::White);
        break;
    }
}

void Lich::UpdateTypeLogic(float deltaTime)
{

}

void Lich::Attack(Creature* target)
{
    if (target == nullptr ||
        !target->IsActive())
    {
        return;
    }

    sf::Vector2f direction = target->GetPosition() - GetPosition();

    const float length =
        std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

    if (length <= 0.f)
    {
        return;
    }

    direction.x /= length;
    direction.y /= length;

    LichFireball* fireball = new LichFireball(
            this,
            GetPosition(),
            direction,
            GetDamage(),
            Constants::DEFAULT_SCALE
        );

    AddPendingProjectile(fireball);
}

void Lich::SetAttakRange(float range)
{
    Monster::SetAttakRange(range);
    attackCollider.SetSize({ range,range });
    attackCollider.SetOrigin({ range / 2,range });
}
