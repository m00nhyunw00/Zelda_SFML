#pragma once

#include "Projectile.h"

class LichFireball : public Projectile
{
public:
    LichFireball(
        Creature* owner,
        const sf::Vector2f& position,
        const sf::Vector2f& direction,
        int damage,
        float scale
    );

    void OnWallCollision() override;
};