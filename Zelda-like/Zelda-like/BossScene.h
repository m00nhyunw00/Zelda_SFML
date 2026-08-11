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
#include "GiantSlime.h"

class BossScene : public InGameScene
{
private:
    Camera camera;

    sf::Sprite* roomSprite = nullptr;

    GiantSlime* boss = nullptr;

private:
    void CreateDungeonRoom();

    void SpawnBoss();

    void SpawnPhase1Monsters();

    void SpawnPhase3Liches();

    void BuildRoomColliders();

    void MovePlayerToSpawn();

    bool CanUseEntrance();

public:
    BossScene(SceneManager* sceneManager, EntityManager* entityManager);

    ~BossScene();

    void HandleEvent(const sf::Event& event, sf::RenderWindow& window) override;

    void Update(float deltaTime, sf::RenderWindow& window) override;

    void Render(sf::RenderWindow& window) override;

    void CheckBossPhase();

    //MonsterColor GetRandomMonsterColor();

    //MonsterType GetRandomMonsterType();
};