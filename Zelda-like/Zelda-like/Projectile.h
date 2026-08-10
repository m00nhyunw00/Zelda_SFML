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
    sf::Vector2f previousPosition;
    sf::Vector2f direction;

    float moveSpeed;

    float maxLifeTime;
    float elapsedLifeTime;

    int damage;

    Animation animation;
    bool animationPaused;

    Collider collider;
    sf::Vector2f colliderOffset = { 0.f, 0.f };
    bool collisionEnabled = true;

    bool piercing;
    bool skillProjectile = false;
    bool ultimateProjectile = false;

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

    virtual void OnWallCollision() { SetActive(false); }

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    ProjectileType GetType() const { return type; }
    Creature* GetOwner() { return owner; }
    sf::Vector2f GetPreviousPosition() const { return previousPosition; };
    Collider& GetCollider() { return collider; }
    int GetDamage() const { return damage; }
    bool IsPiercing() const { return piercing; }
    bool IsSkillProjectile() const { return skillProjectile; }
    bool IsUltimateProjectile() const { return ultimateProjectile; }
    bool IsCollisionEnabled() const { return collisionEnabled; }

    void SetPiercing(bool piercing) { this->piercing = piercing; }
    void SetUltimateProjectile(bool value) { ultimateProjectile = value; }
    void SetColor(const sf::Color& color);
    void SetAnimationPaused(bool paused) { animationPaused = paused; }
    void SetCollisionEnabled(bool enabled) { collisionEnabled = enabled; }
};