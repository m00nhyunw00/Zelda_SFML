#pragma once

#include "Projectile.h"

class BladeWave : public Projectile
{
private:
    float deceleration;

public:
    BladeWave(
        Creature* owner,
        const sf::Vector2f& position,
        const sf::Vector2f& direction,
        int damage,
        float scale
    );

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void OnWallCollision() override;
};