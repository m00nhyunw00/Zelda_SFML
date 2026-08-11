#include "DungeonScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

DungeonScene::DungeonScene(SceneManager* sceneManager, EntityManager* entitymanager) 
    : InGameScene(sceneManager, entitymanager),
    camera({ Constants::CENTER_X, Constants::CENTER_Y },
          {
              (float)Constants::WINDOW_WIDTH,
              (float)Constants::WINDOW_HEIGHT
          }
    )
{
    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font != nullptr)
    {
        /*titleText = new sf::Text(*font);
        titleText->setString("Select Your Job");
        titleText->setCharacterSize(42);

        sf::FloatRect bounds = titleText->getLocalBounds();
        titleText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        titleText->setPosition({ 640.f, 70.f });*/

    }

    //warriorPanel.setSize({ 220.f, 260.f });
    //warriorPanel.setPosition({ 170.f, 180.f });
    //warriorPanel.setFillColor(sf::Color(80, 80, 80));
    //warriorPanel.setOutlineThickness(3.f);

    ResourceManager& resourceManager = ResourceManager::GetInstance();

    sf::Texture* houseTexture = resourceManager.GetTexture("House");
    sf::Texture* indoorTexture = resourceManager.GetTexture("Indoor");

    if (indoorTexture != nullptr)
    {

        sf::Sprite floorTile =
            SpriteUtil::CreateSprite(
                *indoorTexture,
                sf::IntRect(
                    { 240, 48 },
                    { 16, 16 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite upperWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 56, 0 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite lowerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 55, 96 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        SpriteUtil::SetSpriteOriginToCenter(&floorTile);
        SpriteUtil::SetSpriteOriginToCenter(&upperWallTile);
        SpriteUtil::SetSpriteOriginToCenter(&lowerWallTile);

        const sf::Vector2f roomPosition = { Constants::CENTER_X, Constants::CENTER_Y + 22 };

        const int roomColumns = 13;
        const int roomRows = 9;
        const int colliderTickness = 2;

        AddFloorArea(
            floorTile,
            roomPosition,
            roomColumns,
            roomRows
        );

        AddRoomWalls(
            floorTile,
            upperWallTile,
            lowerWallTile,
            roomPosition,
            roomColumns,
            roomRows,
            colliderTickness
        );
    }

    SpawnRandomMonsters();
}

DungeonScene::~DungeonScene()
{
    floorSprites.clear();
    upperWallSprites.clear();
    lowerWallSprites.clear();
    wallColliders.clear();
}

void DungeonScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    if (isGameOver)
    {
        HandleGameOverEvent(event, window);

        return;
    }

    InputManager& input = InputManager::GetInstance();

    // 숫자 키
    if (input.IsNum1Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());
        entityManager->ClearMonsters();

        sceneManager->RequestSceneChange(HOME);
        return;
    }

    if (input.IsNum2Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());
        entityManager->ClearMonsters();

        sceneManager->RequestSceneChange(DUNGEON);
        return;
    }

    // 마우스
    if (input.IsLeftMouseClicked())
    {
        sf::Vector2f mousePosition = input.GetMouseClickWorldPosition(window);

        //if (warriorPanel.getGlobalBounds().contains(mousePosition))
        //{
        //}

    }
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

void DungeonScene::Render(sf::RenderWindow& window)
{
    window.setView(camera.GetView());

    for (const sf::Sprite& floorSprite : floorSprites)
    {
        window.draw(floorSprite);
    }

    for (const sf::Sprite& wallSprite : upperWallSprites)
    {
        window.draw(wallSprite);
    }

    entityManager->Render(window);

    for (const sf::Sprite& wallSprite : lowerWallSprites)
    {
        window.draw(wallSprite);
    }

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

void DungeonScene::SpawnRandomMonsters()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    // 스폰 마릿수 -----------------------------------------

    const int minSpawnCount = Constants::MIN_SPAWN_COUNT;

    const int maxSpawnCount = Constants::MAX_SPAWN_COUNT;

    const int spawnCount = minSpawnCount + rand() % (maxSpawnCount - minSpawnCount + 1);

    // 방 내부 영역 -----------------------------------------

    const sf::Vector2f roomPosition =
    {
        Constants::CENTER_X,
        Constants::CENTER_Y + 22.f
    };

    const int roomColumns = 13;
    const int roomRows = 9;

    const float tileSize = 16.f * 4.f;

    const float roomWidth = roomColumns * tileSize;

    const float roomHeight = roomRows * tileSize;

    // 벽에서 여유 공간
    const float margin = 50.f;

    const float minX = roomPosition.x - roomWidth / 2.f + margin;

    const float maxX = roomPosition.x + roomWidth / 2.f - margin;

    const float minY = roomPosition.y - roomHeight / 2.f + margin;

    const float maxY = roomPosition.y + roomHeight / 2.f - margin;

    // Player 주변 생성 금지
    const float playerSafeRadius = 180.f;


    // 몬스터 생성 -----------------------------------------

    for (int i = 0; i < spawnCount; i++)
    {
        sf::Vector2f spawnPosition;

        bool validPosition = false;

        // 무한 루프 방지
        for (int attempt = 0; attempt < 100; attempt++)
        {
            const float randomX = minX + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX);

            const float randomY = minY + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxY - minY);

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

            if (distance >= playerSafeRadius)
            {
                validPosition = true;
                break;
            }
        }

        if (!validPosition)
        {
            continue;
        }

        // 플레이어 레벨에 따른 MonsterType
        const MonsterType monsterType = GetRandomMonsterType();

        // 랜덤 색상
        const MonsterColor color = GetRandomMonsterColor();

        // 실제 생성
        entityManager->SpawnMonster(monsterType, color, spawnPosition);
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

void DungeonScene::AddFloorArea(
    const sf::Sprite& floorTileTemplate,
    const sf::Vector2f& centerPosition,
    int columns,
    int rows
)
{
    if (columns <= 0 || rows <= 0)
    {
        return;
    }

    const sf::FloatRect bounds = floorTileTemplate.getGlobalBounds();

    const float tileWidth = bounds.size.x;
    const float tileHeight = bounds.size.y;

    const float roomWidth = columns * tileWidth;

    const float roomHeight = rows * tileHeight;

    const float startX = centerPosition.x - roomWidth / 2.f + tileWidth / 2.f;

    const float startY = centerPosition.y - roomHeight / 2.f + tileHeight / 2.f;

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            sf::Sprite floorSprite = floorTileTemplate;

            floorSprite.setPosition({
                startX + column * tileWidth,
                startY + row * tileHeight
                });

            floorSprites.push_back(floorSprite);
        }
    }
}

void DungeonScene::AddRoomWalls(
    const sf::Sprite& floorTileTemplate,
    const sf::Sprite& upperWallTemplate,
    const sf::Sprite& lowerWallTemplate,
    const sf::Vector2f& centerPosition,
    int columns,
    int rows,
    float colliderThickness
)
{
    if (columns <= 0 ||
        rows <= 0 ||
        colliderThickness <= 0.f)
    {
        return;
    }

    const sf::FloatRect tileBounds = floorTileTemplate.getGlobalBounds();

    const float tileWidth = tileBounds.size.x;

    const float tileHeight = tileBounds.size.y;

    const float roomWidth = columns * tileWidth;

    const float roomHeight = rows * tileHeight;

    const float roomLeft = centerPosition.x - roomWidth / 2.f;

    const float roomRight = centerPosition.x + roomWidth / 2.f;

    const float roomTop = centerPosition.y - roomHeight / 2.f;

    const float roomBottom = centerPosition.y + roomHeight / 2.f;

    const sf::FloatRect upperWallBounds = upperWallTemplate.getGlobalBounds();
    const sf::FloatRect lowerWallBounds = lowerWallTemplate.getGlobalBounds();

    // 위쪽 벽 Sprite를 가로로 배치
    for (int column = 0; column < columns; column++)
    {
        sf::Sprite wallSprite = upperWallTemplate;

        wallSprite.setPosition({
            roomLeft + tileWidth / 2.f + column * tileWidth,
            roomTop - upperWallBounds.size.y / 2.f + 20.f
            });

        upperWallSprites.push_back(wallSprite);
    }

    // 아래쪽 벽 Sprite를 가로로 배치
    for (int column = 0; column < columns; column++)
    {
        sf::Sprite wallSprite = lowerWallTemplate;

        wallSprite.setPosition({
            roomLeft
                + tileWidth / 2.f
                + column * tileWidth,

            roomBottom
            });

        lowerWallSprites.push_back(wallSprite);
    }

    // 위쪽 Collider
    Collider topCollider({
        roomWidth,
        colliderThickness
        });

    topCollider.UpdatePosition({
        centerPosition.x,
        roomTop - colliderThickness / 2.f
        });

    wallColliders.push_back(topCollider);

    // 아래쪽 Collider
    Collider bottomCollider({
        roomWidth,
        colliderThickness
        });

    bottomCollider.UpdatePosition({
        centerPosition.x,
        roomBottom + colliderThickness / 2.f
        });

    wallColliders.push_back(bottomCollider);

    // 왼쪽 Collider
    Collider leftCollider({
        colliderThickness,
        roomHeight
        });

    leftCollider.UpdatePosition({
        roomLeft - colliderThickness / 2.f,
        centerPosition.y
        });

    wallColliders.push_back(leftCollider);

    // 오른쪽 Collider
    Collider rightCollider({
        colliderThickness,
        roomHeight
        });

    rightCollider.UpdatePosition({
        roomRight + colliderThickness / 2.f,
        centerPosition.y
        });

    wallColliders.push_back(rightCollider);
}
