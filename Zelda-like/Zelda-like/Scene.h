#pragma once

#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class Player;
class SceneManager;

class Scene
{
protected:
    Player* player;
    SceneManager* sceneManager;
    EntityManager entityManager;

public:
    Scene(SceneManager* manager, Player* player);

    virtual ~Scene() = default;

    virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual void Update(float deltaTime, sf::RenderWindow& window) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;
};