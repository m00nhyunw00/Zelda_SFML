#include "DungeonScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

using namespace std;

DungeonScene::DungeonScene(SceneManager* sceneManager, EntityManager* entityManager)
    : InGameScene(sceneManager, entityManager),
    camera(
        { Constants::CENTER_X, Constants::CENTER_Y },
        {
            static_cast<float>(Constants::WINDOW_WIDTH),
            static_cast<float>(Constants::WINDOW_HEIGHT)
        }
    )
{
    CreateDungeonRoom();

    BuildRoomColliders();

    SetupEntrances();

    MovePlayerToSpawn();

    SpawnRandomMonsters();
}

DungeonScene::~DungeonScene()
{
    delete roomSprite;
    roomSprite = nullptr;

    wallColliders.clear();
}

void DungeonScene::CreateDungeonRoom()
{
    sf::Texture* dungeonTexture = ResourceManager::GetInstance().GetTexture("Dungeon");

    if (dungeonTexture == nullptr)
    {
        cerr << "[DungeonScene] Dungeon texture not found" << endl;
        return;
    }

    delete roomSprite;

    roomSprite = new sf::Sprite(*dungeonTexture);

    roomSprite->setScale({
        Constants::ROOM_SCALE,
        Constants::ROOM_SCALE
        });

    const sf::FloatRect bounds = roomSprite->getLocalBounds();

    roomSprite->setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    roomSprite->setPosition({
        Constants::CENTER_X,
        Constants::CENTER_Y
        });
}

void DungeonScene::Update(float deltaTime, sf::RenderWindow& window)
{
    Player* player = entityManager->GetPlayer();

    CheckGameOver();
    if (player == nullptr || isGameOver)
    {
        UpdateGameOver(deltaTime, window);
        return;
    }

    camera.Follow(player->GetPosition());

    // 이동 전 위치 저장
    const sf::Vector2f previousPosition = player->GetPosition();

    // Player를 포함한 Entity 업데이트
    entityManager->Update(deltaTime, window);

    UpdateProjectileWallCollisions();

    // 방 외벽과 충돌 검사
    for (const Collider& roomCollider : wallColliders)
    {
        if (player->GetBodyCollider().Collision(roomCollider))
        {
            // 충돌했다면 이동 전 위치로 복구
            //player->SetPosition(previousPosition);
            player->MoveForce(previousPosition);

            break;
        }
    }

    UpdateUI(deltaTime, window);
}

void DungeonScene::HandleEvent(const sf::Event& event,sf::RenderWindow& window)
{
    if (isGameOver)
    {
        HandleGameOverEvent(event,window);

        return;
    }

    InputManager& input = InputManager::GetInstance();

    if (!input.IsFPressed() && !input.IsEnterPressed())
    {
        return;
    }


    // 몬스터를 모두 잡기 전에는
    // 어느 출입구도 이용 불가
    if (!CanUseEntrance())
    {
        cout << "[DungeonScene] Defeat all monsters first" << endl;

        return;
    }


    // =========================================================
    // 아래쪽 계단 → HOME
    // =========================================================

    if (IsPlayerNearExit())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(HOME);

        return;
    }


    // =========================================================
    // 위쪽 계단
    // =========================================================

    if (IsPlayerNearNextStage())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();


        if (sceneManager->GetDungeonStage() >= 10)
        {
            sceneManager->RequestSceneChange(BOSS);
        }
        else
        {
            sceneManager->NextDungeonStage();

            sceneManager->RequestSceneChange(DUNGEON);
        }

        return;
    }
}

void DungeonScene::Render(sf::RenderWindow& window)
{
    window.setView(camera.GetView());

    if (roomSprite != nullptr)
    {
        window.draw(*roomSprite);
    }

    entityManager->Render(window);

    // Debug Collider ----------------------------

    for (const Collider& collider : wallColliders)
    {
        collider.Draw(window);
    }

    exitInteractionCollider.Draw(window);

    nextStageInteractionCollider.Draw(window);

    window.setView(window.getDefaultView());

    if (isGameOver)
    {
        RenderGameOver(window);
    }
    else
    {
        RenderUI(window);
    }
}

void DungeonScene::BuildRoomColliders()
{
    wallColliders.clear();

    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const float left = bounds.position.x;

    const float top = bounds.position.y;

    const float width = bounds.size.x;

    const float height = bounds.size.y;


    // 실제 플레이 영역 ----------------------------

    const float playableLeft = left + width * 0.07f;

    const float playableRight = left + width * 0.93f;

    const float playableTop = top + height * 0.16f;

    const float playableBottom = top + height * 0.865f;


    const float playableWidth = playableRight - playableLeft;

    const float playableHeight = playableBottom - playableTop;


    const float wallThickness = 8.f * Constants::ROOM_SCALE;


    // 위쪽 벽
    Collider topCollider({playableWidth,wallThickness});

    topCollider.UpdatePosition({playableLeft + playableWidth / 2.f,playableTop});

    wallColliders.push_back(topCollider);


    // 아래쪽 벽
    Collider bottomCollider({playableWidth,wallThickness});

    bottomCollider.UpdatePosition({playableLeft + playableWidth / 2.f,playableBottom});

    wallColliders.push_back(bottomCollider);


    // 왼쪽 벽
    Collider leftCollider({wallThickness,playableHeight});

    leftCollider.UpdatePosition({playableLeft,playableTop + playableHeight / 2.f});

    wallColliders.push_back(leftCollider);


    // 오른쪽 벽
    Collider rightCollider({wallThickness,playableHeight});

    rightCollider.UpdatePosition({playableRight,playableTop + playableHeight / 2.f});

    wallColliders.push_back(rightCollider);
}

void DungeonScene::MovePlayerToSpawn()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr || roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const sf::Vector2f spawnPosition =
    {
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y * 0.80f
    };

    player->MoveForce(spawnPosition);
}

void DungeonScene::SetupEntrances()
{
    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const float centerX = bounds.position.x + bounds.size.x / 2.f;


    // =========================================================
    // 아래쪽 탈출구
    // =========================================================

    exitInteractionCollider.SetSize({
        80.f * Constants::ROOM_SCALE,
        70.f * Constants::ROOM_SCALE
        });

    exitInteractionCollider.UpdatePosition({
        centerX,

        bounds.position.y +
        bounds.size.y * 0.875f
        });


    // =========================================================
    // 위쪽 다음 Stage 입구
    // =========================================================

    nextStageInteractionCollider.SetSize({
        80.f * Constants::ROOM_SCALE,
        70.f * Constants::ROOM_SCALE
        });

    nextStageInteractionCollider.UpdatePosition({
        centerX,

        bounds.position.y +
        bounds.size.y * 0.155f
        });
}

bool DungeonScene::CanUseEntrance()
{
    return entityManager->AreAllMonstersDead();
}

bool DungeonScene::IsPlayerNearExit()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return false;
    }

    return player->GetBodyCollider().Collision(exitInteractionCollider);
}

bool DungeonScene::IsPlayerNearNextStage()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return false;
    }

    return player->GetBodyCollider().Collision(nextStageInteractionCollider);
}

void DungeonScene::SpawnRandomMonsters()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr ||
        roomSprite == nullptr)
    {
        return;
    }

    const int minSpawnCount = Constants::MIN_SPAWN_COUNT;

    const int maxSpawnCount = Constants::MAX_SPAWN_COUNT;

    const int spawnCount = minSpawnCount + rand() % (maxSpawnCount - minSpawnCount + 1);

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    // 몬스터는 중앙의 넓은 바닥 영역에서만 생성
    const float minX =
        bounds.position.x +
        bounds.size.x * 0.15f;

    const float maxX =
        bounds.position.x +
        bounds.size.x * 0.85f;

    const float minY =
        bounds.position.y +
        bounds.size.y * 0.27f;

    const float maxY =
        bounds.position.y +
        bounds.size.y * 0.72f;


    const float playerSafeRadius = 130.f;

    for (int i = 0; i < spawnCount; i++)
    {
        sf::Vector2f spawnPosition;

        bool validPosition = false;

        for (int attempt = 0; attempt < 100; attempt++)
        {
            const float randomX =
                minX +
                static_cast<float>(rand()) /
                static_cast<float>(RAND_MAX) *
                (maxX - minX);

            const float randomY =
                minY +
                static_cast<float>(rand()) /
                static_cast<float>(RAND_MAX) *
                (maxY - minY);

            spawnPosition =
            {
                randomX,
                randomY
            };

            const sf::Vector2f toPlayer = spawnPosition - player->GetPosition();

            const float distance =
                std::sqrt(
                    toPlayer.x * toPlayer.x +
                    toPlayer.y * toPlayer.y
                );

            if (distance < playerSafeRadius)
            {
                continue;
            }

            validPosition = true;

            break;
        }


        if (!validPosition)
        {
            continue;
        }

        const MonsterType monsterType = GetRandomMonsterType();

        const MonsterColor color = GetRandomMonsterColor();

        entityManager->SpawnMonster(
            monsterType,
            color,
            spawnPosition
        );
    }
}

MonsterColor DungeonScene::GetRandomMonsterColor()
{
    const int random = rand() % 12;

    switch (random)
    {
    case 0:
        return MonsterColor::BLACK;

    case 1:
        return MonsterColor::RED;

    case 2:
        return MonsterColor::BLUE;

    case 3:
        return MonsterColor::PURPLE;
    }

    return MonsterColor::NONE_COLOR;
}

MonsterType DungeonScene::GetRandomMonsterType()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return MonsterType::SLIME;
    }

    const MonsterSpawnData* spawnData = DataManager::GetInstance().GetMonsterSpawnData(player->GetLevel());

    if (spawnData == nullptr)
    {
        return MonsterType::SLIME;
    }

    const int random = rand() % 100;

    int accumulatedRate = 0;

    accumulatedRate += spawnData->slimeRate;

    if (random < accumulatedRate)
    {
        return MonsterType::SLIME;
    }

    accumulatedRate += spawnData->cactoRate;

    if (random < accumulatedRate)
    {
        return MonsterType::CACTO;
    }

    accumulatedRate += spawnData->skeletonRate;

    if (random < accumulatedRate)
    {
        return MonsterType::SKELETON;
    }

    return MonsterType::LICH;
}