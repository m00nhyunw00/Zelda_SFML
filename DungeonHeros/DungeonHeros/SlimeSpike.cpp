#include "SlimeSpike.h"
#include "Constants.h"

SlimeSpike::SlimeSpike(
    Creature* owner,
    const sf::Vector2f& position,
    int damage
)
    : Projectile(
        ProjectileType::SLIME_SPIKE,
        owner,
        position,
        { 0.f, 0.f },
        0.f,
        0.5f,
        damage
    )
{
    sf::Texture* texture = ResourceManager::GetInstance().GetTexture("SlimeSpike");

    if (texture == nullptr)
    {
        return;
    }

    sprite = new sf::Sprite(*texture);

    sprite->setPosition(position);

    if (owner != nullptr)
    {
        sprite->setColor(
            owner->GetSpriteColor()
        );
    }

    animation.Play("SLIME_SPIKE");

    sprite->setScale({ 3.f, 3.f });

    collider.SetSize({
    40.f,
    40.f
        });

    collider.UpdatePosition(position);
}

void SlimeSpike::Update(float deltaTime,sf::RenderWindow& window)
{
    Projectile::Update(
        deltaTime,
        window
    );

    // 예시:
    // 0.08 ~ 0.32초 사이만 실제 공격 판정
    damageActive = elapsedLifeTime >= 0.08f && elapsedLifeTime <= 0.32f;
}