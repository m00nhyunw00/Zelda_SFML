#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class EntityManager;
class SceneManager;

class Scene
{
protected:
    SceneManager* sceneManager;
    EntityManager* entityManager;

public:
    Scene(SceneManager* manager, EntityManager* entityManager);

    virtual ~Scene() = default;

    virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual void Update(float deltaTime, sf::RenderWindow& window) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;
};