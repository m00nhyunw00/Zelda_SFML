#include "Player.h"
#include "InputManager.h"
#include "CreatureType.h"
#include "AnimationState.h"
#include <iostream>
#include <utility>

using namespace std;

Player::Player(
	const std::string& name,
	PlayerType job,
	const PlayerData& data,
	const sf::Vector2f& position
) : Creature(CreatureType::PLAYER, data, position)
{
	this->userName = name;
	this->job = job;
	this->level = 1;
	this->skillName = data.skillName;
	this->skillDamage = data.skillDamage;
	this->maxSkillCooldown = data.maxSkillCooldown;
	this->skillCooldown = 0;
    this->ultimateName = data.ultimateName;
    this->ultimateDamage = data.ultimateDamage;
    this->maxUltimateGauge = data.maxUltimateGauge;
    this->ultimateGauge = 0;
	this->maxExp = 100;
	this->currentExp = 0;
}

void Player::UpdateLogic(float deltaTime)
{
    // 이번 프레임 Trigger 초기화
    attackTriggered = false;
    skillTriggered = false;
    ultimateTriggered = false;

    InputManager& input = InputManager::GetInstance();

    const sf::Vector2f direction = input.GetMoveDirection();

    // 공격 애니메이션 중이 아닐 때만
    // 새로운 공격 행동 시작 가능
    if (animationState != CreatureState::ATTACK)
    {
        // 평타
        if (input.IsSpacePressed())
        {
            animationState = CreatureState::ATTACK;

            attackTriggered = true;
        }

        // 스킬
        else if (input.IsEPressed() && skillCooldown <= 0.f)
        {
            animationState = CreatureState::ATTACK;

            skillTriggered = true;

            skillCooldown = maxSkillCooldown;
        }

        // 궁극기
        else if (input.IsQPressed()/* && 궁극기 게이지 FULL */)
        {
            animationState = CreatureState::ATTACK;

            ultimateTriggered = true;
        }
    }

    // 스킬 쿨타임
    if (skillCooldown > 0.f)
    {
        skillCooldown -= deltaTime;

        if (skillCooldown < 0.f)
        {
            skillCooldown = 0.f;
        }
    }

    HandleMovement(direction, deltaTime);

    HandleAnimation(direction, deltaTime);

    UpdateJobLogic(deltaTime);
}

void Player::HandleMovement(const sf::Vector2f& direction, float deltaTime)
{
    UpdateFacingDirection(direction);
    Move(direction, deltaTime);

}

void Player::HandleAnimation(const sf::Vector2f& direction, float deltaTime)
{
    if (HandleJobAnimation(deltaTime))
    {
        return;
    }

    // 공격 중이 아닐 때만 이동 여부로 상태 변경
    if (animationState != CreatureState::ATTACK)
    {
        if (direction.x != 0.f ||
            direction.y != 0.f)
        {
            animationState = CreatureState::RUN;
        }
        else
        {
            animationState = CreatureState::IDLE;
        }
    }

    animation.Play(animationState, facingDirection);

    const bool animationFinished = animation.Update(*sprite, deltaTime);

    // 공격 애니메이션 한 사이클이 종료됨
    if (animationState == CreatureState::ATTACK && animationFinished)
    {
        if (direction.x != 0.f || direction.y != 0.f)
        {
            animationState = CreatureState::RUN;
        }
        else
        {
            animationState = CreatureState::IDLE;
        }
    }
}

void Player::AddPendingProjectile(Projectile* projectile)
{
    if (projectile == nullptr)
    {
        return;
    }

    pendingProjectiles.push_back(projectile);
}

std::vector<Projectile*> Player::TakePendingProjectiles()
{
    std::vector<Projectile*> result = std::move(pendingProjectiles);

    pendingProjectiles.clear();

    return result;
}

void Player::IncreaseMaxHp(int amount)
{

}

void Player::IncreaseDamage(int amount)
{

}

void Player::IncreaseSkillDamage(int amount)
{

}

void Player::AddExp(int exp)
{

}

void Player::LevelUp()
{

}

void Player::IncreaseStats(int maxHpAmount, int defenceAmount, int damageAmount, int skillDamageAmount)
{

}

void Player::Heal(int amount)
{

}
