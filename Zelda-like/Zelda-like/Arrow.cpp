#include "Arrow.h"
#include "ResourceManager.h"
#include "Constants.h"
#include <iostream>

using namespace std;

Arrow::Arrow(
    Creature* owner,
    const sf::Vector2f& position,
    const sf::Vector2f& direction,
    int damage,
    float scale,
    ProjectileAttackType type
)
    : Projectile(
        ProjectileType::ARROW,
        owner,
        position,
        direction,
        500.f,
        5.f,
        damage)
{
    stuckInWall = false;

    sf::Texture* texture = ResourceManager::GetInstance().GetTexture("Arrow");

    if (texture == nullptr)
    {
        std::cout << "Arrow Texture NOT FOUND!" << std::endl;
        return;
    }

    std::cout << "Arrow Texture Loaded!" << std::endl;

    sprite = new sf::Sprite(*texture);

    sprite->setPosition(position);

    sprite->setScale({
        scale,
        scale
        });

    const float angle =
        std::atan2(
            direction.y,
            direction.x
        ) * 180.f / 3.141592f;

    sprite->setRotation(
        sf::degrees(angle)
    );

    switch (type)
    {
    case ProjectileAttackType::ULTIMATE:
        ultimateProjectile = true;
        animation.Play("TITAN_ARROW");
        SetColor(sf::Color::Red);
        break;
    case ProjectileAttackType::SKILL:
        skillProjectile = true;
        animation.Play("ARROW");
        break;
    case ProjectileAttackType::NORMAL:
        animation.Play("ARROW");
        break;
    }

    collider.SetSize({
        8.f * scale,
        2.f * scale
        });

    collider.SetOrigin({
        4.f * scale,
        1.f * scale
        });

    collider.SetRotation(angle);
    collider.UpdatePosition(position);
}

void Arrow::OnWallCollision()
{
    if (stuckInWall)
    {
        return;
    }

    stuckInWall = true;

    // 벽에 들어가기 전 위치로 복구
    position = previousPosition;

    if (sprite != nullptr)
    {
        sprite->setPosition(position);
    }

    collider.UpdatePosition(
        position + colliderOffset
    );

    // 이동 정지
    moveSpeed = 0.f;

    // 애니메이션 정지
    SetAnimationPaused(true);

    // 콜라이더 비활성화
    SetCollisionEnabled(false);
}