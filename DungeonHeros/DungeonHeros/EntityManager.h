#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "PlayerSaveData.h"
#include "Monster.h"
#include "PlayerType.h"
#include "MonsterType.h"
#include "MonsterColor.h"
#include "Projectile.h"
#include "ProjectileType.h"

class EntityManager
{
private:
    Player* player;
    std::vector<Monster*> monsters;
    std::vector<Projectile*> projectiles;

public:
    EntityManager();
    ~EntityManager();

    void CreatePlayer(
        const std::string& name,
        const PlayerSaveData& saveData,
        const sf::Vector2f& position
    );

    Monster* SpawnMonster(
        MonsterType type,
        MonsterColor color,
        const sf::Vector2f& position
    );

    void SetPlayer(Player* player) { this->player = player; }
    Player* GetPlayer() const { return player; }

    void AddMonster(Monster* monster);
    const std::vector<Monster*>& GetMonsters() const { return monsters; }

    void AddProjectile(Projectile* projectile);

    void ClearMonsters();
    void ClearProjectiles();
    void DeletePlayer();
    void ResetGame();

    void CheckCollisions();
    void CheckCreatureObjectCollisions(const std::vector<Collider>& objectColliders);
    void CheckProjectileWallCollisions(const std::vector<Collider>& wallColliders);
    void ResolveObjectCollision(Creature* creature, const std::vector<Collider>& colliders);
    void CheckPlayerMonsterCollisions();
    void CheckPlayerMeleeAttackCollisions();
    void CheckMonsterAttackCollisions();
    void HandlePlayerProjectileAttack();
    void CheckProjectileCollisions();

    bool AreAllMonstersDead();

    void Update(float deltaTime, sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void RemoveInactiveEntities();

    void PrintPlayerInfo();
    void PrintPlayerHp();
};

