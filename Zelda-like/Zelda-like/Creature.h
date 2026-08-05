#pragma once

#include "SFML/Graphics.hpp"
#include "Entity.h"
#include "CreatureType.h"
#include "AnimationState.h"
#include "PlayerData.h"
#include "Animation.h"
#include "Collider.h"

class Creature : public Entity
{
protected:
    sf::Vector2f position;       // 현재 위치
    float moveSpeed;             // 초당 이동 속도

    Direction facingDirection;      // 바라보는 방향 상태값
    AnimationState animationState;  // 애니메이션 상태값

    Animation animation;         // 현재 애니메이션
    Collider collider;           // 몸체 충돌 영역

private:
    CreatureType category;       // 플레이어인지 몬스터인지 구분
    int maxHp;                   // 최대 체력
    int hp;                      // 현재 체력
    int defence;                 // 방어력
    int damage;                  // 공격력
    int evasionRate;             // 회피율 0~100

protected:
    Creature(
        CreatureType category,
        const PlayerData& data,
        const sf::Vector2f& position
    );

    // Player와 Monster의 서로 다른 로직 구현을 위한 순수 가상 함수
    virtual void UpdateLogic(float deltaTime) = 0;

public:
    virtual ~Creature() = default;

    // Getter -------------------------------------------------

    CreatureType GetCategory() { return category; }
    int GetMaxHp() { return maxHp; }
    int GetHp() { return hp; }
    int GetDefence() { return defence; }
    int GetDamage() { return damage; }
    int GetEvasionRate() { return evasionRate; }
    float GetMoveSpeed() const { return moveSpeed; }
    sf::Vector2f GetPosition() { return position; }
    Collider& GetCollider() { return collider; }
    Animation& GetAnimation() { return animation; }

    // ---------------------------------------------------------

    // Setter --------------------------------------------------

    void SetHp(int hp) { this->hp = hp; }
    void SetPosition(const sf::Vector2f& position) { this->position = position; }

    // ---------------------------------------------------------

    // Entity의 순수 가상 함수 구현
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

    void UpdateFacingDirection(const sf::Vector2f& direction);
    void Move(const sf::Vector2f& direction, float deltaTime);

    // 공격 방식은 Player와 Monster마다 다르므로 순수 가상 함수
    virtual void Attack(Creature* target) = 0;

    int TakeDamage(int incomingDamage); // 피격 함수
    bool IsDead();                // 사망 판정 함수
};

