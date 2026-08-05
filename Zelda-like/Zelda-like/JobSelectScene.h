#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "PlayerType.h"

class JobSelectScene : public Scene
{
private:
    PlayerType selectedJob;

    sf::RectangleShape warriorPanel;
    sf::RectangleShape archerPanel;
    sf::RectangleShape magePanel;

    // 직업 대표 이미지
    sf::Sprite* warriorSprite;
    sf::Sprite* archerSprite;

    // 마법사는 몸체와 지팡이를 따로 그림
    sf::Sprite* mageSprite;
    sf::Sprite* mageStaffSprite;

    sf::Text* titleText;

    sf::Text* warriorText;
    sf::Text* archerText;
    sf::Text* mageText;

    sf::Text* guideText;

public:
    JobSelectScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~JobSelectScene();

    void InitializeJobSprites();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};