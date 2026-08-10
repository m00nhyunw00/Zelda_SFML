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

    std::string ultimateName;
    int ultimateDamage;

    float maxUltimateGauge;
    float ultimateGauge;

    int maxExp;
    int currentExp;

protected:
    std::vector<Projectile*> pendingProjectiles;

    bool skillTriggered = false;
    bool ultimateTriggered = false;

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

    virtual bool HandleJobAnimation(float deltaTime) { return false; }

    void UpdateLogic(float deltaTime) override final;
    virtual void UpdateJobLogic(float deltaTime) = 0;

    void AddPendingProjectile(Projectile* projectile);

public:
    ~Player() override = default;

    virtual void UseSkill(Creature* target) = 0;   
    virtual void UseUltimate(Creature* target) = 0;;


    std::vector<Projectile*> TakePendingProjectiles();

    void AddUltimateGauge(int damage);
    void ResetUltimateGauge();

    void IncreaseMaxHp(int amount);
    void IncreaseDamage(int amount);
    void IncreaseDefence(int amount);
    void IncreaseSkillDamage(int amount);
    void IncreaseUltimateDamage(int amount);
    void IncreaseMaxExp(int amount);

    void AddExp(int exp);
    void LevelUp();
    void IncreaseStats(int maxHpAmount, int defenceAmount, int damageAmount, int expAmount);

    void Heal(int amount);

    void PrintPlayerInfo();
    void PrintPlayerHp();

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
    const std::string& GetUltimateName() const { return ultimateName; }
    int GetUltimateDamage() const { return ultimateDamage; }
    float GetMaxUltimateGauge() const { return maxUltimateGauge; }
    float GetUltimateGauge() const { return ultimateGauge; }
    bool IsSkillTriggered() const { return skillTriggered; }
    bool IsUltimateTriggered() const { return ultimateTriggered; }

    // Setter -------------------------------------------------

    void SetJob(PlayerType job) { this->job = job; }
    void SetSkillCooldown(float cooldown) { this->skillCooldown = cooldown; }
    void SetCurrentExp(int exp) { this->currentExp = exp; }

};