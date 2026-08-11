#include "SaveManager.h"
#include "Player.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

SaveManager::SaveManager()
{
    saveFilePath = "Save/SaveData.json";
}

SaveManager::~SaveManager()
{

}

SaveManager& SaveManager::GetInstance()
{
    static SaveManager instance;

    return instance;
}

std::string SaveManager::PlayerTypeToString(PlayerType type) const
{
    switch (type)
    {
    case PlayerType::WARRIOR:
        return "WARRIOR";

    case PlayerType::ARCHER:
        return "ARCHER";

    case PlayerType::MAGE:
        return "MAGE";

    default:
        return "NONE";
    }
}

PlayerType SaveManager::StringToPlayerType(const std::string& value) const
{
    if (value == "WARRIOR")
    {
        return PlayerType::WARRIOR;
    }

    if (value == "ARCHER")
    {
        return PlayerType::ARCHER;
    }

    if (value == "MAGE")
    {
        return PlayerType::MAGE;
    }

    return PlayerType::NONE_PLAYER;
}

bool SaveManager::SavePlayer(Player* player)
{
    if (player == nullptr)
    {
        return false;
    }

    try
    {
        std::filesystem::create_directories("Save");

        json root;

        root["job"] = PlayerTypeToString(player->GetJob());

        root["level"] = player->GetLevel();

        root["currentHp"] = player->GetHp();

        root["currentExp"] = player->GetCurrentExp();

        // 인벤토리는 나중에 추가
        root["inventory"] = json::array();

        std::ofstream file(saveFilePath);

        if (!file.is_open())
        {
            cerr << "Save File Generating Failed" << endl;

            return false;
        }

        file << root.dump(4);

        cout << "Game Save Succeeded" << endl;

        return true;
    }
    catch (const std::exception& exception)
    {
        cerr << "Game Save Failed : " << exception.what() << endl;

        return false;
    }
}

bool SaveManager::LoadPlayerSaveData(PlayerSaveData& saveData)
{
    if (!HasSaveData())
    {
        return false;
    }

    std::ifstream file(saveFilePath);

    if (!file.is_open())
    {
        return false;
    }

    try
    {
        json root;

        file >> root;

        saveData.job =
            StringToPlayerType(
                root.at("job")
                .get<std::string>()
            );

        saveData.level =
            root.at("level")
            .get<int>();

        saveData.currentHp =
            root.at("currentHp")
            .get<int>();

        saveData.currentExp =
            root.at("currentExp")
            .get<int>();

        saveData.inventory.clear();

        if (root.contains("inventory"))
        {
            for (const auto& item :
                root["inventory"])
            {
                saveData.inventory.push_back(item.get<std::string>());
            }
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "Save JSON Parsing Failed: " << exception.what() << endl;

        return false;
    }

    return true;
}

bool SaveManager::HasSaveData() const
{
    if (!std::filesystem::exists(saveFilePath))
    {
        return false;
    }

    if (std::filesystem::file_size(saveFilePath) == 0)
    {
        return false;
    }

    return true;
}

void SaveManager::DeleteSaveData()
{
    if (!HasSaveData())
    {
        return;
    }

    std::filesystem::remove(saveFilePath);

    cout << "Save Data Deleted" << endl;
}