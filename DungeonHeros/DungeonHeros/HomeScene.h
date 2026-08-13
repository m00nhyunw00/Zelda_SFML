#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "SaveManager.h"
#include "PlayerType.h"
#include "InGameScene.h"
#include "Camera.h"
#include "Collider.h"

class HomeScene : public InGameScene
{
private:
    Camera camera;

    sf::Sprite* roomSprite = nullptr;

    // 아래쪽 출입구 → Dungeon
    Collider entranceInteractionCollider;

    float healTimer = 0.f;
    float healInterval = 1.f;
    int healAmount;

private:
    void CreateHomeRoom();

    void BuildRoomColliders();

    void SetupEntrance();

    void MovePlayerToSpawn();

    bool IsPlayerNearEntrance();

public:
    HomeScene(SceneManager* sceneManager,EntityManager* entityManager);

    ~HomeScene();

    void HandleEvent(const sf::Event& event,sf::RenderWindow& window) override;

    void Update(float deltaTime,sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;
};