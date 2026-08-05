#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "PlayerType.h"

class HomeScene : public Scene
{
private:

public:
    HomeScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~HomeScene();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};