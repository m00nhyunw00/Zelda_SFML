#include "Archer.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include <iostream>

using namespace std;

Archer::Archer(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{
    sf::Texture* idleTexture = ResourceManager::GetInstance().GetTexture("Player_Idle");

    animation.SetOwnerType("ARCHER");

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

    bodyCollider.SetSize({ 32.f, 24.f });
    bodyCollider.SetOffset({ 0.f, 12.f });
}

void Archer::UpdateJobLogic(float deltaTime)
{

}

void Archer::Attack(Creature* target)
{

}

void Archer::UseSkill(Creature* target)
{

}