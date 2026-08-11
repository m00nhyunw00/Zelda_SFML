#include "HomeScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

using namespace std;

HomeScene::HomeScene(SceneManager* sceneManager, EntityManager* entitymanager) 
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

        sf::Sprite upperLeftCornerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 16, 32 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite upperRightCornerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 112, 32 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite lowerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 54, 96 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite lowerLeftCornerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 48, 96 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite lowerRightCornerWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 80, 96 },
                    { 16, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite LeftWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 16, 37 },
                    { 6, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite RightWallTile =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 122, 37 },
                    { 6, 24 }
                ),
                { 4.f, 4.f }
            );

        sf::Sprite Entrance =
            SpriteUtil::CreateSprite(
                *houseTexture,
                sf::IntRect(
                    { 128, 80 },
                    { 16, 16 }
                ),
                { 4.f, 4.f }
            );

        SpriteUtil::SetSpriteOriginToCenter(&floorTile);

        SpriteUtil::SetSpriteOriginToCenter(&upperWallTile);
        SpriteUtil::SetSpriteOriginToCenter(&lowerWallTile);

        SpriteUtil::SetSpriteOriginToCenter(&LeftWallTile);
        SpriteUtil::SetSpriteOriginToCenter(&RightWallTile);

        SpriteUtil::SetSpriteOriginToCenter(&upperLeftCornerWallTile);
        SpriteUtil::SetSpriteOriginToCenter(&upperRightCornerWallTile);

        SpriteUtil::SetSpriteOriginToCenter(&lowerLeftCornerWallTile);
        SpriteUtil::SetSpriteOriginToCenter(&lowerRightCornerWallTile);

        SpriteUtil::SetSpriteOriginToCenter(&Entrance);

        const sf::Vector2f roomPosition = { Constants::CENTER_X, Constants::CENTER_Y + 22 };

        const int roomColumns = 14;
        const int roomRows = 8;
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
            LeftWallTile,
            RightWallTile,
            upperLeftCornerWallTile,
            upperRightCornerWallTile,
            lowerLeftCornerWallTile,
            lowerRightCornerWallTile,
            roomPosition,
            roomColumns,
            roomRows,
            colliderTickness
        );

        const sf::FloatRect floorBounds =
            floorTile.getGlobalBounds();

        const sf::FloatRect entranceBounds =
            Entrance.getGlobalBounds();

        const float roomHeight =
            roomRows * floorBounds.size.y;

        const float roomTop =
            roomPosition.y - roomHeight / 2.f;

        const sf::Vector2f entrancePosition =
        {
            roomPosition.x,
            roomTop - entranceBounds.size.y / 2.f
        };

        entranceSprite =
            new sf::Sprite(Entrance);

        entranceSprite->setPosition(
            entrancePosition
        );

        // Entrance 상호작용 범위
        entranceInteractionCollider.SetSize({
            entranceBounds.size.x + 80.f,
            entranceBounds.size.y + 100.f
            });

        entranceInteractionCollider.UpdatePosition({
            entrancePosition.x,
            roomTop + 30.f
            });
    }
}

HomeScene::~HomeScene()
{
    floorSprites.clear();
    upperWallSprites.clear();
    lowerWallSprites.clear();
    wallColliders.clear();

    delete entranceSprite;
    entranceSprite = nullptr;
}

void HomeScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // 숫자 키 -------------------------------------------

    // 테스트용 입력
    if (input.IsNum1Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());
        entityManager->ClearMonsters();
        sceneManager->RequestSceneChange(TITLE);
        return;
    }

    // 테스트용 입력
    if (input.IsNum2Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->RequestSceneChange(DUNGEON);

        return;
    }

    if (input.IsFPressed() && IsPlayerNearEntrance())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->RequestSceneChange(DUNGEON);

        return;
    }

    if (input.IsEnterPressed() && IsPlayerNearEntrance())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
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

void HomeScene::Update(float deltaTime, sf::RenderWindow& window)
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

void HomeScene::Render(sf::RenderWindow& window)
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

    if (entranceSprite != nullptr)
    {
        window.draw(*entranceSprite);
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

void HomeScene::AddFloorArea(
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

void HomeScene::AddRoomWalls(
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
)
{
    if (columns <= 0 ||
        rows <= 0 ||
        colliderThickness <= 0.f)
    {
        return;
    }

    // Floor / Room Size -------------------------------------

    const sf::FloatRect floorBounds = floorTileTemplate.getGlobalBounds();

    const float tileWidth = floorBounds.size.x;

    const float tileHeight = floorBounds.size.y;

    const float roomWidth = tileWidth * columns;

    const float roomHeight = tileHeight * rows;

    const float roomLeft = centerPosition.x - roomWidth / 2.f;

    const float roomRight = centerPosition.x + roomWidth / 2.f;

    const float roomTop = centerPosition.y - roomHeight / 2.f;

    const float roomBottom = centerPosition.y + roomHeight / 2.f;


    // Sprite Sizes -------------------------------------

    const sf::FloatRect upperBounds = upperWallTemplate.getGlobalBounds();

    const sf::FloatRect lowerBounds = lowerWallTemplate.getGlobalBounds();

    const sf::FloatRect leftBounds = leftWallTemplate.getGlobalBounds();

    const sf::FloatRect rightBounds = rightWallTemplate.getGlobalBounds();

    const sf::FloatRect upperLeftCornerBounds = upperLeftCornerTemplate.getGlobalBounds();

    const sf::FloatRect upperRightCornerBounds = upperRightCornerTemplate.getGlobalBounds();

    const sf::FloatRect lowerLeftCornerBounds = lowerLeftCornerTemplate.getGlobalBounds();

    const sf::FloatRect lowerRightCornerBounds = lowerRightCornerTemplate.getGlobalBounds();


    // Top / Bottom Y -------------------------------------

    const float upperWallY = roomTop - upperBounds.size.y / 2.f;

    const float lowerWallY = roomBottom + lowerBounds.size.y / 2.f;


    // Upper Corners -------------------------------------

    sf::Sprite upperLeftCorner = upperLeftCornerTemplate;

    upperLeftCorner.setPosition({ roomLeft + upperLeftCornerBounds.size.x / 2.f,upperWallY });

    upperWallSprites.push_back(upperLeftCorner);


    sf::Sprite upperRightCorner = upperRightCornerTemplate;

    upperRightCorner.setPosition({ roomRight - upperRightCornerBounds.size.x / 2.f,upperWallY });

    upperWallSprites.push_back(upperRightCorner);


    // Upper Wall -------------------------------------

    for (int column = 1; column < columns - 1; column++)
    {
        sf::Sprite wallSprite = upperWallTemplate;

        const float x = roomLeft + tileWidth / 2.f + column * tileWidth;

        wallSprite.setPosition({x, upperWallY});

        upperWallSprites.push_back(wallSprite);
    }

    // Lower Corners -------------------------------------

    sf::Sprite lowerLeftCorner = lowerLeftCornerTemplate;

    lowerLeftCorner.setPosition({ roomLeft + lowerLeftCornerBounds.size.x / 2.f,lowerWallY });

    lowerWallSprites.push_back(lowerLeftCorner);


    sf::Sprite lowerRightCorner = lowerRightCornerTemplate;

    lowerRightCorner.setPosition({ roomRight - lowerRightCornerBounds.size.x / 2.f,lowerWallY });

    lowerWallSprites.push_back(lowerRightCorner);


    // Lower Wall -------------------------------------

    for (int column = 1; column < columns - 1; column++)
    {
        sf::Sprite wallSprite = lowerWallTemplate;

        const float x = roomLeft + tileWidth / 2.f + column * tileWidth;

        wallSprite.setPosition({ x,lowerWallY });

        lowerWallSprites.push_back(wallSprite);
    }


    // Side Wall -------------------------------------

    const float sideWallHeight = leftBounds.size.y;

    int sideWallCount = static_cast<int>(std::ceil(roomHeight / sideWallHeight));

    if (sideWallCount < 1)
    {
        sideWallCount = 1;
    }

    float sideWallInterval = 0.f;

    if (sideWallCount > 1)
    {
        sideWallInterval = (roomHeight - sideWallHeight) / static_cast<float>(sideWallCount - 1);
    }

    // Left Wall -------------------------------------

    const float leftWallX = roomLeft + leftBounds.size.x / 2.f;

    for (int i = 0; i < sideWallCount; i++)
    {
        sf::Sprite wallSprite = leftWallTemplate;

        float y = roomTop + sideWallHeight / 2.f;

        if (sideWallCount > 1)
        {
            y += sideWallInterval * i;
        }
        else
        {
            y = centerPosition.y;
        }

        wallSprite.setPosition({ leftWallX,y });

        upperWallSprites.push_back(wallSprite);
    }

    // Right Wall -------------------------------------

    const float rightWallX = roomRight - rightBounds.size.x / 2.f;

    for (int i = 0; i < sideWallCount; i++)
    {
        sf::Sprite wallSprite = rightWallTemplate;

        float y = roomTop + sideWallHeight / 2.f;

        if (sideWallCount > 1)
        {
            y += sideWallInterval * i;
        }
        else
        {
            y = centerPosition.y;
        }

        wallSprite.setPosition({rightWallX, y});

        upperWallSprites.push_back(wallSprite);
    }

    // Colliders -------------------------------------

    Collider topCollider({ roomWidth,colliderThickness });
    topCollider.UpdatePosition({ centerPosition.x,roomTop - 30.f });
    wallColliders.push_back(topCollider);


    Collider bottomCollider({ roomWidth,colliderThickness });
    bottomCollider.UpdatePosition({centerPosition.x,roomBottom + 30.f});
    wallColliders.push_back(bottomCollider);


    Collider leftCollider({ colliderThickness,roomHeight + 30.f });
    leftCollider.UpdatePosition({ roomLeft + 20.f ,centerPosition.y });
    wallColliders.push_back(leftCollider);


    Collider rightCollider({ colliderThickness,roomHeight + 30.f });
    rightCollider.UpdatePosition({ roomRight - 20.f,centerPosition.y });
    wallColliders.push_back(rightCollider);
}

bool HomeScene::IsPlayerNearEntrance()
{
    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return false;
    }

    return player->GetBodyCollider().Collision(entranceInteractionCollider);
}