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

    sf::Text* titleText;

    sf::Text* warriorText;
    sf::Text* archerText;
    sf::Text* mageText;

    sf::Text* guideText;

public:
    JobSelectScene(SceneManager* manager, Player* player);
    ~JobSelectScene();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};