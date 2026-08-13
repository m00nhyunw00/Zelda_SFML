#pragma once

#include "SFML/Graphics.hpp"
#include "Scene.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "SaveManager.h"
#include "PlayerType.h"
#include "Camera.h"
#include "InGameScene.h"
#include "Collider.h"

class DungeonScene : public InGameScene
{
private:
    Camera camera;

    sf::Sprite* roomSprite = nullptr;

    // 아래쪽 = 탈출
    Collider exitInteractionCollider;

    // 위쪽 = 다음 스테이지 / 10층에서는 보스룸
    Collider nextStageInteractionCollider;

private:
    void CreateDungeonRoom();

    void BuildRoomColliders();

    void SetupEntrances();

    void MovePlayerToSpawn();

    bool CanUseEntrance();

    bool IsPlayerNearExit();

    bool IsPlayerNearNextStage();

public:
    DungeonScene(SceneManager* sceneManager, EntityManager* entityManager);

    ~DungeonScene();

    void HandleEvent(const sf::Event& event,sf::RenderWindow& window) override;

    void Update(float deltaTime,sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    void SpawnRandomMonsters();

    bool IsValidMonsterSpawnPosition(const sf::Vector2f& position);

    MonsterColor GetRandomMonsterColor();

    MonsterType GetRandomMonsterType();
};