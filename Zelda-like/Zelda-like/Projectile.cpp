#include "Projectile.h"

Projectile::Projectile(
    ProjectileType type,
    Creature* owner,
    const sf::Vector2f& position,
    const sf::Vector2f& direction,
    float moveSpeed,
    float maxLifeTime,
    int damage)
{
    this->type = type;
    this->owner = owner;
    this->position = position;
    this->previousPosition = position;
    this->direction = direction;
    this->moveSpeed = moveSpeed;
    this->maxLifeTime = maxLifeTime;
    this->elapsedLifeTime = 0.f;
    this->damage = damage;
    this->animationPaused = false;
    this->piercing = false;
    this->skillProjectile = false;
    this->ultimateProjectile = false;

    collider.UpdatePosition(position);
}

void Projectile::Update(float deltaTime, sf::RenderWindow& window)
{
    (void)window;

    if (!IsActive())
    {
        return;
    }

    // 수명 증가
    elapsedLifeTime += deltaTime;

    if (elapsedLifeTime >= maxLifeTime)
    {
        SetActive(false);
        return;
    }

    // 이동
    previousPosition = position;
    position += direction * moveSpeed * deltaTime;

    if (sprite != nullptr)
    {
        sprite->setPosition(position);

        if (!animationPaused)
        {
            animation.Update(
                *sprite,
                deltaTime
            );
        }
    }

    // Collider 위치 갱신
    collider.UpdatePosition(position + colliderOffset);
}

void Projectile::Render(sf::RenderWindow& window)
{
    if (!IsActive())
    {
        return;
    }

    if (sprite != nullptr)
    {
        window.draw(*sprite);
    }

    // 디버깅용
    collider.Draw(window);
}

void Projectile::SetColor(const sf::Color& color)
{
    if (sprite != nullptr)
    {
        sprite->setColor(color);
    }
}