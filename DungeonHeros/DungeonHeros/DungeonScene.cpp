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

    // Player를 포함한 Entity 업데이트
    entityManager->Update(deltaTime, window);

    UpdateCollisions();

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

    if (!input.IsEnterPressed())
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

        if (sceneManager->GetDungeonStage() >= Constants::TOTAL_STAGE)
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

    //for (const Collider& collider : wallColliders)
    //{
    //    collider.Draw(window);
    //}

    //exitInteractionCollider.Draw(window);

    //nextStageInteractionCollider.Draw(window);

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
    objectColliders.clear();

    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds =
        roomSprite->getGlobalBounds();

    const float left = bounds.position.x;
    const float top = bounds.position.y;
    const float width = bounds.size.x;
    const float height = bounds.size.y;


    // =========================================================
    // 실제 플레이 영역
    // =========================================================

    const float playableLeft =
        left + width * 0.06f;

    const float playableRight =
        left + width * 0.94f;

    const float playableTop =
        top + height * 0.13f;

    const float playableBottom =
        top + height * 0.850f;

    const float playableWidth =
        playableRight - playableLeft;

    const float playableHeight =
        playableBottom - playableTop;

    const float wallThickness =
        8.f * Constants::ROOM_SCALE;


    // =========================================================
    // 벽 Collider
    //
    // 캐릭터 X
    // 몬스터 X
    // 투사체 X
    // =========================================================

    // 위쪽 벽
    Collider topCollider({
        playableWidth,
        wallThickness
        });

    topCollider.UpdatePosition({
        playableLeft + playableWidth / 2.f,
        playableTop
        });

    wallColliders.push_back(topCollider);


    // 아래쪽 벽
    Collider bottomCollider({
        playableWidth,
        wallThickness
        });

    bottomCollider.UpdatePosition({
        playableLeft + playableWidth / 2.f,
        playableBottom
        });

    wallColliders.push_back(bottomCollider);


    // 왼쪽 벽
    Collider leftCollider({
        wallThickness,
        playableHeight
        });

    leftCollider.UpdatePosition({
        playableLeft,
        playableTop + playableHeight / 2.f
        });

    wallColliders.push_back(leftCollider);


    // 오른쪽 벽
    Collider rightCollider({
        wallThickness,
        playableHeight
        });

    rightCollider.UpdatePosition({
        playableRight,
        playableTop + playableHeight / 2.f
        });

    wallColliders.push_back(rightCollider);


    // =========================================================
    // Object Collider 생성용
    //
    // 캐릭터 X
    // 몬스터 X
    // 투사체 O
    // =========================================================

    auto AddObjectCollider =
        [&](float x,
            float y,
            float sizeX,
            float sizeY)
        {
            Collider collider({
                width * sizeX,
                height * sizeY
                });

            collider.UpdatePosition({
                left + width * x,
                top + height * y
                });

            objectColliders.push_back(collider);
        };


    // =========================================================
    // 왼쪽 위 오브젝트
    // =========================================================

    // 왼쪽 위 보라색 횃불/수정 받침대
    AddObjectCollider(
        0.103f,
        0.135f,
        0.045f,
        0.060f
    );

    // 왼쪽 위 세로 상자 더미
    AddObjectCollider(
        0.100f,
        0.210f,
        0.052f,
        0.110f
    );

    // 왼쪽 위 해골 더미
    AddObjectCollider(
        0.151f,
        0.170f,
        0.060f,
        0.030f
    );

    // 왼쪽 위 항아리
    AddObjectCollider(
        0.155f,
        0.218f,
        0.045f,
        0.060f
    );


    // =========================================================
    // 오른쪽 위 오브젝트
    // =========================================================

    // 오른쪽 위 보라색 횃불/수정 받침대
    AddObjectCollider(
        0.895f,
        0.135f,
        0.045f,
        0.060f
    );

    // 오른쪽 위 세로 상자 더미
    AddObjectCollider(
        0.900f,
        0.210f,
        0.052f,
        0.110f
    );

    // 오른쪽 위 해골 더미
    AddObjectCollider(
        0.835f,
        0.170f,
        0.060f,
        0.030f
    );

    // 오른쪽 위 항아리
    AddObjectCollider(
        0.840f,
        0.218f,
        0.045f,
        0.060f
    );


    // =========================================================
    // 좌우 중간 횃불
    // =========================================================

    // 왼쪽 중간 횃불
    AddObjectCollider(
        0.100f,
        0.470f,
        0.040f,
        0.065f
    );

    // 오른쪽 중간 횃불
    AddObjectCollider(
        0.905f,
        0.470f,
        0.040f,
        0.065f
    );


    // =========================================================
    // 가운데 위쪽 오브젝트
    // =========================================================

    // 왼쪽 상자
    AddObjectCollider(
        0.338f,
        0.397f,
        0.052f,
        0.070f
    );

    // 왼쪽 항아리
    AddObjectCollider(
        0.415f,
        0.420f,
        0.050f,
        0.060f
    );

    // 오른쪽 항아리
    AddObjectCollider(
        0.575f,
        0.420f,
        0.050f,
        0.060f
    );

    // 오른쪽 상자
    AddObjectCollider(
        0.647f,
        0.397f,
        0.052f,
        0.070f
    );


    // =========================================================
    // 가운데 아래쪽 오브젝트
    // =========================================================

    // 왼쪽 상자
    AddObjectCollider(
        0.397f,
        0.615f,
        0.052f,
        0.070f
    );

    // 중앙 항아리
    AddObjectCollider(
        0.495f,
        0.620f,
        0.050f,
        0.030f
    );

    // 오른쪽 상자
    AddObjectCollider(
        0.593f,
        0.615f,
        0.052f,
        0.070f
    );


    // =========================================================
    // 왼쪽 아래 오브젝트
    // =========================================================

    // 왼쪽 아래 보라색 횃불
    AddObjectCollider(
        0.103f,
        0.815f,
        0.045f,
        0.065f
    );

    // 왼쪽 아래 항아리
    AddObjectCollider(
        0.155f,
        0.818f,
        0.045f,
        0.060f
    );

    // 왼쪽 아래 해골 더미
    AddObjectCollider(
        0.100f,
        0.855f,
        0.060f,
        0.030f
    );


    // =========================================================
    // 오른쪽 아래 오브젝트
    // =========================================================

    // 오른쪽 아래 항아리
    AddObjectCollider(
        0.838f,
        0.818f,
        0.045f,
        0.060f
    );

    // 오른쪽 아래 보라색 횃불
    AddObjectCollider(
        0.895f,
        0.815f,
        0.045f,
        0.065f
    );

    // 오른쪽 아래 해골 더미
    AddObjectCollider(
        0.895f,
        0.855f,
        0.060f,
        0.030f
    );
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
        100.f * Constants::ROOM_SCALE
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

bool DungeonScene::IsValidMonsterSpawnPosition(const sf::Vector2f& position)
{
    if (roomSprite == nullptr)
    {
        return false;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const float playableLeft =
        bounds.position.x +
        bounds.size.x * 0.07f;

    const float playableRight =
        bounds.position.x +
        bounds.size.x * 0.93f;

    const float playableTop =
        bounds.position.y +
        bounds.size.y * 0.16f;

    const float playableBottom =
        bounds.position.y +
        bounds.size.y * 0.865f;

    const float margin = Constants::DEFAULT_COLLIDER_SIZE;

    // 실제 플레이 영역 밖이면 스폰 불가
    if (position.x < playableLeft + margin ||
        position.x > playableRight - margin ||
        position.y < playableTop + margin ||
        position.y > playableBottom - margin)
    {
        return false;
    }

    // 몬스터 생성 공간 확인용 Collider
    Collider spawnCollider({
        Constants::DEFAULT_COLLIDER_SIZE * 1.3f,
        Constants::DEFAULT_COLLIDER_SIZE * 1.3f
        });

    spawnCollider.UpdatePosition(position);

    // 벽과 겹치면 스폰 불가
    for (const Collider& collider : wallColliders)
    {
        if (spawnCollider.Collision(collider))
        {
            return false;
        }
    }

    // 오브젝트와 겹치면 스폰 불가
    for (const Collider& collider : objectColliders)
    {
        if (spawnCollider.Collision(collider))
        {
            return false;
        }
    }

    return true;
}

void DungeonScene::SpawnRandomMonsters()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr || roomSprite == nullptr)
    {
        return;
    }

    const int minSpawnCount = Constants::MIN_SPAWN_COUNT;

    const int maxSpawnCount =Constants::MAX_SPAWN_COUNT;

    const int spawnCount = minSpawnCount + rand() % (maxSpawnCount - minSpawnCount + 1);

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();


    // 몬스터 생성 가능 범위
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


    // 플레이어 주변에는 몬스터 생성 금지
    const float playerSafeRadius = 130.f;


    int spawnedCount = 0;

    int totalAttempt = 0;

    // 무한 루프 방지를 위한 전체 최대 시도 횟수
    const int maxTotalAttempt = spawnCount * 100;


    while (spawnedCount < spawnCount && totalAttempt < maxTotalAttempt)
    {
        totalAttempt++;


        // ---------------- 랜덤 위치 생성 ----------------

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

        const sf::Vector2f spawnPosition =
        {
            randomX,
            randomY
        };


        // ---------------- 플레이어와 거리 검사 ----------------

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


        // ---------------- Collider 검사 ----------------

        if (!IsValidMonsterSpawnPosition(spawnPosition))
        {
            // 오브젝트 또는 벽 위라면
            // 이 위치는 버리고 다시 랜덤 생성
            continue;
        }


        // ---------------- 몬스터 생성 ----------------

        const MonsterType monsterType = GetRandomMonsterType();

        const MonsterColor color = GetRandomMonsterColor();

        Monster* monster =
            entityManager->SpawnMonster(
                monsterType,
                color,
                spawnPosition
            );

        if (monster == nullptr)
        {
            continue;
        }


        spawnedCount++;
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