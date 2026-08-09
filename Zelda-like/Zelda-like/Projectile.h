#pragma once

#include "Entity.h"
#include "Creature.h"
#include "ProjectileType.h"
#include "Collider.h"

class Projectile : public Entity
{
protected:
    ProjectileType type;
    Creature* owner;

    sf::Vector2f position;
    sf::Vector2f direction;

    float moveSpeed;

    float maxLifeTime;
    float elapsedLifeTime;

    int damage;

    Animation animation;
    Collider collider;

public:
    Projectile(
        ProjectileType type,
        Creature* owner,
        const sf::Vector2f& position,
        const sf::Vector2f& direction,
        float moveSpeed,
        float maxLifeTime,
        int damage
    );

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    ProjectileType GetType() const { return type; }
    Creature* GetOwner() { return owner; }
    Collider& GetCollider() { return collider; }
    int GetDamage() const { return damage; }
};