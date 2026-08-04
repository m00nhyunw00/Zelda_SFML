#include "Creature.h"

Creature::Creature(CreatureType category, int maxHp, int defence, int damage, int evasionRate)
{
    this->category = category;
    this->maxHp = maxHp;
    this->hp = maxHp;
    this->defence = defence;
    this->damage = damage;
    this->evasionRate = evasionRate;
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