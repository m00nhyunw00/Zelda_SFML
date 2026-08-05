#pragma once

#include <map>
#include <string>

#include "PlayerData.h"
#include "PlayerType.h"

class DataManager
{
private:
    std::map<PlayerType, PlayerData> playerDataTable;

private:
    DataManager();
    ~DataManager();

    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

public:
    static DataManager& GetInstance();

    bool LoadPlayerData(const std::string& filePath);

    const PlayerData* GetPlayerData(PlayerType type) const;
};

