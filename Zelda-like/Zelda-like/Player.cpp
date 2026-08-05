#include "Player.h"
#include "InputManager.h"
#include "CreatureType.h"
#include "AnimationState.h"

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
	this->maxExp = 100;
	this->currentExp = 0;
}

void Player::UpdateLogic(float deltaTime)
{
    const sf::Vector2f direction = InputManager::GetInstance().GetMoveDirection();

    if (InputManager::GetInstance().IsSpacePressed())
    {
        animationState = AnimationState::ATTACK;
        // Attack();
    }

    HandleMovement(direction, deltaTime);   // 모든 직업의 공통 이동 입력 처리
    HandleAnimation(direction, deltaTime);  // 모든 직업의 공통 애니메이션 처리

    // 모든 직업의 공통 쿨타임 처리
    if (skillCooldown > 0.f)
    {
        skillCooldown -= deltaTime;

        if (skillCooldown < 0.f)
        {
            skillCooldown = 0.f;
        }
    }

    // 직업마다 다른 업데이트 실행
    UpdateJobLogic(deltaTime);
}

void Player::HandleMovement(const sf::Vector2f& direction, float deltaTime)
{
    UpdateFacingDirection(direction);
    Move(direction, deltaTime);
}

void Player::HandleAnimation(const sf::Vector2f& direction, float deltaTime)
{
    // 공격 중이 아닐 때만 이동 여부로 상태 변경
    if (animationState != AnimationState::ATTACK)
    {
        if (direction.x != 0.f ||
            direction.y != 0.f)
        {
            animationState = AnimationState::RUN;
        }
        else
        {
            animationState = AnimationState::IDLE;
        }
    }

    animation.Play(animationState, facingDirection);

    const bool animationFinished = animation.Update(*sprite, deltaTime);

    // 공격 애니메이션 한 사이클이 종료됨
    if (animationState == AnimationState::ATTACK && animationFinished)
    {
        if (direction.x != 0.f || direction.y != 0.f)
        {
            animationState = AnimationState::RUN;
        }
        else
        {
            animationState = AnimationState::IDLE;
        }
    }
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

void Player::IncreaseStats(int maxHpAmount, int damageAmount, int skillDamageAmount)
{

}

void Player::Heal(int amount)
{

}
