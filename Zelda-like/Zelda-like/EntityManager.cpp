#include "EntityManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "Warrior.h"
#include "Archer.h"
#include "Mage.h"
#include "Slime.h"
#include "Constants.h"

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

    for (Projectile* projectile : projectiles)
    {
        delete projectile;
    }

    projectiles.clear();

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

void EntityManager::AddProjectile(Projectile* projectile)
{
    if (projectile == nullptr)
    {
        return;
    }

    projectiles.push_back(projectile);
}

void EntityManager::ClearMonsters()
{
    for (Monster* monster : monsters)
    {
        delete monster;
    }

    monsters.clear();
}

void EntityManager::ClearProjectiles()
{
    for (Projectile* projectile : projectiles)
    {
        delete projectile;
    }

    projectiles.clear();
}

void EntityManager::DeletePlayer()
{
    delete player;

    player = nullptr;
}

void EntityManager::ResetGame()
{
    ClearMonsters();
    ClearProjectiles();
    DeletePlayer();
}

void EntityManager::CheckCollisions()
{
    // Player ↔ Monster 몸체 충돌
    CheckPlayerMonsterCollisions();

    // Player의 근접 공격 충돌
    CheckPlayerMeleeAttackCollisions();

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

void EntityManager::CheckPlayerMeleeAttackCollisions()
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

    if (player->GetJob() != PlayerType::WARRIOR)
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

void EntityManager::HandlePlayerRangedAttack()
{
    if (player == nullptr ||
        !player->IsActive())
    {
        return;
    }

    // 원거리 직업만 처리
    if (player->GetJob() != PlayerType::ARCHER &&
        player->GetJob() != PlayerType::MAGE)
    {
        return;
    }

    // 이번 프레임에 아무 공격도 시작하지 않았다면 종료
    if (!player->IsAttackTriggered() &&
        !player->IsSkillTriggered() &&
        !player->IsUltimateTriggered())
    {
        return;
    }

    Creature* target = nullptr;

    float attackRange;
    float aimAngle;

    switch (player->GetJob())
    {
    case PlayerType::ARCHER:
        attackRange = Constants::ARCHER_ATTACK_RANGE;
        aimAngle = Constants::ARCHER_ANGLE;
        break;

    //case PlayerType::MAGE:
    //    attackRange = Constants::MAGE_ATTACK_RANGE;
    //    aimAngle = Constants::MAGE_ANGLE;
    //    break;

    default:
        return;
    }

    sf::Vector2f facing;

    switch (player->GetFacingDirection())
    {
    case Direction::UP:
        facing = { 0.f, -1.f };
        break;

    case Direction::DOWN:
        facing = { 0.f, 1.f };
        break;

    case Direction::LEFT:
        facing = { -1.f, 0.f };
        break;

    case Direction::RIGHT:
        facing = { 1.f, 0.f };
        break;
    }

    const float threshold =
        std::cos(
            aimAngle * 3.141592f / 180.f
        );

    float bestDot = threshold;

    for (Monster* monster : monsters)
    {
        if (monster == nullptr ||
            !monster->IsActive())
        {
            continue;
        }

        sf::Vector2f toMonster =
            monster->GetPosition() -
            player->GetPosition();

        const float distance =
            std::sqrt(
                toMonster.x * toMonster.x +
                toMonster.y * toMonster.y
            );

        if (distance <= 0.f ||
            distance > attackRange)
        {
            continue;
        }

        toMonster.x /= distance;
        toMonster.y /= distance;

        const float dot =
            facing.x * toMonster.x +
            facing.y * toMonster.y;

        if (dot >= bestDot)
        {
            bestDot = dot;
            target = monster;
        }
    }

    // 이번 프레임에 시작된 행동들만 실행
    if (player->IsAttackTriggered())
    {
        player->Attack(target);
    }
    else if (player->IsSkillTriggered())
    {
        player->UseSkill(target);
    }
    //else if (player->IsUltimateTriggered())
    //{
    //    player->UseUltimate(target);
    //}
}

// 투사체 충돌 함수
void EntityManager::CheckProjectileCollisions()
{
    for (Projectile* projectile : projectiles)
    {
        if (projectile == nullptr ||
            !projectile->IsActive())
        {
            continue;
        }

        Creature* owner = projectile->GetOwner();

        if (owner == nullptr)
        {
            continue;
        }

        // Player가 발사한 투사체
        if (owner->GetCategory() == CreatureType::PLAYER)
        {
            for (Monster* monster : monsters)
            {
                if (monster == nullptr ||
                    !monster->IsActive())
                {
                    continue;
                }

                if (projectile->GetCollider().Collision(
                    monster->GetBodyCollider()))
                {
                    monster->TakeDamage(
                        projectile->GetDamage()
                    );

                    projectile->SetActive(false);

                    break;
                }
            }
        }

        // Monster가 발사한 투사체
        else if (owner->GetCategory() == CreatureType::MONSTER)
        {
            if (player == nullptr ||
                !player->IsActive())
            {
                continue;
            }

            if (projectile->GetCollider().Collision(
                player->GetBodyCollider()))
            {
                player->TakeDamage(
                    projectile->GetDamage()
                );

                projectile->SetActive(false);
            }
        }
    }
}

void EntityManager::RemoveInactiveEntities()
{
    // Monster 제거
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

    // Projectile 제거
    for (auto iterator = projectiles.begin();
        iterator != projectiles.end();)
    {
        Projectile* projectile =
            *iterator;

        if (projectile == nullptr ||
            !projectile->IsActive())
        {
            delete projectile;

            iterator =
                projectiles.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    }
}

void EntityManager::Update(float deltaTime, sf::RenderWindow& window)
{
    if (player == nullptr)
    {
        return;
    }

    int beforeHp = player->GetHp();

    if (player->IsActive())
    {
        player->Update(
            deltaTime,
            window
        );

        HandlePlayerRangedAttack();

        std::vector<Projectile*> newProjectiles = player->TakePendingProjectiles();

        for (Projectile* projectile : newProjectiles)
        {
            AddProjectile(projectile);
        }
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

    for (Projectile* projectile : projectiles)
    {
        if (projectile != nullptr &&
            projectile->IsActive())
        {
            projectile->Update(
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

void EntityManager::Render(sf::RenderWindow& window)
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

    for (Projectile* projectile : projectiles)
    {
        if (projectile != nullptr &&
            projectile->IsActive())
        {
            projectile->Render(window);
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