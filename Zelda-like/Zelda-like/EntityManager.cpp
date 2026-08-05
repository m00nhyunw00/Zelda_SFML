#include "EntityManager.h"
#include "DataManager.h"
#include "Warrior.h"
#include "Archer.h"
#include "Mage.h"

#include <iostream>

using namespace std;

EntityManager::EntityManager()
{
    player = nullptr;
}

EntityManager::~EntityManager()
{
    for (Entity* entity : entities)
    {
        delete entity;
    }

    entities.clear();

    player = nullptr;
}

void EntityManager::CreatePlayer(
    const std::string& name,
    PlayerType job,
    const sf::Vector2f& position
)
{
    DataManager& dataManager = DataManager::GetInstance();

    const PlayerData* data = dataManager.GetPlayerData(job);

    if (data == nullptr)
        return;

    switch (job)
    {
    case PlayerType::WARRIOR:
        player = new Warrior(name, *data, position);
        break;
    case PlayerType::ARCHER:
        player = new Archer(name, *data, position);
        break;
    case PlayerType::MAGE:
        player = new Mage(name, *data, position);
        break;
    default:
        return;
    }

    AddEntity(player);
}

void EntityManager::AddEntity(Entity* entity)
{
    if (entity == nullptr)
        return;

    entities.push_back(entity);
}

void EntityManager::RemoveInactiveEntities()
{
    for (auto iterator = entities.begin(); iterator != entities.end();)
    {
        Entity* entity = *iterator;

        if (entity == nullptr || !entity->IsActive())
        {
            if (entity == player)
                player = nullptr;

            delete entity;
            iterator = entities.erase(iterator);
        }
        else
        {
            iterator++;
        }
    }
}

void EntityManager::Update(float deltaTime, sf::RenderWindow& window)
{
    for (Entity* entity : entities)
    {
        if (entity != nullptr && entity->IsActive())
        {
            entity->Update(deltaTime, window);
        }
    }

    RemoveInactiveEntities();
}

void EntityManager::Render(sf::RenderWindow& window)
{
    for (Entity* entity : entities)
        if (entity != nullptr && entity->IsActive())
            entity->Render(window);
}

void EntityManager::PrintPlayerInfo()
{
    using std::cout;
    using std::endl;

    if (player == nullptr)
    {
        cout << "Player가 생성되지 않았습니다." << endl;
        return;
    }

    cout << "================ Player Info ================" << endl;
    cout << "Name              : " << player->GetUserName() << endl;
    cout << "Job               : " << static_cast<int>(player->GetJob()) << endl;
    cout << "Level             : " << player->GetLevel() << endl;

    cout << "HP                : "
        << player->GetHp() << " / "
        << player->GetMaxHp() << endl;

    cout << "Damage            : " << player->GetDamage() << endl;
    cout << "Defence           : " << player->GetDefence() << endl;
    cout << "Evasion Rate      : " << player->GetEvasionRate() << endl;
    cout << "Move Speed        : " << player->GetMoveSpeed() << endl;

    cout << "Skill             : " << player->GetSkillName() << endl;
    cout << "Skill Damage      : " << player->GetSkillDamage() << endl;
    cout << "Skill Cooldown    : "
        << player->GetSkillCooldown()
        << " / "
        << player->GetMaxSkillCooldown()
        << endl;

    cout << "EXP               : "
        << player->GetCurrentExp()
        << " / "
        << player->GetMaxExp()
        << endl;

    cout << "Position          : ("
        << player->GetPosition().x
        << ", "
        << player->GetPosition().y
        << ")" << endl;

    cout << "=============================================" << endl;
}