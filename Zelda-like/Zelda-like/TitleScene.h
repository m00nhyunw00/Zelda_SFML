#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"

class TitleScene : public Scene
{
private:
    sf::Text* titleText;                // 게임 제목
    sf::Text* startText;                // 시작 버튼 안에 표시할 텍스트
    sf::RectangleShape startButton;     // 게임 시작 버튼

public:
    TitleScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~TitleScene() override;

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;

    void CheckSaveDate();
};