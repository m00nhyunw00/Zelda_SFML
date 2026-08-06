#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "PlayerType.h"
#include "MonsterType.h"
#include "MonsterColor.h"

class EntityManager
{
private:
    Player* player;
    std::vector<Entity*> entities;

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

    const std::vector<Entity*>& GetEntities() const { return entities; }

    void AddEntity(Entity* entity);

    void CheckCollisions();

    void Update(float deltaTime, sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void RemoveInactiveEntities();

    void PrintPlayerInfo();
};

