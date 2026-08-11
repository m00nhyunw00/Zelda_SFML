#pragma once

#include <string>
#include "MonsterColor.h"

struct MonsterData
{
    int maxHp;
    int defence;
    int damage;

    float moveSpeed;

    float detectionRange;
    float attackRange;

    float attackCooldown;

    int exp;

    MonsterColor color;
};