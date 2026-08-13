#pragma once

#include "Scene.h"
#include "SFML/Graphics.hpp"

class TitleScene : public Scene
{
private:
    sf::Sprite* backgroundSprite = nullptr;

    sf::Text* titleText;
    sf::Text* startText;

    sf::RectangleShape startButton;

    void CheckSaveDate();

public:
    TitleScene(SceneManager* sceneManager, EntityManager* entityManager);

    ~TitleScene();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;
};