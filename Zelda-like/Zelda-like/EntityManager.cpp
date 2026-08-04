#include "EntityManager.h"

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
    if (player != nullptr && player->IsActive())
    {
        // player update()
        player->Update(deltaTime, window);
    }

    for (Entity* entity : entities)
    {
        if (entity != nullptr && entity->IsActive())
        {
            // ±âÅ¸ entity update()
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