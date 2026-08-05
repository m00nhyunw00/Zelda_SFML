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

bool DataManager::LoadPlayerData(const string& filePath)
{
    ifstream file(filePath);

    if (!file.is_open())
    {
        cerr << "플레이어 데이터 파일을 열 수 없습니다: " << filePath << endl;

        return false;
    }

    try
    {
        json root;
        file >> root;

        playerDataTable.clear();

        PlayerData data;
        string job;

        // Warrior 데이터 로드 -------------------------------------

        job = "WARRIOR";

        data.maxHp = root.at(job).at("maxHp").get<int>();

        data.defence = root.at(job).at("defence").get<int>();

        data.damage = root.at(job).at("damage").get<int>();

        data.evasionRate = root.at(job).at("evasionRate").get<int>();

        data.moveSpeed = root.at(job).at("moveSpeed").get<float>();

        data.skillName = root.at(job).at("skillName").get<string>();

        data.skillDamage = root.at(job).at("skillDamage").get<int>();

        data.maxSkillCooldown = root.at(job).at("maxSkillCooldown").get<float>();

        playerDataTable[PlayerType::WARRIOR] = data;

        // Archer 데이터 로드 -------------------------------------

        job = "ARCHER";

        data.maxHp = root.at(job).at("maxHp").get<int>();

        data.defence = root.at(job).at("defence").get<int>();

        data.damage = root.at(job).at("damage").get<int>();

        data.evasionRate = root.at(job).at("evasionRate").get<int>();

        data.moveSpeed = root.at(job).at("moveSpeed").get<float>();

        data.skillName = root.at(job).at("skillName").get<string>();

        data.skillDamage = root.at(job).at("skillDamage").get<int>();

        data.maxSkillCooldown = root.at(job).at("maxSkillCooldown").get<float>();

        playerDataTable[PlayerType::ARCHER] = data;

        // Mage 데이터 로드 -------------------------------------

        job = "MAGE";

        data.maxHp = root.at(job).at("maxHp").get<int>();

        data.defence = root.at(job).at("defence").get<int>();

        data.damage = root.at(job).at("damage").get<int>();

        data.evasionRate = root.at(job).at("evasionRate").get<int>();

        data.moveSpeed = root.at(job).at("moveSpeed").get<float>();

        data.skillName = root.at(job).at("skillName").get<string>();

        data.skillDamage = root.at(job).at("skillDamage").get<int>();

        data.maxSkillCooldown = root.at(job).at("maxSkillCooldown").get<float>();

        playerDataTable[PlayerType::MAGE] = data;
    }
    catch (const json::exception& exception)
    {
        cerr << "플레이어 JSON 파싱 실패: " << exception.what() << endl;

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