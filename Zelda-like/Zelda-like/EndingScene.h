#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"

class EndingScene : public Scene
{
private:
    sf::Sprite* endingSprite = nullptr;

public:
    EndingScene(SceneManager* sceneManager,EntityManager* entityManager);

    ~EndingScene();

    void HandleEvent(const sf::Event& event,sf::RenderWindow& window) override;

    void Update(float deltaTime,sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;
};