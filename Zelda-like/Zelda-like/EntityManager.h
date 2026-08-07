#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "PlayerType.h"
#include "MonsterType.h"
#include "MonsterColor.h"

class EntityManager
{
private:
    Player* player;
    std::vector<Monster*> monsters;

public:
    EntityManager();
    ~EntityManager();

    void CreatePlayer(
        const std::string& name,
        PlayerType job,
        const sf::Vector2f& position
    );

    void SpawnMonster(
        MonsterType type,
        MonsterColor color,
        const sf::Vector2f& position
    );

    void SetPlayer(Player* player) { this->player = player; }
    Player* GetPlayer() const { return player; }

    const std::vector<Monster*>& GetMonsters() const { return monsters; }

    void AddMonster(Monster* monster);

    void ClearMonsters();

    void DeletePlayer();

    void CheckCollisions();
    void CheckPlayerMonsterCollisions();
    void CheckPlayerAttackCollisions();
    void CheckMonsterAttackCollisions();
    void CheckProjectileCollisions();

    void Update(float deltaTime, sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void RemoveInactiveEntities();

    void PrintPlayerInfo();
    void PrintPlayerHp();
};

