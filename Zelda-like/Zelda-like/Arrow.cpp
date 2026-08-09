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
    float scale
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

    animation.Play("ARROW");

    collider.SetSize({
    8.f * scale,
    2.f * scale
        });
}