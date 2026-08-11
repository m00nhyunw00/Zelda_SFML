#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "SaveManager.h"
#include "PlayerType.h"
#include "InGameScene.h"
#include "Camera.h"

class HomeScene : public InGameScene
{
private:
    std::vector<sf::Sprite> floorSprites;
    std::vector<sf::Sprite> upperWallSprites;
    std::vector<sf::Sprite> lowerWallSprites;

    sf::Sprite* entranceSprite = nullptr;

    Collider entranceInteractionCollider;

    Camera camera;

private:
    bool IsPlayerNearEntrance();

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
        const sf::Sprite& leftWallTemplate,
        const sf::Sprite& rightWallTemplate,
        const sf::Sprite& upperLeftCornerTemplate,
        const sf::Sprite& upperRightCornerTemplate,
        const sf::Sprite& lowerLeftCornerTemplate,
        const sf::Sprite& lowerRightCornerTemplate,
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