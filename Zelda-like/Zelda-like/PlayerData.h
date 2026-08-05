#pragma once

#include <string>

struct PlayerData
{
    int maxHp;
    int defence;
    int damage;
    int evasionRate;

    float moveSpeed;

    std::string skillName;
    int skillDamage;
    float maxSkillCooldown;
};