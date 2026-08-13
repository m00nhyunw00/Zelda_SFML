#pragma once

#include "Scene.h"
#include "UI.h"
#include "Button.h"
#include "GaugeBar.h"
#include "SkillCooldownUI.h"
#include "UltimateUI.h"

class InGameScene : public Scene
{
protected:
    std::vector<Collider> wallColliders;
    std::vector<Collider> objectColliders;

    // ----- 게임 오버 시 UI ------------------------

    bool isGameOver;

    sf::RectangleShape* gameOverOverlay = nullptr;
    sf::Text* gameOverText = nullptr;

    Button* restartButton = nullptr;
    Button* exitButton = nullptr;

    GaugeBar* playerHpBar = nullptr;
    GaugeBar* playerUltimateBar = nullptr;

    SkillCooldownUI* skillCooldownUI = nullptr;
    UltimateUI* ultimateUI = nullptr;

    GaugeBar* playerExpBar = nullptr;
    sf::Text* levelText = nullptr;

    // ----- 엔딩 UI -------------------------------

    bool isEnding = false;

    sf::Sprite* endingSprite = nullptr;

    float endingAlpha = 0.f;

    float endingFadeDuration = 2.f;     // 완전히 나타나기까지 약 2초


public:
    InGameScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~InGameScene();

    bool CheckGameOver();
    bool CheckEnding();
    void HandleGameOverEvent(const sf::Event& event, sf::RenderWindow& window);
    void HandleEndingEvent(const sf::Event& event, sf::RenderWindow& window);
    void StartEnding();
    void UpdateEnding(float deltaTime, sf::RenderWindow& window);
    void RenderEnding(sf::RenderWindow& window);
    void UpdateUI(float deltaTime,sf::RenderWindow& window);    
    void UpdatePlayerHpBar(float deltaTime, sf::RenderWindow& window);
    void UpdateSkillCooldownUI(float deltaTime, sf::RenderWindow& window);
    void UpdateUltimateUI(float deltaTime, sf::RenderWindow& window);
    void UpdateUltimateGauge(float deltaTime, sf::RenderWindow& window);
    void UpdateGameOver(float deltaTime, sf::RenderWindow& window);
    void UpdateExpBar(float deltaTime, sf::RenderWindow& window);
    void UpdateCollisions();
    void UpdateLevelText();
    void RenderUI(sf::RenderWindow& window);
    void RenderGameOver(sf::RenderWindow& window);
};

