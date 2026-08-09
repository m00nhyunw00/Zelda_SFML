#pragma once

#include "Projectile.h"

class Arrow : public Projectile
{
public:
    Arrow(
        Creature* owner,
        const sf::Vector2f& position,
        const sf::Vector2f& direction,
        int damage,
        float scale
    );
};
