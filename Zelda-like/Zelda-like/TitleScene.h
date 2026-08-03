#pragma once

#include "Scene.h"

class SceneManager;

class TitleScene : public Scene
{
private:
    SceneManager* sceneManager;

    sf::Font font;

    sf::Text* titleText;                // 게임 제목

    sf::Text* startText;                // 시작 버튼 안에 표시할 텍스트
    sf::RectangleShape startButton;     // 게임 시작 버튼

public:
    TitleScene(SceneManager* manager);
    ~TitleScene();

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
};