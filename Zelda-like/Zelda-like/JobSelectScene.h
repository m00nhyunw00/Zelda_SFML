#pragma once

#include "Scene.h"

class SceneManager;

class JobSelectScene : public Scene
{
private:
    SceneManager* sceneManager;

    int selectedJob;

public:
    JobSelectScene(SceneManager* manager);
    ~JobSelectScene();

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
};