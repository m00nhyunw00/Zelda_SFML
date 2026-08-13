#include "LichFireball.h"
#include "ResourceManager.h"
#include "Constants.h"

#include <iostream>

using namespace std;

LichFireball::LichFireball(
    Creature* owner,
    const sf::Vector2f& position,
    const sf::Vector2f& direction,
    int damage,
    float scale
)
    : Projectile(
        ProjectileType::LICH_FIREBALL,
        owner,
        position,
        direction,
        130.f,      // 이동 속도
        4.f,        // 최대 수명
        damage
    )
{
    sf::Texture* texture = ResourceManager::GetInstance().GetTexture("LichFireball");

    if (texture == nullptr)
    {
        cout << "[LichFireball] Texture not found" << endl;

        return;
    }

    sprite =
        new sf::Sprite(*texture);

    sprite->setPosition(position);

    sprite->setScale({
        scale,
        scale
        });

    animation.Play("LICH_FIREBALL");

    collider.SetSize({
        8.f * scale,
        8.f * scale
        });

    collider.SetOrigin({
        4.f * scale,
        4.f * scale
        });

    colliderOffset =
    {
        0.f,
        4.f * scale
    };

    collider.UpdatePosition(
        position +
        colliderOffset
    );
}

void LichFireball::OnWallCollision()
{
    SetActive(false);
}