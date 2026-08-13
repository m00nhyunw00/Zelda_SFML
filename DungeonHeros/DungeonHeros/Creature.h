#pragma once

#include "SFML/Graphics.hpp"
#include "Entity.h"
#include "CreatureType.h"
#include "AnimationState.h"
#include "PlayerData.h"
#include "PlayerLevelData.h"
#include "MonsterData.h"
#include "Animation.h"
#include "Collider.h"
#include "ProjectileType.h"

class Projectile;

class Creature : public Entity
{
protected:
    sf::Vector2f position;          // 현재 위치
    sf::Vector2f previousPosition;  // 이전 프레임에서의 위치
    float moveSpeed;                // 초당 이동 속도

    Direction facingDirection;      // 바라보는 방향 상태값
    CreatureState animationState;   // 애니메이션 상태값

    Animation animation;            // 현재 애니메이션
    Collider bodyCollider;          // 몸체 충돌 콜라이더
    Collider attackCollider;        // 공격 범위 콜라이더

    bool attackTriggered;           // 공격 플래그

    bool canMove = true;            // 이동 가능 여부
    bool canAttack = true;          // 공격 가능 여부
    bool invincible = false;        // 무적 여부

    float slowTimer = 0.f;
    float slowRate = 1.f;

private:
    CreatureType category;          // 플레이어인지 몬스터인지 구분
    int maxHp;                      // 최대 체력
    int hp;                         // 현재 체력
    int defence;                    // 방어력
    int damage;                     // 공격력
    int evasionRate;                // 회피율 0~100

protected:
    Creature(
        CreatureType category,
        const PlayerLevelData& data,
        const sf::Vector2f& position
    );

    Creature(
        CreatureType category,
        const MonsterData& data,
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
    float GetMoveSpeed() { return moveSpeed; }
    sf::Vector2f GetPosition() { return position; }
    sf::Vector2f GetPreviousPosition() { return previousPosition; }
    Collider& GetBodyCollider() { return bodyCollider; }
    Collider& GetAttackCollider() { return attackCollider; }
    Animation& GetAnimation() { return animation; }
    Direction GetFacingDirection() { return facingDirection; };
    sf::Color GetSpriteColor() const;
    bool IsInvincible() const { return invincible; }
    bool IsAttackTriggered() const { return attackTriggered; }
    bool CanMove() { return canMove; }

    // Setter --------------------------------------------------

    void SetMaxHp(int maxHp) { this->maxHp = maxHp; }
    void SetHp(int hp) { this->hp = hp; }
    void SetDefence(int defence) { this->defence = defence; }
    void SetDamage(int damage) { this->damage = damage; }
    void SetPosition(const sf::Vector2f& position) { this->position = position; }
    void SetMoveSpeed(const float& moveSpeed) { this->moveSpeed = moveSpeed; }
    void SetCanMove(bool canMove) { this->canMove = canMove; }
    void SetCanAttack(bool canAttack) { this->canAttack = canAttack; }
    void SetInvincible(bool invincible) { this->invincible = invincible; }

    // ---------------------------------------------------------

    // Entity의 순수 가상 함수 구현
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

    void UpdateFacingDirection(const sf::Vector2f& direction);
    void Move(const sf::Vector2f& direction, float deltaTime);

    // 공격 방식은 Player와 Monster마다 다르므로 순수 가상 함수
    virtual void Attack(Creature* target) = 0;

    int TakeDamage(int incomingDamage); // 피격 함수

    void MoveForce(const sf::Vector2f& position);

    void ApplySlow(float rate, float duration);
    void UpdateSlow(float deltaTime);
};

