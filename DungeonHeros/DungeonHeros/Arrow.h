#pragma once

#include "Projectile.h"

class Arrow : public Projectile
{
private:
    bool stuckInWall;

public:
    Arrow(
        Creature* owner,
        const sf::Vector2f& position,
        const sf::Vector2f& direction,
        int damage,
        float scale,
        ProjectileAttackType type
    );

    void OnWallCollision() override;
};
