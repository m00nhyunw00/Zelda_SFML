// PlayerSaveData.h
#pragma once

#include "PlayerType.h"
#include <vector>
#include <string>

struct PlayerSaveData
{
    PlayerType job = PlayerType::NONE_PLAYER;

    int level = 1;
    int currentHp = 0;
    int currentExp = 0;

    std::vector<std::string> inventory;
};