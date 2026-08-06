#include "Monster.h"

Monster::Monster(
    MonsterType type,
    const MonsterData& data,
    const sf::Vector2f& position
) : Creature(CreatureType::MONSTER, data, position)
{
    this->type = type;
    this->color = data.color;
	this->exp = 0;
}

sf::Vector2f Monster::AIMovement(float deltaTime)
{
    return { 0,0 };
}

void Monster::HandleAnimation(const sf::Vector2f& direction, float deltaTime)
{
    
}

void Monster::UpdateLogic(float deltaTime)
{

}