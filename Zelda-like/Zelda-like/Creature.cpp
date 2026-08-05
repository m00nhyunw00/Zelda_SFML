#include "Creature.h"

Creature::Creature(
    CreatureType category, 
    const PlayerData& data,
    const sf::Vector2f& position
)
{
    this->position = position;
    this->moveSpeed = data.moveSpeed;

    this->category = category;
    this->maxHp = data.maxHp;
    this->hp = data.maxHp;
    this->defence = data.defence;
    this->damage = data.damage;
    this->evasionRate = data.evasionRate;
}

void Creature::Update(float deltaTime, sf::RenderWindow& window)
{

}

void Creature::Render(sf::RenderWindow& window)
{

}

void Creature::Move(const sf::Vector2f& direction, float deltaTime)
{

}

int Creature::TakeDamage(int incomingDamage)
{

}

bool Creature::IsDead()
{

}