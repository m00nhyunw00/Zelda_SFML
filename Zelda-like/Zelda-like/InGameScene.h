#pragma once

#include "Scene.h"
#include "UI.h"
#include "Button.h"
#include "GaugeBar.h"
#include "SkillCooldownUI.h"

class InGameScene : public Scene
{
protected:
    std::vector<Collider> wallColliders;

    bool isGameOver;

    // ----- 게임 오버 시 UI ------------------------

    sf::RectangleShape* gameOverOverlay = nullptr;
    sf::Text* gameOverText = nullptr;

    Button* restartButton = nullptr;
    Button* exitButton = nullptr;

    GaugeBar* playerHpBar = nullptr;
    GaugeBar* playerUltimateBar = nullptr;

    SkillCooldownUI* skillCooldownUI = nullptr;

    GaugeBar* playerExpBar = nullptr;
    sf::Text* levelText = nullptr;

public:
    InGameScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~InGameScene();

    void UpdateProjectileWallCollisions();

    bool CheckGameOver();

    void HandleGameOverEvent(const sf::Event& event, sf::RenderWindow& window);
    void UpdateUI(float deltaTime,sf::RenderWindow& window);    
    void UpdatePlayerHpBar(float deltaTime, sf::RenderWindow& window);
    void UpdateSkillCooldownUI(float deltaTime, sf::RenderWindow& window);
    void UpdateUltimateGauge(float deltaTime, sf::RenderWindow& window);
    void UpdateGameOver(float deltaTime, sf::RenderWindow& window);
    void UpdateExpBar(float deltaTime, sf::RenderWindow& window);
    void UpdateLevelText();
    void RenderUI(sf::RenderWindow& window);
    void RenderGameOver(sf::RenderWindow& window);
};

