#pragma once

#include "Scene.h"

class SceneManager;

class TitleScene : public Scene
{
private:
    SceneManager* sceneManager;

    sf::Font font;
    sf::Text* titleText;
    sf::Text* startText;

public:
    TitleScene(SceneManager* manager);
    ~TitleScene();

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
};