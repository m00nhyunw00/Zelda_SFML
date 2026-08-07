#include "Creature.h"
#include "Constants.h"

using namespace std;

Creature::Creature(CreatureType category, const PlayerData& data, const sf::Vector2f& position)
    : bodyCollider({ Constants::DEFAULT_COLLIDER_SIZE, Constants::DEFAULT_COLLIDER_SIZE }, { 0.f, 0.f }),
      attackCollider({ Constants::DEFAULT_COLLIDER_SIZE, Constants::DEFAULT_ATTACK_RANGE }, { 0.f, 0.f })
{
    this->position = position;
    this->moveSpeed = data.moveSpeed;

    facingDirection = Direction::DOWN;
    animationState = CreatureState::IDLE;

    this->category = category;
    this->maxHp = data.maxHp;
    this->hp = data.maxHp;
    this->defence = data.defence;
    this->damage = data.damage;
    this->evasionRate = data.evasionRate;

    attackTriggered = false;

    bodyCollider.UpdatePosition(position);
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE / 2, Constants::DEFAULT_ATTACK_RANGE });
    attackCollider.UpdatePosition(position);
}

Creature::Creature(CreatureType category, const MonsterData& data, const sf::Vector2f& position)
    : bodyCollider({ Constants::DEFAULT_COLLIDER_SIZE, Constants::DEFAULT_COLLIDER_SIZE }, { 0.f, 0.f }), 
      attackCollider({ Constants::DEFAULT_COLLIDER_SIZE, Constants::DEFAULT_ATTACK_RANGE }, { 0.f, 0.f })
{
    this->position = position;
    this->moveSpeed = data.moveSpeed;

    facingDirection = Direction::DOWN;
    animationState = CreatureState::IDLE;

    this->category = category;
    this->maxHp = data.maxHp;
    this->hp = data.maxHp;
    this->defence = data.defence;
    this->damage = data.damage;
    this->evasionRate = data.evasionRate;

    bodyCollider.UpdatePosition(position);
    attackCollider.SetOrigin({ Constants::DEFAULT_COLLIDER_SIZE / 2, Constants::DEFAULT_ATTACK_RANGE });
    attackCollider.UpdatePosition(position);
}

void Creature::Update(float deltaTime, sf::RenderWindow& window)
{
    (void)window;

    if (!IsActive())
    {
        return;
    }

    UpdateLogic(deltaTime);
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

    bodyCollider.Draw(window);
    attackCollider.Draw(window);
}

void Creature::UpdateFacingDirection(const sf::Vector2f& direction)
{
    if (direction.x == 0.f && direction.y == 0.f)
    {
        return;
    }

    if (animationState == CreatureState::ATTACK)
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

    switch (facingDirection)
    {
    case Direction::UP:
        attackCollider.SetRotation(0.f);
        break;

    case Direction::RIGHT:
        attackCollider.SetRotation(90.f);
        break;

    case Direction::DOWN:
        attackCollider.SetRotation(180.f);
        break;

    case Direction::LEFT:
        attackCollider.SetRotation(270.f);
        break;
    }

    attackCollider.UpdatePosition(position);

}

void Creature::Move(const sf::Vector2f& direction, float deltaTime)
{
    previousPosition = position;

    if (animationState == CreatureState::ATTACK)
    {
        if (category == CreatureType::PLAYER)
            position += direction * (moveSpeed / 2) * deltaTime;
    }
    else
        position += direction * moveSpeed * deltaTime;

    if (sprite != nullptr)
    {
        sprite->setPosition(position);
    }

    bodyCollider.UpdatePosition(position);
    attackCollider.UpdatePosition(position);

}

void Creature::MoveForce(const sf::Vector2f& position)
{
    this->position = position;

    if (sprite != nullptr)
    {
        sprite->setPosition(position);
    }

    bodyCollider.UpdatePosition(position);
    attackCollider.UpdatePosition(position);
}

int Creature::TakeDamage(int incomingDamage)
{
    int beforeHp = hp;

    hp -= incomingDamage;

    if (hp < 0)	// 체력이 0 미만으로 떨어질 경우에 대한 처리
    {
        hp = 0;
        SetActive(false);
    }

    if (beforeHp < hp)	// 공격력보다 방어력이 높아 오히려 체력이 올라가는 현상을 방지
        hp = beforeHp;

    return beforeHp - hp;	// 몬스터가 죽은 경우를 감안하여 실제로 적용된 논리적 데미지를 반환
}

bool Creature::IsDead()
{
    return false;
}