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

#include "HomeScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "Constants.h"

#include <iostream>

using namespace std;

HomeScene::HomeScene(SceneManager* sceneManager, EntityManager* entityManager)
    : InGameScene(sceneManager, entityManager),
    camera(
        {
            Constants::CENTER_X,
            Constants::CENTER_Y
        },
        {
            static_cast<float>(Constants::WINDOW_WIDTH),
            static_cast<float>(Constants::WINDOW_HEIGHT)
        }
    )
{
    CreateHomeRoom();

    BuildRoomColliders();

    SetupEntrance();

    MovePlayerToSpawn();
}

HomeScene::~HomeScene()
{
    delete roomSprite;
    roomSprite = nullptr;

    wallColliders.clear();
}

void HomeScene::HandleEvent(const sf::Event& event,sf::RenderWindow& window)
{
    (void)event;
    (void)window;

    InputManager& input =InputManager::GetInstance();

    // 테스트용
    if (input.IsNum1Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        sceneManager->RequestSceneChange(TITLE);

        return;
    }


    // 테스트용 Dungeon 이동
    if (input.IsNum2Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(DUNGEON);

        return;
    }

    // 테스트용 BossRoom 이동
    if (input.IsNum3Pressed())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(BOSS);

        return;
    }

    // 실제 출입구
    if ((input.IsFPressed() || input.IsEnterPressed()) && IsPlayerNearEntrance())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(DUNGEON);

        return;
    }
}

void HomeScene::Update(float deltaTime, sf::RenderWindow& window)
{
    Player* player = entityManager->GetPlayer();

    CheckGameOver();

    if (player == nullptr || isGameOver)
    {
        UpdateGameOver(deltaTime,window);

        return;
    }

    camera.Follow(player->GetPosition());

    const sf::Vector2f previousPosition = player->GetPosition();

    entityManager->Update(deltaTime,window);

    UpdateProjectileWallCollisions();

    for (const Collider& roomCollider : wallColliders)
    {
        if (player->GetBodyCollider().Collision(roomCollider))
        {
            player->MoveForce(previousPosition
            );

            break;
        }
    }

    UpdateUI(deltaTime,window);
}

void HomeScene::Render(sf::RenderWindow& window)
{
    window.setView(camera.GetView());


    if (roomSprite != nullptr)
    {
        window.draw(*roomSprite);
    }


    entityManager->Render(window);


    // Collider Debug
    for (const Collider& collider : wallColliders)
    {
        collider.Draw(window);
    }

    entranceInteractionCollider.Draw(window);


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

void HomeScene::CreateHomeRoom()
{
    sf::Texture* homeTexture =
        ResourceManager::GetInstance().GetTexture("Home");

    if (homeTexture == nullptr)
    {
        cerr << "[HomeScene] Home texture not found" << endl;
        return;
    }

    roomSprite = new sf::Sprite(*homeTexture);

    // 실제 집 영역만 사용
    roomSprite->setTextureRect(
        sf::IntRect(
            { 250, 60 },
            { 1040, 915 }
        )
    );

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

void HomeScene::BuildRoomColliders()
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

    // 실제 걸어다니는 내부 영역
    const float playableLeft = left + width * 0.055f;

    const float playableRight = left + width * 0.945f;

    const float playableTop = top + height * 0.19f;

    const float playableBottom = top + height * 0.84f;

    const float playableWidth =playableRight - playableLeft;

    const float playableHeight =playableBottom - playableTop;

    const float colliderThickness = 8.f * Constants::ROOM_SCALE;

    // 위
    Collider topCollider({ playableWidth,colliderThickness });

    topCollider.UpdatePosition({ playableLeft + playableWidth / 2.f, playableTop });

    wallColliders.push_back(topCollider);


    // 아래
    Collider bottomCollider({ playableWidth,colliderThickness });

    bottomCollider.UpdatePosition({ playableLeft + playableWidth / 2.f, playableBottom * 0.96f });

    wallColliders.push_back(bottomCollider);


    // 왼쪽
    Collider leftCollider({ colliderThickness,playableHeight });

    leftCollider.UpdatePosition({ playableLeft, playableTop + playableHeight / 2.f });

    wallColliders.push_back(leftCollider);


    // 오른쪽
    Collider rightCollider({ colliderThickness, playableHeight });

    rightCollider.UpdatePosition({ playableRight,playableTop + playableHeight / 2.f });

    wallColliders.push_back(rightCollider);
}

void HomeScene::SetupEntrance()
{
    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const float centerX = bounds.position.x + bounds.size.x / 2.f;

    entranceInteractionCollider.SetSize({ 150.f * Constants::ROOM_SCALE, 200.f * Constants::ROOM_SCALE });

    entranceInteractionCollider.UpdatePosition({ centerX,bounds.position.y + bounds.size.y * 0.85f });
}

void HomeScene::MovePlayerToSpawn()
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
        bounds.position.y + bounds.size.y * 0.7f
    };

    player->MoveForce(spawnPosition);
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