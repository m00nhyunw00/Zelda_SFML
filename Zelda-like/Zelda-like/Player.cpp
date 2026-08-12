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
    const PlayerData& basicData,
    const PlayerLevelData& levelData,
    const PlayerSaveData& saveData,
    const sf::Vector2f& position
)
    : Creature(
        CreatureType::PLAYER,
        levelData,
        position
    )
{
    userName = name;
    this->job = job;

    level = saveData.level;

    moveSpeed = basicData.moveSpeed;

    skillName = basicData.skillName;

    maxSkillCooldown = basicData.maxSkillCooldown;

    skillCooldown = 0.f;

    ultimateName = basicData.ultimateName;

    skillDamage = levelData.skillDamage;

    ultimateDamage = levelData.ultimateDamage;

    maxUltimateGauge = levelData.maxUltimateGauge;

    maxExp = levelData.maxExp;

    ultimateGauge = 0.f;

    currentExp = saveData.currentExp;

    SetHp(saveData.currentHp);
}

void Player::UpdateLogic(float deltaTime)
{
    attackTriggered = false;
    skillTriggered = false;
    ultimateTriggered = false;

    InputManager& input = InputManager::GetInstance();

    const sf::Vector2f direction = input.GetMoveDirection();

    if (animationState != CreatureState::ATTACK)
    {
        if (input.IsSpacePressed())
        {
            animationState = CreatureState::ATTACK;
            attackTriggered = true;
        }
        else if (input.IsEPressed() && skillCooldown <= 0.f)
        {
            animationState = CreatureState::ATTACK;
            skillTriggered = true;

            skillCooldown = maxSkillCooldown;
        }
        else if (input.IsQPressed() && ultimateGauge >= maxUltimateGauge)
        {
            animationState = CreatureState::ATTACK;
            ultimateTriggered = true;
        }
    }

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
    // 한 방향으로 움직일 때만 바라보는 방향으로 facingDirection 변경
    if ((direction.x == 0.f && direction.y != 0.f) ||
        (direction.x != 0.f && direction.y == 0.f))
    {
        UpdateFacingDirection(direction);
    }
    // 대각선 이동 시, 기존 facingDirection 유지

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

void Player::AddUltimateGauge(int damage)
{
    if (damage <= 0)
    {
        return;
    }

    ultimateGauge += damage;

    if (ultimateGauge > maxUltimateGauge)
    {
        ultimateGauge = maxUltimateGauge;
    }
}

void Player::ResetUltimateGauge()
{
    ultimateGauge = 0;
}

void Player::ApplyLevelData()
{
    const PlayerLevelData* data = DataManager::GetInstance().GetPlayerLevelData(job, level);

    if (data == nullptr)
    {
        return;
    }

    SetMaxHp(data->maxHp);

    SetDefence(data->defence);

    SetDamage(data->damage);

    skillDamage =data->skillDamage;

    ultimateDamage =data->ultimateDamage;

    maxUltimateGauge = data->maxUltimateGauge;

    maxExp =data->maxExp;

    if (ultimateGauge > maxUltimateGauge)
    {
        ultimateGauge = maxUltimateGauge;
    }
}

void Player::IncreaseMaxHp(int amount)
{
    SetMaxHp(GetMaxHp() + amount);
}

void Player::IncreaseDamage(int amount)
{
    SetDamage(GetDamage() + amount);
}

void Player::IncreaseDefence(int amount)
{
    SetDefence(GetDefence() + amount);
}

void Player::IncreaseSkillDamage(int amount)
{
    skillDamage += amount;

}

void Player::IncreaseUltimateDamage(int amount)
{
    ultimateDamage += amount;
}

void Player::IncreaseMaxExp(int amount)
{
    maxExp += amount;
}

void Player::AddExp(int exp)
{
    if (exp <= 0 || level >= 30)
    {
        return;
    }

    currentExp += exp;

    while (level < 30 && currentExp >= maxExp)
    {
        currentExp -= maxExp;

        LevelUp();
    }

    if (level >= 30)
    {
        currentExp = 0;
    }
}

void Player::LevelUp()
{
    if (level >= 30)
    {
        return;
    }

    level++;

    ApplyLevelData();

    // 레벨업 시 체력 완전 회복
    SetHp(GetMaxHp());

    cout
        << "Level Up! Lv."
        << level
        << endl;

    PrintPlayerInfo();
}

void Player::ApplyDeathPenalty()
{
    const int lostExp = static_cast<int>(maxExp / 3.f);

    currentExp -= lostExp;

    if (currentExp < 0)
    {
        currentExp = 0;
    }

    // 체력 완전 회복
    SetHp(GetMaxHp());

    // 궁극기 게이지 초기화
    ultimateGauge = 0.f;

    // 스킬 쿨타임도 초기화
    skillCooldown = 0.f;

    SetActive(true);

    cout << "Death Penalty - EXP Lost: " << lostExp << endl;
}

void Player::IncreaseStats(int maxHpAmount, int defenceAmount, int damageAmount, int expAmount)
{
    IncreaseMaxHp(maxHpAmount);
    IncreaseDefence(defenceAmount);
    IncreaseDamage(damageAmount);
    IncreaseMaxExp(expAmount);
}

void Player::Heal(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    SetHp(GetHp() + amount);

    if (GetHp() > GetMaxHp())
    {
        SetHp(GetMaxHp());
    }
}

void Player::PrintPlayerInfo()
{
    using std::cout;
    using std::endl;

    cout << "================ Player Info ================" << endl;
    cout << "Name              : " << GetUserName() << endl;
    cout << "Job               : " << static_cast<int>(GetJob()) << endl;
    cout << "Level             : " << GetLevel() << endl;

    cout << "HP                : "
        << GetHp() << " / "
        << GetMaxHp() << endl;

    cout << "Damage            : " << GetDamage() << endl;
    cout << "Defence           : " << GetDefence() << endl;
    cout << "Move Speed        : " << GetMoveSpeed() << endl;

    cout << "Skill             : " << GetSkillName() << endl;
    cout << "Skill Damage      : " << GetSkillDamage() << endl;
    cout << "Skill Cooldown    : "
        << GetSkillCooldown()
        << " / "
        << GetMaxSkillCooldown()
        << endl;

    cout << "Ultimate Skill             : " << GetUltimateName() << endl;
    cout << "Ultimate Skill Damage      : " << GetUltimateDamage() << endl;
    cout << "Ultimate Skill Gauge       : "
        << GetUltimateGauge()
        << " / "
        << GetMaxUltimateGauge()
        << endl;

    cout << "EXP               : "
        << GetCurrentExp()
        << " / "
        << GetMaxExp()
        << endl;

    cout << "Position          : ("
        << GetPosition().x
        << ", "
        << GetPosition().y
        << ")" << endl;

    cout << "=============================================" << endl;
}

void Player::PrintPlayerHp()
{
    cout << "=============================================" << endl;


    cout << "HP : " << GetHp() << " / " << GetMaxHp() << endl;

    cout << "=============================================" << endl;
}