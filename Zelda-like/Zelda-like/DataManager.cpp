#include "DataManager.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

DataManager& DataManager::GetInstance()
{
    static DataManager instance;

    return instance;
}

PlayerType DataManager::StringToPlayerType(const std::string& value) const
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

MonsterType DataManager::StringToMonsterType(
    const std::string& value) const
{
    if (value == "SLIME")
    {
        return MonsterType::SLIME;
    }

    if (value == "CACTO")
    {
        return MonsterType::CACTO;
    }

    if (value == "SKELETON")
    {
        return MonsterType::SKELETON;
    }

    if (value == "LICH")
    {
        return MonsterType::LICH;
    }

    if (value == "GIANT_SLIME")
    {
        return MonsterType::GIANT_SLIME;
    }

    return MonsterType::NONE_MONSTER;
}

bool DataManager::LoadPlayerData(
    const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] PlayerData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        playerDataTable.clear();

        for (const auto& item : root.items())
        {
            const std::string& jobName = item.key();
            const json& value = item.value();

            const PlayerType playerType = StringToPlayerType(jobName);

            if (playerType == PlayerType::NONE_PLAYER)
            {
                cerr << "[DataManager] PlayerData - Unknown player type: " << jobName << endl;

                continue;
            }

            PlayerData data;

            data.moveSpeed = value.at("moveSpeed").get<float>();

            data.skillName = value.at("skillName").get<std::string>();

            data.maxSkillCooldown = value.at("maxSkillCooldown").get<float>();

            data.ultimateName = value.at("ultimateName").get<std::string>();

            playerDataTable[playerType] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "[DataManager] PlayerData - JSON parsing failed: " << exception.what() << endl;

        playerDataTable.clear();

        return false;
    }

    return true;
}

const PlayerData* DataManager::GetPlayerData(PlayerType type) const
{
    auto iterator = playerDataTable.find(type);

    if (iterator == playerDataTable.end())
        return nullptr;

    return &iterator->second;
}

bool DataManager::LoadMonsterData(
    const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] MonsterData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        monsterDataTable.clear();

        for (const auto& item : root.items())
        {
            const std::string& typeName = item.key();
            const json& value = item.value();

            const MonsterType monsterType = StringToMonsterType(typeName);

            if (monsterType == MonsterType::NONE_MONSTER)
            {
                cerr << "[DataManager] MonsterData - Unknown monster type: " << typeName << endl;

                continue;
            }

            MonsterData data;

            data.maxHp = value.at("maxHp").get<int>();

            data.defence = value.at("defence").get<int>();

            data.damage = value.at("damage").get<int>();

            data.moveSpeed = value.at("moveSpeed").get<float>();

            data.detectionRange = value.at("detectionRange").get<float>();

            data.attackRange = value.at("attackRange").get<float>();

            data.exp = value.at("exp").get<int>();

            data.attackCooldown = value.at("attackCooldown").get<float>();

            data.color = MonsterColor::NONE_COLOR;

            monsterDataTable[monsterType] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "[DataManager] MonsterData - JSON parsing failed: " << exception.what() << endl;

        monsterDataTable.clear();

        return false;
    }

    return true;
}

const MonsterData* DataManager::GetMonsterData(MonsterType type) const
{
    auto iterator = monsterDataTable.find(type);

    if (iterator == monsterDataTable.end())
        return nullptr;

    return &iterator->second;
}

bool DataManager::LoadAnimationData(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] AnimationData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        nlohmann::json jsonData;

        file >> jsonData;

        animationDataTable.clear();

        for (const auto& item : jsonData.items())
        {
            const string& key = item.key();
            const nlohmann::json& value = item.value();

            AnimationData data;

            data.textureKey = value.at("textureKey").get<std::string>();

            data.totalRows = value.at("totalRows").get<int>();

            data.totalColumns = value.at("totalColumns").get<int>();

            data.frameCount = value.at("frameCount").get<int>();

            data.frameDuration = value.at("frameDuration").get<float>();

            animationDataTable[key] = data;
        }
    }
    catch (const nlohmann::json::exception& exception)
    {
        cerr << "[DataManager] AnimationData - JSON parsing failed: " << exception.what() << endl;

        return false;
    }

    return true;
}

const AnimationData* DataManager::GetAnimationData(const std::string& key) const
{
    auto iterator = animationDataTable.find(key);

    if (iterator == animationDataTable.end())
        return nullptr;

    return &iterator->second;
}

bool DataManager::LoadPlayerLevelData(
    const std::string& filePath)
{
    cout << "[DataManager] PlayerLevelData - Current path: " << std::filesystem::current_path() << endl;

    cout << "[DataManager] PlayerLevelData - Loading: " << std::filesystem::absolute(filePath) << endl;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] PlayerLevelData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        playerLevelDataTable.clear();

        for (const auto& jobItem : root.items())
        {
            const PlayerType playerType = StringToPlayerType(jobItem.key());

            if (playerType == PlayerType::NONE_PLAYER)
            {
                continue;
            }

            for (const auto& levelItem : jobItem.value().items())
            {
                int level = std::stoi(levelItem.key());

                const json& value = levelItem.value();

                PlayerLevelData data;

                data.maxExp = value.at("maxExp").get<int>();

                data.maxHp = value.at("maxHp").get<int>();

                data.defence = value.at("defence").get<int>();

                data.damage = value.at("damage").get<int>();

                data.skillDamage = value.at("skillDamage").get<int>();

                data.ultimateDamage = value.at("ultimateDamage").get<int>();

                data.maxUltimateGauge = value.at("maxUltimateGauge").get<float>();

                playerLevelDataTable[playerType][level] = data;
            }
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "[DataManager] PlayerLevelData - JSON parsing failed: " << exception.what() << endl;

        playerLevelDataTable.clear();

        return false;
    }

    return true;
}

const PlayerLevelData* DataManager::GetPlayerLevelData(PlayerType type, int level) const
{
    auto playerIterator = playerLevelDataTable.find(type);

    if (playerIterator == playerLevelDataTable.end())
    {
        return nullptr;
    }

    auto levelIterator = playerIterator->second.find(level);

    if (levelIterator == playerIterator->second.end())
    {
        return nullptr;
    }

    return &levelIterator->second;
}

bool DataManager::LoadMonsterLevelData(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] MonsterLevelData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        monsterLevelDataTable.clear();

        for (const auto& item : root.items())
        {
            int level = std::stoi(item.key());

            const json& value = item.value();

            MonsterLevelData data;

            data.hpRate = value.at("hpRate").get<float>();

            data.damageRate = value.at("damageRate").get<float>();

            data.defenceRate = value.at("defenceRate").get<float>();

            data.expRate = value.at("expRate").get<float>();

            data.moveSpeedRate = value.at("moveSpeedRate").get<float>();

            monsterLevelDataTable[level] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "[DataManager] MonsterLevelData - JSON parsing failed: " << exception.what() << endl;

        monsterLevelDataTable.clear();

        return false;
    }

    return true;
}

const MonsterLevelData* DataManager::GetMonsterLevelData(int level) const
{
    auto iterator = monsterLevelDataTable.find(level);

    if (iterator == monsterLevelDataTable.end())
    {
        return nullptr;
    }

    return &iterator->second;
}

bool DataManager::LoadMonsterSpawnData(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "[DataManager] MonsterSpawnData - File open failed: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        monsterSpawnDataTable.clear();

        for (const auto& item : root.items())
        {
            const std::string& rangeKey = item.key();

            const json& value = item.value();

            MonsterSpawnData data;

            data.slimeRate = value.at("SLIME").get<int>();

            data.cactoRate = value.at("CACTO").get<int>();

            data.skeletonRate = value.at("SKELETON").get<int>();

            data.lichRate = value.at("LICH").get<int>();

            monsterSpawnDataTable[rangeKey] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "[DataManager] MonsterSpawnData - JSON parsing failed: " << exception.what() << endl;

        monsterSpawnDataTable.clear();

        return false;
    }

    return true;
}

const MonsterSpawnData* DataManager::GetMonsterSpawnData(int playerLevel) const
{
    std::string key;

    if (playerLevel <= 4)
    {
        key = "1-4";
    }
    else if (playerLevel <= 9)
    {
        key = "5-9";
    }
    else if (playerLevel <= 14)
    {
        key = "10-14";
    }
    else if (playerLevel <= 19)
    {
        key = "15-19";
    }
    else if (playerLevel <= 24)
    {
        key = "20-24";
    }
    else if (playerLevel <= 29)
    {
        key = "25-29";
    }
    else
    {
        key = "30";
    }

    auto iterator = monsterSpawnDataTable.find(key);

    if (iterator == monsterSpawnDataTable.end())
    {
        return nullptr;
    }

    return &iterator->second;
}