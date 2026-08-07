#include "HomeScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

HomeScene::HomeScene(SceneManager* sceneManager, EntityManager* entitymanager) : Scene(sceneManager, entitymanager)
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
}

HomeScene::~HomeScene()
{
    floorSprites.clear();
    upperWallSprites.clear();
    lowerWallSprites.clear();
    roomColliders.clear();
}

void HomeScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // 숫자 키
    if (input.IsNum1Pressed())
    {
        entityManager->ClearMonsters();
        sceneManager->RequestSceneChange(TITLE);
        return;
    }

    if (input.IsNum2Pressed())
    {
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

void HomeScene::Update(
    float deltaTime,
    sf::RenderWindow& window)
{
    Player* player =
        entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    // 이동 전 위치 저장
    const sf::Vector2f previousPosition =
        player->GetPosition();

    // Player를 포함한 Entity 업데이트
    entityManager->Update(
        deltaTime,
        window
    );

    // 방 외벽과 충돌 검사
    for (const Collider& roomCollider : roomColliders)
    {
        if (player->GetBodyCollider().Collision(roomCollider))
        {
            // 충돌했다면 이동 전 위치로 복구
            player->MoveForce(previousPosition);

            break;
        }
    }
}

void HomeScene::Render(sf::RenderWindow& window)
{
    for (const sf::Sprite& floorSprite : floorSprites)
    {
        window.draw(floorSprite);
    }

    for (const sf::Sprite& wallSprite : upperWallSprites)
    {
        window.draw(wallSprite);
    }

    Player* player = entityManager->GetPlayer();

    if (player != nullptr)
    {
        player->Render(window);
    }

    for (const sf::Sprite& wallSprite : lowerWallSprites)
    {
        window.draw(wallSprite);
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
            roomLeft
                + tileWidth / 2.f
                + column * tileWidth,

            roomTop
                - upperWallBounds.size.y / 2.f
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

    roomColliders.push_back(topCollider);

    // 아래쪽 Collider
    Collider bottomCollider({
        roomWidth,
        colliderThickness
        });

    bottomCollider.UpdatePosition({
        centerPosition.x,
        roomBottom + colliderThickness / 2.f
        });

    roomColliders.push_back(bottomCollider);

    // 왼쪽 Collider
    Collider leftCollider({
        colliderThickness,
        roomHeight
        });

    leftCollider.UpdatePosition({
        roomLeft - colliderThickness / 2.f,
        centerPosition.y
        });

    roomColliders.push_back(leftCollider);

    // 오른쪽 Collider
    Collider rightCollider({
        colliderThickness,
        roomHeight
        });

    rightCollider.UpdatePosition({
        roomRight + colliderThickness / 2.f,
        centerPosition.y
        });

    roomColliders.push_back(rightCollider);
}
