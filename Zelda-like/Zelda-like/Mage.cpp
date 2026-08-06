#include "Mage.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include <iostream>

using namespace std;

Mage::Mage(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Player_Idle");

    animation.SetOwnerType("MAGE");

    if (idleTexture == nullptr)
    {
        cerr << "Player_Idle texture not found." << endl;
        return;
    }

    sprite = new sf::Sprite(*idleTexture);

    sprite->setTextureRect(
        SpriteUtil::GetFrameRect(
            *idleTexture,
            3,
            4,
            0,
            0
        )
    );
    sprite->setScale({ 4.f,4.f });
    sprite->setPosition(startPosition);
}

void Mage::UpdateJobLogic(float deltaTime)
{

}

void Mage::Attack(Creature* target)
{

}

void Mage::UseSkill(Creature* target)
{

}