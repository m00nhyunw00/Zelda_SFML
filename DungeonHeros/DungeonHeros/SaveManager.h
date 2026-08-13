#pragma once

#include "PlayerSaveData.h"
#include <string>

class Player;

class SaveManager
{
private:
    SaveManager();
    ~SaveManager();

    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    std::string saveFilePath;

public:
    static SaveManager& GetInstance();

    bool SavePlayer(Player* player);

    bool LoadPlayerSaveData(PlayerSaveData& saveData);

    bool HasSaveData() const;

    void DeleteSaveData();

private:
    std::string PlayerTypeToString(PlayerType type) const;

    PlayerType StringToPlayerType(const std::string& value) const;
};
