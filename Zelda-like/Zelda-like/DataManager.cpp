#include "DataManager.h"

#include <fstream>
#include <iostream>

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

MonsterType DataManager::StringToMonsterType(const std::string& value) const
{
    if (value == "SLIME")
    {
        return MonsterType::SLIME;
    }

    return MonsterType::NONE_MONSTER;
}

bool DataManager::LoadPlayerData(
    const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr
            << "플레이어 데이터 파일을 열 수 없습니다: "
            << filePath
            << std::endl;

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
                std::cerr
                    << "알 수 없는 플레이어 직업 데이터: "
                    << jobName
                    << std::endl;

                continue;
            }

            PlayerData data;

            data.maxHp = value.at("maxHp").get<int>();

            data.defence = value.at("defence").get<int>();

            data.damage = value.at("damage").get<int>();

            data.evasionRate = value.at("evasionRate").get<int>();

            data.moveSpeed = value.at("moveSpeed").get<float>();

            data.skillName = value.at("skillName").get<std::string>();

            data.skillDamage = value.at("skillDamage").get<int>();

            data.maxSkillCooldown = value.at("maxSkillCooldown").get<float>();

            playerDataTable[playerType] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "플레이어 JSON 파싱 실패: " << exception.what() << std::endl;

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
        std::cerr
            << "몬스터 데이터 파일을 열 수 없습니다: "
            << filePath
            << std::endl;

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
                std::cerr
                    << "알 수 없는 몬스터 데이터: "
                    << typeName
                    << std::endl;

                continue;
            }

            MonsterData data;

            data.maxHp = value.at("maxHp").get<int>();

            data.defence = value.at("defence").get<int>();

            data.damage = value.at("damage").get<int>();

            data.evasionRate = value.at("evasionRate").get<int>();

            data.moveSpeed = value.at("moveSpeed").get<float>();

            data.detectionRange = value.at("detectionRange").get<float>();

            data.attackRange = value.at("attackRange").get<float>();

            data.color = MonsterColor::NONE_COLOR;

            monsterDataTable[monsterType] = data;
        }
    }
    catch (const json::exception& exception)
    {
        cerr << "몬스터 JSON 파싱 실패: " << exception.what() << std::endl;

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
        cerr << "애니메이션 데이터 파일을 열 수 없습니다: " << filePath << endl;

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
        cerr << "애니메이션 JSON 파싱 실패: " << exception.what() << endl;

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