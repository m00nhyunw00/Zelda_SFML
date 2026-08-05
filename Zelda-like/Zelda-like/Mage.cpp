#include "Mage.h"
#include "PlayerType.h"

Mage::Mage(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{

}

void Mage::UpdateLogic(float deltaTime)
{

}

void Mage::Attack(Creature* target)
{

}

void Mage::UseSkill(Creature* target)
{

}