#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"

class EntityManager
{
private:
    Player* player;
    std::vector<Entity*> entities;

public:
    EntityManager();
    ~EntityManager();

    void SetPlayer(Player* player) { this->player = player; }
    Player* GetPlayer() const { return player; }

    void AddEntity(Entity* entity);

    void Update(float deltaTime, sf::RenderWindow& window);

    void Render(sf::RenderWindow& window);

    void RemoveInactiveEntities();
};

