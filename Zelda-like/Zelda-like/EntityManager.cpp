#include "EntityManager.h"
#include "DataManager.h"
#include "Warrior.h"
#include "Archer.h"
#include "Mage.h"
#include "Slime.h"

#include <iostream>

using namespace std;

EntityManager::EntityManager()
{
    player = nullptr;
}

EntityManager::~EntityManager()
{
    for (Monster* monster : monsters)
    {
        delete monster;
    }

    monsters.clear();

    delete player;
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
}

void EntityManager::SpawnMonster(
    MonsterType type,
    MonsterColor color,
    const sf::Vector2f& position)
{
    const MonsterData* data =
        DataManager::GetInstance().GetMonsterData(type);

    if (data == nullptr)
    {
        std::cout << "MonsterData dosen't exist" << std::endl;
        return;
    }

    Monster* monster = nullptr;

    switch (type)
    {
    case MonsterType::SLIME:
        monster = new Slime(color, *data, position);
        break;

    default:
        std::cout << "Not Valid MonsterType" << std::endl;
        return;
    }

    monster->SetTarget(player);

    AddMonster(monster);

    cout << "Slime Spawned: " << position.x << ", " << position.y << endl;
}

void EntityManager::AddMonster(Monster* monster)
{
    if (monster == nullptr)
        return;

    monsters.push_back(monster);
}

void EntityManager::ClearMonsters()
{
    monsters.clear();
}

void EntityManager::DeletePlayer()
{
    delete player;
}

void EntityManager::CheckCollisions()
{
    // Player ↔ Monster 몸체 충돌
    CheckPlayerMonsterCollisions();

    // Player의 공격
    CheckPlayerAttackCollisions();

    // Monster의 공격
    CheckMonsterAttackCollisions();

    // Projectile ↔ Monster / Player 공격 충돌
    CheckProjectileCollisions();
}

void EntityManager::CheckPlayerMonsterCollisions()
{
    if (player == nullptr ||
        !player->IsActive())
    {
        return;
    }

    for (Monster* monster : monsters)
    {
        if (monster == nullptr ||
            !monster->IsActive())
        {
            continue;
        }

        if (player->GetBodyCollider().Collision(
            monster->GetBodyCollider()))
        {
            cout << "Player Monster Body Collision!" << endl;

            player->MoveForce(
                player->GetPreviousPosition()
            );

            monster->MoveForce(
                monster->GetPreviousPosition()
            );
        }
    }
}

void EntityManager::CheckPlayerAttackCollisions()
{
    if (player == nullptr ||
        !player->IsActive())
    {
        return;
    }

    // 이번 프레임에 공격을 시작한 게 아니면 검사하지 않음
    if (!player->IsAttackTriggered())
    {
        return;
    }

    for (Monster* monster : monsters)
    {
        if (monster == nullptr ||
            !monster->IsActive())
        {
            continue;
        }

        if (player->GetAttackCollider().Collision(
            monster->GetBodyCollider()))
        {
            player->Attack(monster);
        }
    }
}

void EntityManager::CheckMonsterAttackCollisions()
{
    for (Monster* monster : monsters)
    {
        if (!monster->IsAttackTriggered())
            continue;

        if (monster->GetAttackCollider().Collision(player->GetBodyCollider()))
        {
            monster->Attack(player);
        }
    }
}


// 투사체 충돌 함수
void EntityManager::CheckProjectileCollisions()
{
    // TODO
}

void EntityManager::RemoveInactiveEntities()
{
    for (auto iterator = monsters.begin();
        iterator != monsters.end();)
    {
        Monster* monster = *iterator;

        if (monster == nullptr ||
            !monster->IsActive())
        {
            delete monster;

            iterator =
                monsters.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    }
}

void EntityManager::Update(
    float deltaTime,
    sf::RenderWindow& window)
{
    int beforeHp = player->GetHp();

    if (player != nullptr &&
        player->IsActive())
    {
        player->Update(
            deltaTime,
            window
        );
    }

    for (Monster* monster : monsters)
    {
        if (monster != nullptr &&
            monster->IsActive())
        {
            monster->Update(
                deltaTime,
                window
            );
        }
    }

    CheckCollisions();

    int afterHp = player->GetHp();

    if (beforeHp != afterHp)
        PrintPlayerHp();

    RemoveInactiveEntities();
}

void EntityManager::Render(
    sf::RenderWindow& window)
{
    if (player != nullptr && player->IsActive())
    {
        player->Render(window);
    }

    for (Monster* monster : monsters)
    {
        if (monster != nullptr &&
            monster->IsActive())
        {
            monster->Render(window);
        }
    }
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

void EntityManager::PrintPlayerHp()
{
    if (player == nullptr)
    {
        cout << "Player가 생성되지 않았습니다." << endl;
        return;
    }

    cout << "=============================================" << endl;


    cout << "HP : "<< player->GetHp() << " / "<< player->GetMaxHp() << endl;

    cout << "=============================================" << endl;
}