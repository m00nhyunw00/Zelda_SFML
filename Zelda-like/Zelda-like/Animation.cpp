#include "Animation.h"
#include "ResourceManager.h"
#include "SpriteUtil.h"
#include "AnimationData.h"
#include "DataManager.h"
#include <iostream>
#include <cmath>

using namespace std;

Animation::Animation()
{
    currentState = CreatureState::IDLE;
    currentDirection = Direction::DOWN;

    currentAnimationKey = "";
    ownerType = "";

    currentFrame = 0;
    elapsedTime = 0.f;
}

std::string Animation::GetAnimationKey()
{
    switch (currentState)
    {
    case CreatureState::IDLE:
        if (ownerType == "WARRIOR" ||
            ownerType == "ARCHER" ||
            ownerType == "MAGE")
        {
            currentAnimationKey = "PLAYER_IDLE";
            return currentAnimationKey;
        }

        return ownerType + "_IDLE";

    case CreatureState::RUN:
        if (ownerType == "WARRIOR" ||
            ownerType == "ARCHER" ||
            ownerType == "MAGE")
        {
            return "PLAYER_RUN";
        }

        currentAnimationKey = ownerType + "_RUN";
        return currentAnimationKey;

    case CreatureState::ATTACK:
        return ownerType + "_ATTACK";

    default:
        return "NONE";
    }
}

void Animation::Play(CreatureState state, Direction direction)
{
    // 이미 같은 애니메이션을 재생 중이면 초기화하지 않음
    if (currentState == state &&
        currentDirection == direction)
    {
        return;
    }

    currentState = state;
    currentDirection = direction;

    currentAnimationKey = "";

    // 다른 애니메이션으로 바뀌었으므로 처음부터 재생
    currentFrame = 0;
    elapsedTime = 0.f;
}

void Animation::Play(const std::string& animationKey)
{
    if (currentAnimationKey == animationKey)
    {
        return;
    }

    currentAnimationKey = animationKey;

    currentFrame = 0;
    elapsedTime = 0.f;
}

bool Animation::Update(sf::Sprite& sprite, float deltaTime)
{
    std::string animationKey;

    if (!currentAnimationKey.empty())
    {
        animationKey = currentAnimationKey;
    }
    else
    {
        animationKey = GetAnimationKey();
    }

    if (animationKey.empty())
    {
        return false;
    }

    const AnimationData* data = DataManager::GetInstance().GetAnimationData(animationKey);

    if (data == nullptr)
    {
        cerr << "애니메이션 데이터가 없습니다: " << animationKey << endl;

        return false;
    }

    sf::Texture* texture = ResourceManager::GetInstance().GetTexture(data->textureKey);

    if (texture == nullptr)
    {
        cerr << "애니메이션 Texture가 없습니다: " << data->textureKey << endl;

        return false;
    }

    bool animationFinished = false;

    // 현재 프레임이 유지된 시간 누적
    elapsedTime += deltaTime;

    if (elapsedTime >= data->frameDuration)
    {
        elapsedTime -= data->frameDuration;
        currentFrame++;

        if (currentFrame >= data->frameCount)
        {
            currentFrame = 0;

            // IDLE의 경우, 무한 반복되지만 ATTACK의 경우는 한 사이클이 끝나면 종료한 후 IDLE로 돌아가야 함
            if (currentState == CreatureState::ATTACK)
            {
                animationFinished = true;
            }
        }
    }

    int directionRow = 0;


    if (data->totalRows > 1)
    {
        if (currentState != ATTACK)
        {
            switch (currentDirection)
            {
            case Direction::DOWN:
                directionRow = 0;
                break;

            case Direction::LEFT:
            case Direction::RIGHT:
                directionRow = 1;
                break;
            case Direction::UP:
                directionRow = 2;
                break;
            }
        }
        else
        {
            switch (currentDirection)
            {
            case Direction::DOWN:
                if (ownerType == "WARRIOR" || ownerType == "SLIME")
                {
                    directionRow = 0;
                }
                else if (ownerType == "ARCHER")
                {
                    directionRow = 1;
                }
                break;

            case Direction::LEFT:
            case Direction::RIGHT:
                if (ownerType == "WARRIOR" || ownerType == "SLIME")
                {
                    directionRow = 1;
                }
                else if (ownerType == "ARCHER")
                {
                    directionRow = 0;
                }
                break;

            case Direction::UP:
                directionRow = 2;
                break;
            }
        }
    }

    sprite.setTexture(*texture);

    sprite.setTextureRect(
        SpriteUtil::GetFrameRect(
            *texture,
            data->totalRows, data->totalColumns,
            directionRow, currentFrame
        )
    );

    const sf::FloatRect bounds =
        sprite.getLocalBounds();

    // 이 과정 없이 좌우 반전을 할 경우, 회전의 중심이 스프라이트의 중심이 아니기 때문에 의도한대로 회전이 안됨
    sprite.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    const sf::Vector2f scale =
        sprite.getScale();

    if (currentDirection == Direction::LEFT)
    {
        sprite.setScale({
            -std::abs(scale.x),
            scale.y
            });
    }
    else
    {
        sprite.setScale({
            std::abs(scale.x),
            scale.y
            });
    }

    return animationFinished;
}