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

    const std::vector<Monster*>& GetMonsters() const { return monsters; }

    void AddMonster(Monster* monster);
    void AddProjectile(Projectile* projectile);

    void ClearMonsters();
    void ClearProjectiles();
    void DeletePlayer();
    void ResetGame();

    void CheckCollisions();
    void CheckProjectileWallCollisions(const std::vector<Collider>& walls);
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

