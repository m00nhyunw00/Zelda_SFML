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
    healAmount = static_cast<int>(entityManager->GetPlayer()->GetMaxHp() * 0.005f);

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
    if (input.IsEnterPressed() && IsPlayerNearEntrance())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(DUNGEON);

        return;
    }

    if (input.IsEscPressed() && IsPlayerNearEntrance())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->ResetDungeonStage();

        sceneManager->RequestSceneChange(TITLE);

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

    entityManager->Update(deltaTime,window);

    UpdateCollisions();

    if (player->GetHp() >= player->GetMaxHp())
    {
        healTimer = 0.f;
        return;
    }

    healTimer += deltaTime;

    if (healTimer >= healInterval)
    {
        player->Heal(healAmount);

        healTimer = 0.f;
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
    //for (const Collider& collider : wallColliders)
    //{
    //    collider.Draw(window);
    //}

    //for (const Collider& collider : objectColliders)
    //{
    //    collider.Draw(window);
    //}

    //entranceInteractionCollider.Draw(window);


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

    objectColliders.clear();

    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds =
        roomSprite->getGlobalBounds();

    const float left =
        bounds.position.x;

    const float top =
        bounds.position.y;

    const float width =
        bounds.size.x;

    const float height =
        bounds.size.y;


    // =========================================================
    // 콜라이더 생성용 함수
    // =========================================================

    auto AddWallCollider =
        [&](float xRatio,
            float yRatio,
            float widthRatio,
            float heightRatio)
        {
            Collider collider({
                width * widthRatio,
                height * heightRatio
                });

            collider.UpdatePosition({
                left + width * xRatio,
                top + height * yRatio
                });

            wallColliders.push_back(
                collider
            );
        };


    auto AddObjectCollider =
        [&](float xRatio,
            float yRatio,
            float widthRatio,
            float heightRatio)
        {
            Collider collider({
                width * widthRatio,
                height * heightRatio
                });

            collider.UpdatePosition({
                left + width * xRatio,
                top + height * yRatio
                });

            objectColliders.push_back(
                collider
            );
        };

    // =========================================================
    // 방 벽
    // 캐릭터/몬스터/투사체 모두 충돌
    // =========================================================

    const float playableLeft =
        left + width * 0.055f;

    const float playableRight =
        left + width * 0.945f;

    const float playableTop =
        top + height * 0.19f;

    const float playableBottom =
        top + height * 0.84f;

    const float playableWidth =
        playableRight - playableLeft;

    const float playableHeight =
        playableBottom - playableTop;

    const float colliderThickness =
        8.f * Constants::ROOM_SCALE;

    // 위쪽 벽
    Collider topCollider({
        playableWidth,
        colliderThickness
        });

    topCollider.UpdatePosition({
        playableLeft +
            playableWidth / 2.f,

        playableTop
        });

    wallColliders.push_back(
        topCollider
    );

    // 아래쪽 벽
    Collider bottomCollider({
        playableWidth,
        colliderThickness
        });

    bottomCollider.UpdatePosition({
        playableLeft +
            playableWidth / 2.f,

        playableBottom * 0.96f
        });

    wallColliders.push_back(
        bottomCollider
    );

    // 왼쪽 벽
    Collider leftCollider({
        colliderThickness,
        playableHeight
        });

    leftCollider.UpdatePosition({
        playableLeft,

        playableTop +
            playableHeight / 2.f
        });

    wallColliders.push_back(
        leftCollider
    );

    // 오른쪽 벽
    Collider rightCollider({
        colliderThickness,
        playableHeight
        });

    rightCollider.UpdatePosition({
        playableRight,

        playableTop +
            playableHeight / 2.f
        });

    wallColliders.push_back(
        rightCollider
    );

    // =========================================================
    // 벽에 붙어 있는 큰 오브젝트
    //
    // 캐릭터/몬스터 통과 X
    // 투사체 통과 X
    // =========================================================

    // 왼쪽 책장
    AddWallCollider(
        0.115f,
        0.215f,
        0.110f,
        0.120f
    );

    // 벽난로
    AddWallCollider(
        0.240f,
        0.205f,
        0.145f,
        0.120f
    );

    // =========================================================
    // 일반 가구
    //
    // 캐릭터/몬스터 통과 X
    // 투사체 통과 O
    // =========================================================

    // 위쪽 중앙 책상
    AddObjectCollider(
        0.495f,
        0.245f,
        0.130f,
        0.100f
    );

    // 중앙 책상 오른쪽 상자
    AddObjectCollider(
        0.605f,
        0.245f,
        0.085f,
        0.100f
    );

    // 침대
    AddObjectCollider(
        0.800f,
        0.290f,
        0.130f,
        0.180f
    );

    // 침대 오른쪽 선반
    AddObjectCollider(
        0.905f,
        0.255f,
        0.080f,
        0.150f
    );

    // ---------------- 식탁 ----------------

    AddObjectCollider(
        0.265f,
        0.500f,
        0.215f,
        0.130f
    );

    // ---------------- 식탁 의자 ----------------

    // 왼쪽 위 의자
    AddObjectCollider(
        0.205f,
        0.385f,
        0.055f,
        0.065f
    );

    // 오른쪽 위 의자
    AddObjectCollider(
        0.315f,
        0.385f,
        0.055f,
        0.065f
    );

    // 왼쪽 아래 의자
    AddObjectCollider(
        0.210f,
        0.605f,
        0.055f,
        0.020f
    );

    // 오른쪽 아래 의자
    AddObjectCollider(
        0.315f,
        0.605f,
        0.055f,
        0.020f
    );

    // =========================================================
    // 왼쪽 아래 오브젝트
    // =========================================================

    // 위쪽 통
    AddObjectCollider(
        0.075f,
        0.680f,
        0.075f,
        0.080f
    );

    // 아래쪽 통
    AddObjectCollider(
        0.085f,
        0.770f,
        0.075f,
        0.085f
    );

    // 항아리
    AddObjectCollider(
        0.145f,
        0.720f,
        0.060f,
        0.085f
    );

    // 왼쪽 아래 상자
    AddObjectCollider(
        0.225f,
        0.770f,
        0.080f,
        0.080f
    );

    // =========================================================
    // 아래쪽 화분
    // =========================================================

    // 왼쪽 화분
    AddObjectCollider(
        0.640f,
        0.760f,
        0.075f,
        0.080f
    );

    // 오른쪽 화분
    AddObjectCollider(
        0.715f,
        0.760f,
        0.075f,
        0.080f
    );

    // =========================================================
    // 오른쪽 아래 상자
    // =========================================================

    AddObjectCollider(
        0.900f,
        0.730f,
        0.080f,
        0.180f
    );
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