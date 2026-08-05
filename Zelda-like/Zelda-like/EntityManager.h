#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "PlayerType.h"

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

    void SetPlayer(Player* player) { this->player = player; }
    Player* GetPlayer() const { return player; }

    void AddEntity(Entity* entity);

    void Update(float deltaTime, sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void RemoveInactiveEntities();

    void PrintPlayerInfo();
};

