#pragma once

#include "Creature.h"
#include "PlayerType.h"
#include "PlayerData.h"
#include <string>

class Player : public Creature
{
private:
    std::string userName;
    PlayerType job;

    int level; 

    std::string skillName;
    int skillDamage;

    float maxSkillCooldown;
    float skillCooldown;

    int maxExp;
    int currentExp;

private:
    void HandleMovement(const sf::Vector2f& direction, float deltaTime);
    void HandleAnimation(const sf::Vector2f& direction, float deltaTime);


protected:
    Player(
        const std::string& name,
        PlayerType job,
        const PlayerData& data,
        const sf::Vector2f& position
    );

    void UpdateLogic(float deltaTime) override final;
    virtual void UpdateJobLogic(float deltaTime) = 0;

    virtual void Attack(Creature* target) override = 0;
    virtual void UseSkill(Creature* target) = 0;

public:
    ~Player() override = default;

    // Getter -------------------------------------------------

    const std::string& GetUserName() const { return userName; }
    PlayerType GetJob() const { return job; }
    int GetLevel() const { return level; }
    const std::string& GetSkillName() const { return skillName; }
    int GetSkillDamage() const { return skillDamage; }
    float GetMaxSkillCooldown() const { return maxSkillCooldown; }
    float GetSkillCooldown() const { return skillCooldown; }
    int GetMaxExp() const { return maxExp; }
    int GetCurrentExp() const { return currentExp; }

    // ---------------------------------------------------------

    // Setter -------------------------------------------------

    void SetJob(PlayerType job) { this->job = job; }
    void SetSkillCooldown(float cooldown) { this->skillCooldown = cooldown; }
    void SetCurrentExp(int exp) { this->currentExp = currentExp; }

    // ---------------------------------------------------------

    void IncreaseMaxHp(int amount);
    void IncreaseDamage(int amount);
    void IncreaseSkillDamage(int amount);

    void AddExp(int exp);
    void LevelUp();
    void IncreaseStats(int maxHpAmount, int damageAmount, int skillDamageAmount);

    void Heal(int amount);
};