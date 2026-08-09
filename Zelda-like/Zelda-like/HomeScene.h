#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "InGameScene.h"

class HomeScene : public InGameScene
{
private:
    std::vector<sf::Sprite> floorSprites;
    std::vector<sf::Sprite> upperWallSprites;
    std::vector<sf::Sprite> lowerWallSprites;

    std::vector<Collider> roomColliders;

private:
    void AddFloorArea(
        const sf::Sprite& floorTileTemplate,
        const sf::Vector2f& centerPosition,
        int columns,
        int rows
    );

    void AddRoomWalls(
        const sf::Sprite& floorTileTemplate,
        const sf::Sprite& upperWallTemplate,
        const sf::Sprite& lowerWallTemplate,
        const sf::Vector2f& centerPosition,
        int columns,
        int rows,
        float colliderThickness
    );

public:
    HomeScene(SceneManager* sceneManager, EntityManager* entityManager);
    ~HomeScene();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void Update(float deltaTime, sf::RenderWindow& window) override;
    void Render(sf::RenderWindow& window) override;
};