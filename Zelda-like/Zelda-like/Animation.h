#pragma once

#include "SFML/Graphics.hpp"
#include "AnimationState.h"

class Animation
{
private:
    std::string ownerType;  // WARRIOR, ARCHER, SLIME 등

    std::string currentAnimationKey;    // 직접 재생용 애니메이션 Key

    CreatureState currentState;
    Direction currentDirection;

    int currentFrame;      // 현재 재생 중인 프레임 인덱스
    float elapsedTime;     // 다음 프레임으로 넘어가기 위해 누적된 시간

public:
    void SetOwnerType(const std::string& ownerType) { this->ownerType = ownerType; }

    std::string GetAnimationKey();

    // State와 Direction을 활용하여 애니메이션 재생 (주로 Creature에서 사용)
    void Play(CreatureState state, Direction direction);

    // animationKey를 활용하여 애니메이션 재생 (주로 Projectile에서 사용)
    void Play(const std::string& animationKey);

    bool Update(sf::Sprite& sprite, float deltaTime);
};

