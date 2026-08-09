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

    //bool isGameOver;

    //// ----- 게임 오버 시 UI ------------------------

    //sf::RectangleShape* gameOverOverlay;
    //sf::Text* gameOverText;

    //sf::RectangleShape* restartButton;
    //sf::Text* restartText;
    //sf::RectangleShape* exitButton;
    //sf::Text* exitText;

    //// ----- 체력 게이지 UI ------------------------

    //sf::RectangleShape* hpBarBackground = nullptr;
    //sf::RectangleShape* hpBar = nullptr;

    //sf::Text* hpText = nullptr;

public:
    Scene(SceneManager* manager, EntityManager* entityManager);

    virtual ~Scene() = default;

    virtual void HandleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;

    virtual void Update(float deltaTime, sf::RenderWindow& window) = 0;

    virtual void Render(sf::RenderWindow& window) = 0;
};