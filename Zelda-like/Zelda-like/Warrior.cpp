#include "Warrior.h"
#include "PlayerType.h"

Warrior::Warrior(
    const std::string& name,
    const PlayerData& data,
    const sf::Vector2f& startPosition
) : Player(name, PlayerType::MAGE, data, startPosition)
{

}

void Warrior::UpdateLogic(float deltaTime)
{

}

void Warrior::Attack(Creature* target)
{

}

void Warrior::UseSkill(Creature* target)
{

}