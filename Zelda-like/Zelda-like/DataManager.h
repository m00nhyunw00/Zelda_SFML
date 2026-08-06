#pragma once

#include <map>
#include <string>

#include "PlayerData.h"
#include "PlayerType.h"
#include "MonsterData.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "AnimationState.h"
#include "AnimationData.h"

class DataManager
{
private:
    std::map<PlayerType, PlayerData> playerDataTable;
    std::map<MonsterType, MonsterData> monsterDataTable;
    std::map<std::string, AnimationData> animationDataTable;

private:
    DataManager();
    ~DataManager();

    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

public:
    static DataManager& GetInstance();

    PlayerType StringToPlayerType(const std::string& value) const;
    MonsterType StringToMonsterType(const std::string& value) const;

    bool LoadPlayerData(const std::string& filePath);
    const PlayerData* GetPlayerData(PlayerType type) const;

    bool LoadMonsterData(const std::string& filePath);
    const MonsterData* GetMonsterData(MonsterType type) const;

    bool LoadAnimationData(const std::string& filePath);
    const AnimationData* GetAnimationData(const std::string& key) const;
};

