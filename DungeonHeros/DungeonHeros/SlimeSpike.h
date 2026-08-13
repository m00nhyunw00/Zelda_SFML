#pragma once

#include "Projectile.h"
#include "ResourceManager.h"

class SlimeSpike : public Projectile
{
private:
    bool damageActive = false;          // 촉수 애니메이션이 시작된 직후와 끝나기 직전은 데미지를 주지 못함
    bool hasDamagedPlayer = false;      // 플레이어에게 데미지를 입힌 적이 있는지 체크

public:
    SlimeSpike(
        Creature* owner,
        const sf::Vector2f& position,
        int damage
    );

    void Update(float deltaTime, sf::RenderWindow& windo) override;

    bool CanDamage() const { return damageActive && !hasDamagedPlayer; }

    void MarkDamaged() { hasDamagedPlayer = true; }

    bool IsDamageActive() const { return damageActive; }
};
