#include "BladeWave.h"
#include "ResourceManager.h"
#include "Constants.h"
#include <cmath>
#include <iostream>

using namespace std;

BladeWave::BladeWave(
    Creature* owner,
    const sf::Vector2f& position,
    const sf::Vector2f& direction,
    int damage,
    float scale
)
    : Projectile(
        ProjectileType::BLADE_WAVE,
        owner,
        position,
        direction,
        220.f,      // 초기 속도
        10.f,       // 최대 10초 유지
        damage
    )
{
    deceleration = 70.f;

    sf::Texture* texture = ResourceManager::GetInstance().GetTexture("Fireball");

    if (texture == nullptr)
    {
        return;
    }

    sprite = new sf::Sprite(*texture);

    sprite->setPosition(position);

    sprite->setScale({
        scale,
        scale
        });

    animation.Play("BLADE_WAVE");

    collider.SetSize({
        8.f * scale,
        8.f * scale
        });

    collider.SetOrigin({
        4.f * scale,
        4.f * scale
        });

    colliderOffset = {
        0.f,
        4.f * scale
    };

    collider.UpdatePosition(position + colliderOffset);

    // 궁극기로 입힌 피해는 궁극기 게이지 충전 안 함
    SetUltimateProjectile(true);
}

void BladeWave::Update(float deltaTime, sf::RenderWindow& window)
{
    moveSpeed -= deceleration * deltaTime;

    if (moveSpeed < 0.f)
    {
        moveSpeed = 0.f;
    }

    Projectile::Update(deltaTime, window);
}

void BladeWave::OnWallCollision()
{
    // 벽에 닿으면 파괴하지 않고 그 자리에서 정지
    direction = { 0.f, 0.f };
}