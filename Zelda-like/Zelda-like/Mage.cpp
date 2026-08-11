#include "Mage.h"
#include "ResourceManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include <iostream>

using namespace std;

Mage::Mage(
    const std::string& name,
    const PlayerData& basicData,
    const PlayerLevelData& levelData,
    const PlayerSaveData& saveData,
    const sf::Vector2f& startPosition
)
    : Player(
        name,
        PlayerType::MAGE,
        basicData,
        levelData,
        saveData,
        startPosition
    )
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
    sprite->setScale({ Constants::DEFAULT_SCALE, Constants::DEFAULT_SCALE });
    sprite->setPosition(startPosition);

    bodyCollider.SetSize({ 32.f, 24.f });
    bodyCollider.SetOffset({ 0.f, 12.f });
}

void Mage::UpdateJobLogic(float deltaTime)
{

}

void Mage::Attack(Creature* target)
{
    return;
}

void Mage::UseSkill(Creature* target)
{
    return;
}

void Mage::UseUltimate(Creature* target)
{
    return;
}