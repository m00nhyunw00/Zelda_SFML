#include "Slime.h"
#include "ResourceManager.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "SpriteUtil.h"
#include <iostream>

using namespace std;

Slime::Slime(
    const MonsterColor& color,
    const MonsterData& data,
    const sf::Vector2f& startPosition
) : Monster(MonsterType::SLIME, data, startPosition)
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Slime_Idle");

    animation.SetOwnerType("SLIME");

    if (idleTexture == nullptr)
    {
        cerr << "Slime_Idle texture not found." << endl;
        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            1,
            6,
            0,
            0
        )
    );
    sprite->setScale({ 4.f,4.f });
    sprite->setPosition(startPosition);
}

void Slime::UpdateTypeLogic(float deltaTime)
{

}

void Slime::Attack(Creature* target)
{

}