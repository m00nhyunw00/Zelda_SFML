#pragma once

#include <string>
#include "MonsterColor.h"

struct MonsterData
{
    int maxHp;
    int defence;
    int damage;
    int evasionRate;

    float moveSpeed;

    float detectionRange;         
    float attackRange;              

    MonsterColor color;
};