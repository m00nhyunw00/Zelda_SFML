#pragma once

#include "SFML/Graphics.hpp"
#include "AnimationState.h"

class Animation
{
private:
    std::string ownerType;  // WARRIOR, ARCHER, SLIME 등

    CreatureState currentState;
    Direction currentDirection;

    int currentFrame;      // 현재 재생 중인 프레임 인덱스
    float elapsedTime;     // 다음 프레임으로 넘어가기 위해 누적된 시간

public:
    void SetOwnerType(const std::string& ownerType) { this->ownerType = ownerType; }

    std::string GetAnimationKey();

    void Play(CreatureState state, Direction direction);
    bool Update(sf::Sprite& sprite, float deltaTime);
};

