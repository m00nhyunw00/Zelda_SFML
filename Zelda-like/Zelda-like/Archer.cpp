#include "Archer.h"

#include "PlayerType.h"

Archer::Archer(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{

}

void Archer::UpdateLogic(float deltaTime)
{

}

void Archer::Attack(Creature* target)
{

}

void Archer::UseSkill(Creature* target)
{

}