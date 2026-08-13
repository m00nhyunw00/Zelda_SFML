#include "EntityManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "Warrior.h"
#include "Archer.h"
#include "Mage.h"
#include "Slime.h"
#include "Cacto.h"
#include "Skeleton.h"
#include "GiantSlime.h"
#include "Lich.h"
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

//

void EntityManager::CreatePlayer(
    const std::string& name,
    const PlayerSaveData& saveData,
    const sf::Vector2f& position
)
{
    DataManager& dataManager = DataManager::GetInstance();

    const PlayerData* basicData = dataManager.GetPlayerData(saveData.job);

    if (basicData == nullptr)
    {
        return;
    }

    const PlayerLevelData* levelData = dataManager.GetPlayerLevelData(saveData.job, saveData.level);

    if (levelData == nullptr)
    {
        return;
    }

    // 기존 플레이어가 있다면 제거
    DeletePlayer();

    switch (saveData.job)
    {
    case PlayerType::WARRIOR:
        player = new Warrior(
            name,
            *basicData,
            *levelData,
            saveData,
            position
        );

        break;

    case PlayerType::ARCHER:

        player = new Archer(
            name,
            *basicData,
            *levelData,
            saveData,
            position
        );

        break;

    case PlayerType::MAGE:

        player = new Mage(
            name,
            *basicData,
            *levelData,
            saveData,
            position
        );

        break;

    default:
        return;
    }

    PrintPlayerInfo();
}

Monster* EntityManager::SpawnMonster(
    MonsterType type,
    MonsterColor color,
    const sf::Vector2f& position)
{
    const MonsterData* baseData = DataManager::GetInstance().GetMonsterData(type);

    if (baseData == nullptr)
    {
        return nullptr;
    }

    // 기본 데이터 복사
    MonsterData data = *baseData;

    // 보스는 고정 스탯
    if (type != MonsterType::GIANT_SLIME)
    {
        int monsterLevel = 1;

        if (player != nullptr)
        {
            monsterLevel = player->GetLevel();
        }

        const MonsterLevelData* levelData = DataManager::GetInstance().GetMonsterLevelData(monsterLevel);

        if (levelData != nullptr)
        {
            data.maxHp = static_cast<int>(data.maxHp * levelData->hpRate);

            data.damage = static_cast<int>(data.damage * levelData->damageRate);

            data.defence = static_cast<int>(data.defence * levelData->defenceRate);

            data.exp = static_cast<int>(data.exp * levelData->expRate);

            data.moveSpeed *= levelData->moveSpeedRate;
        }
    }

    Monster* monster = nullptr;

    switch (type)
    {
    case MonsterType::SLIME:
        monster = new Slime(color, data, position);
        break;

    case MonsterType::CACTO:
        monster = new Cacto(color, data, position);
        break;

    case MonsterType::SKELETON:
        monster = new Skeleton(color, data, position);
        break;

    case MonsterType::LICH:
        monster = new Lich(color, data, position);
        break;

    case MonsterType::GIANT_SLIME:
        monster = new GiantSlime(data, position);
        break;

    default:
        return nullptr;
    }

    if (monster == nullptr)
    {
        return nullptr;
    }

    monster->SetTarget(player);

    monsters.push_back(monster);

    return monster;
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

void EntityManager::CheckCreatureObjectCollisions(const std::vector<Collider>& objectColliders)
{
    // ---------------- Player ----------------

    if (player != nullptr && player->IsActive())
    {
        for (const Collider& collider : objectColliders)
        {
            if (player->GetBodyCollider().Collision(collider))
            {
                player->MoveForce(player->GetPreviousPosition());

                break;
            }
        }
    }

    // ---------------- Monster ----------------

    for (Monster* monster : monsters)
    {
        if (monster == nullptr ||
            !monster->IsActive())
        {
            continue;
        }

        // 고정된 몬스터는 위치 보정을 하지 않음
        if (!monster->CanMove())
        {
            continue;
        }

        ResolveObjectCollision(monster, objectColliders);
    }
}

void EntityManager::CheckProjectileWallCollisions(const std::vector<Collider>& wallColliders)
{
    for (Projectile* projectile : projectiles)
    {
        if (projectile == nullptr ||
            !projectile->IsActive())
        {
            continue;
        }

        if (projectile->GetType() == ProjectileType::ARROW && projectile->IsUltimateProjectile())
        {
            continue;
        }

        for (const Collider& wallCollider : wallColliders)
        {
            if (projectile->GetCollider().Collision(wallCollider))
            {
                projectile->OnWallCollision();
                break;
            }
        }
    }
}

void EntityManager::ResolveObjectCollision(Creature* creature, const std::vector<Collider>& colliders)
{
    if (creature == nullptr)
    {
        return;
    }

    const sf::Vector2f currentPosition =creature->GetPosition();

    const sf::Vector2f previousPosition =creature->GetPreviousPosition();

    // 현재 위치에서 실제로 충돌하고 있는지 확인
    bool collided = false;

    for (const Collider& collider : colliders)
    {
        if (creature->GetBodyCollider().Collision(collider))
        {
            collided = true;
            break;
        }
    }

    if (!collided)
    {
        return;
    }

    // =========================================================
    // 1. X 이동만 취소
    //
    // Y 방향 이동은 유지해서 벽을 따라 미끄러질 수 있게 함
    // =========================================================

    creature->MoveForce({
        previousPosition.x,
        currentPosition.y
        });

    bool stillColliding = false;

    for (const Collider& collider : colliders)
    {
        if (creature->GetBodyCollider().Collision(collider))
        {
            stillColliding = true;
            break;
        }
    }

    if (!stillColliding)
    {
        return;
    }


    // =========================================================
    // 2. Y 이동만 취소
    //
    // X 방향 이동은 유지
    // =========================================================

    creature->MoveForce({
        currentPosition.x,
        previousPosition.y
        });

    stillColliding = false;

    for (const Collider& collider : colliders)
    {
        if (creature->GetBodyCollider().Collision(collider))
        {
            stillColliding = true;
            break;
        }
    }

    if (!stillColliding)
    {
        return;
    }


    // =========================================================
    // 3. X/Y 둘 다 불가능한 경우에만 완전히 이전 위치로 복귀
    // =========================================================

    creature->MoveForce(
        previousPosition
    );
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
            //cout << "Player Monster Body Collision!" << endl;

            //player->MoveForce(
            //    player->GetPreviousPosition()
            //);

            //monster->MoveForce(
            //    monster->GetPreviousPosition()
            //);
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

    if (player->GetJob() != PlayerType::WARRIOR)
    {
        return;
    }

    Warrior* warrior =
        static_cast<Warrior*>(player);

    // 평타

    if (player->IsAttackTriggered())
    {
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

                break;
            }
        }
    }

    // Power Strike 시작

    if (player->IsSkillTriggered())
    {
        // 몬스터와 충돌하지 않아도 스킬 자체는 발동
        player->UseSkill(nullptr);
    }

    // Power Strike 진행 중 충돌 판정

    if (warrior->IsPowerStrike())
    {
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
                warrior->HitPowerStrike(monster);
            }
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

void EntityManager::HandlePlayerProjectileAttack()
{
    if (player == nullptr || !player->IsActive())
    {
        return;
    }

    if (!player->IsAttackTriggered() &&
        !player->IsSkillTriggered() &&
        !player->IsUltimateTriggered())
    {
        return;
    }

    Creature* target = nullptr;

    // Archer -----------------------------------------

    if (player->GetJob() == PlayerType::ARCHER)
    {
        const float attackRange = Constants::ARCHER_ATTACK_RANGE;

        const float aimAngle = Constants::ARCHER_ANGLE;

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

        if (player->IsAttackTriggered())
        {
            player->Attack(target);
        }
        else if (player->IsSkillTriggered())
        {
            player->UseSkill(target);
        }
        else if (player->IsUltimateTriggered())
        {
            player->UseUltimate(target);
        }

        return;
    }

    // Warrior -----------------------------------------

    if (player->GetJob() == PlayerType::WARRIOR)
    {
        // Warrior 평타/Power Strike는
        // 기존 근접 충돌 함수에서 처리

        // Blade Sweep만 Projectile 공격
        if (player->IsUltimateTriggered())
        {
            player->UseUltimate(nullptr);
        }

        return;
    }

    // Mage -----------------------------------------

    if (player->GetJob() == PlayerType::MAGE)
    {
        // 나중에 Fireball / 궁극기 처리
    }
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
                if (projectile == nullptr || !projectile->IsActive() || !projectile->IsCollisionEnabled())
                {
                    continue;
                }

                if (projectile->HasHitTarget(monster))
                {
                    continue;
                }

                if (projectile->GetCollider().Collision(monster->GetBodyCollider()))
                {
                    // 같은 투사체가 같은 몬스터를 여러 번 공격하지 않도록 기록
                    projectile->AddHitTarget(monster);

                    int realDamage = monster->TakeDamage(projectile->GetDamage());

                    Player* ownerPlayer = dynamic_cast<Player*>(projectile->GetOwner());

                    if (ownerPlayer != nullptr && !projectile->IsUltimateProjectile())
                    {
                        if (projectile->IsSkillProjectile())
                        {
                            ownerPlayer->AddUltimateGauge(realDamage * 2);
                            std::cout << realDamage * 2 << " Guage Charged (X2 Buff)" << endl;
                        }
                        else
                        {
                            ownerPlayer->AddUltimateGauge(realDamage);
                            std::cout << realDamage << " Guage Charged" << endl;
                        }
                    }

                    if (!projectile->IsPiercing())
                    {
                        projectile->SetActive(false);
                        break;
                    }
                }
            }
        }

        // Monster가 발사한 투사체
        else if (owner->GetCategory() == CreatureType::MONSTER)
        {
            if (player == nullptr ||
                !player->IsActive() ||
                !projectile->IsCollisionEnabled())
            {
                continue;
            }

            // Slime Spike -------------------------------
            if (projectile->GetType() ==
                ProjectileType::SLIME_SPIKE)
            {
                SlimeSpike* spike =
                    static_cast<SlimeSpike*>(projectile);

                // 아직 데미지 판정이 활성화되지 않았거나
                // 이미 플레이어를 한 번 때렸으면 검사 안 함
                if (!spike->CanDamage())
                {
                    continue;
                }

                if (spike->GetCollider().Collision(player->GetBodyCollider()))
                {
                    player->TakeDamage(spike->GetDamage());

                    // 한 Spike가 여러 프레임 연속 데미지를 주는 것 방지
                    spike->MarkDamaged();
                }

                // SlimeSpike는 여기서 삭제하지 않음.
                // 애니메이션/수명이 끝나면 스스로 비활성화
                continue;
            }

            // 일반 Monster Projectile -------------------------------
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
    for (auto iterator = monsters.begin(); iterator != monsters.end();)
    {
        Monster* monster = *iterator;

        //if (monster == nullptr || !monster->IsActive())
        //{
        //    if (monster != nullptr)
        //    {
        //        // 삭제될 몬스터가 발사한 투사체 먼저 비활성화
        //        for (Projectile* projectile : projectiles)
        //        {
        //            if (projectile != nullptr &&
        //                projectile->GetOwner() == monster)
        //            {
        //                projectile->SetActive(false);
        //            }
        //        }

        //        if (player != nullptr && player->IsActive())
        //        {
        //            player->AddExp(monster->GetExp());
        //        }
        //    }

        //    delete monster;
        //    iterator = monsters.erase(iterator);
        //}
        if (monster == nullptr || !monster->IsActive())
        {
            // Giant Slime은 BossScene에서 사망 처리를 할 때까지 유지
            if (monster != nullptr &&
                dynamic_cast<GiantSlime*>(monster) != nullptr)
            {
                ++iterator;
                continue;
            }

            if (monster != nullptr)
            {
                // 삭제될 몬스터가 발사한 투사체 먼저 비활성화
                for (Projectile* projectile : projectiles)
                {
                    if (projectile != nullptr &&
                        projectile->GetOwner() == monster)
                    {
                        projectile->SetActive(false);
                    }
                }

                if (player != nullptr && player->IsActive())
                {
                    player->AddExp(monster->GetExp());
                }
            }

            delete monster;
            iterator = monsters.erase(iterator);
        }
        else
        {
            ++iterator;
        }
    }

    // Projectile 제거
    for (auto iterator = projectiles.begin(); iterator != projectiles.end();)
    {
        Projectile* projectile = *iterator;

        if (projectile == nullptr || !projectile->IsActive())
        {
            delete projectile;
            iterator = projectiles.erase(iterator);
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

        HandlePlayerProjectileAttack();

        std::vector<Projectile*> newProjectiles = player->TakePendingProjectiles();

        for (Projectile* projectile : newProjectiles)
        {
            AddProjectile(projectile);
        }
    }

    for (Monster* monster : monsters)
    {
        if (monster == nullptr || !monster->IsActive())
        {
            continue;
        }

        monster->Update(deltaTime, window);

        std::vector<Projectile*> newProjectiles = monster->TakePendingProjectiles();

        for (Projectile* projectile : newProjectiles)
        {
            AddProjectile(projectile);
        }
    }

    for (Projectile* projectile : projectiles)
    {
        if (projectile != nullptr &&
            projectile->IsActive())
        {
            projectile->Update(deltaTime, window);
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

bool EntityManager::AreAllMonstersDead()
{
    for (const Monster* monster : monsters)
    {
        if (monster != nullptr && monster->IsActive())
        {
            return false;
        }
    }

    return true;
}

void EntityManager::PrintPlayerInfo()
{
    using std::cout;
    using std::endl;

    if (player == nullptr)
    {
        cout << "Player Not Genterated" << endl;
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

    cout << "Ultimate Skill             : " << player->GetUltimateName() << endl;
    cout << "Ultimate Skill Damage      : " << player->GetUltimateDamage() << endl;
    cout << "Ultimate Skill Gauge       : "
        << player->GetUltimateGauge()
        << " / "
        << player->GetMaxUltimateGauge()
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
        cout << "Player Not Generated" << endl;
        return;
    }

    cout << "=============================================" << endl;


    cout << "HP : "<< player->GetHp() << " / "<< player->GetMaxHp() << endl;

    cout << "=============================================" << endl;
}