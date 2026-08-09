#pragma once

#include "Scene.h"
#include "UI.h"
#include "Button.h"
#include "GaugeBar.h"

class InGameScene : public Scene
{
protected:
    bool isGameOver;

    // ----- 게임 오버 시 UI ------------------------

    sf::RectangleShape* gameOverOverlay;
    sf::Text* gameOverText;

    //sf::RectangleShape* restartButton;
    //sf::Text* restartText;
    //sf::RectangleShape* exitButton;
    //sf::Text* exitText;

    Button* restartButton = nullptr;
    Button* exitButton = nullptr;

    GaugeBar* playerHpBar = nullptr;
    GaugeBar* playerUltimateBar = nullptr;

public:
    InGameScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~InGameScene();

    bool CheckGameOver();

    void HandleGameOverEvent(const sf::Event& event, sf::RenderWindow& window);
    void UpdatePlayerHpBar(float deltaTime, sf::RenderWindow& window);
    void UpdateGameOver(float deltaTime, sf::RenderWindow& window);
    void RenderPlayerBar(sf::RenderWindow& window);
    void RenderGameOver(sf::RenderWindow& window);
};

