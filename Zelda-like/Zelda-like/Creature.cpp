#include "Creature.h"

using namespace std;

Creature::Creature(CreatureType category, const PlayerData& data, const sf::Vector2f& position)
    : collider({ 32.f, 24.f }, { 0.f, 12.f })
{
    this->position = position;
    this->moveSpeed = data.moveSpeed;

    facingDirection = Direction::DOWN;
    animationState = AnimationState::IDLE;

    this->category = category;
    this->maxHp = data.maxHp;
    this->hp = data.maxHp;
    this->defence = data.defence;
    this->damage = data.damage;
    this->evasionRate = data.evasionRate;

    collider.UpdatePosition(position);
}

Creature::Creature(CreatureType category, const MonsterData& data, const sf::Vector2f& position)
    : collider({ 32.f, 24.f }, { 0.f, 12.f })
{
    this->position = position;
    this->moveSpeed = data.moveSpeed;

    facingDirection = Direction::DOWN;
    animationState = AnimationState::IDLE;

    this->category = category;
    this->maxHp = data.maxHp;
    this->hp = data.maxHp;
    this->defence = data.defence;
    this->damage = data.damage;
    this->evasionRate = data.evasionRate;

    collider.UpdatePosition(position);
}

void Creature::Update(float deltaTime, sf::RenderWindow& window)
{
    (void)window;

    if (!IsActive())
    {
        return;
    }

    UpdateLogic(deltaTime);

    if (sprite != nullptr)
    {
        sprite->setPosition(position);
    }

    //collider.SetPosition(position);
}

void Creature::Render(sf::RenderWindow& window)
{
    if (!IsActive())
    {
        return;
    }

    if (sprite != nullptr)
    {
        window.draw(*sprite);
    }
}

void Creature::UpdateFacingDirection(const sf::Vector2f& direction)
{
    if (direction.x == 0.f && direction.y == 0.f)
    {
        return;
    }

    if (animationState == AnimationState::ATTACK)
    {
        return;

    }

    if (abs(direction.x) >= abs(direction.y))
    {
        facingDirection = direction.x > 0.f
            ? Direction::RIGHT : Direction::LEFT;
    }
    else
    {
        facingDirection = direction.y > 0.f
            ? Direction::DOWN : Direction::UP;
    }
}

void Creature::Move(const sf::Vector2f& direction, float deltaTime)
{
    if (animationState == AnimationState::ATTACK)
        position += direction * (moveSpeed / 2) * deltaTime;
    else
        position += direction * moveSpeed * deltaTime;

    if (sprite != nullptr)
    {
        sprite->setPosition(position);
    }

    collider.UpdatePosition(position);
}

int Creature::TakeDamage(int incomingDamage)
{
    return 0;
}

bool Creature::IsDead()
{
    return false;
}