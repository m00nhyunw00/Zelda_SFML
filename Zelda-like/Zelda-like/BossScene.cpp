#include "BossScene.h"
#include "EndingScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

using namespace std;
BossScene::BossScene(SceneManager* sceneManager,EntityManager* entityManager)
    : InGameScene(sceneManager, entityManager),
    camera(
        { Constants::CENTER_X, Constants::CENTER_Y },
          {
              static_cast<float>(Constants::WINDOW_WIDTH),
              static_cast<float>(Constants::WINDOW_HEIGHT)
          })
{
    CreateDungeonRoom();

    BuildRoomColliders();

    MovePlayerToSpawn();

    SpawnBoss();

    SpawnPhase1Monsters();
}

BossScene::~BossScene()
{
    delete roomSprite;
    roomSprite = nullptr;

    wallColliders.clear();
}

void BossScene::CreateDungeonRoom()
{
    sf::Texture* dungeonTexture = ResourceManager::GetInstance().GetTexture("BossRoom");

    if (dungeonTexture == nullptr)
    {
        cerr << "[BossScene] Boss texture not found" << endl;
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

void BossScene::SpawnBoss()
{
    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const sf::Vector2f bossPosition =
    {
        bounds.position.x + bounds.size.x / 2.f,

        bounds.position.y + bounds.size.y * 0.35f
    };

    Monster* monster =
        entityManager->SpawnMonster(
            MonsterType::GIANT_SLIME,
            MonsterColor::NONE_COLOR,
            bossPosition
        );

    boss = dynamic_cast<GiantSlime*>(monster);
}

void BossScene::SpawnPhase1Monsters()
{
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

    // 가운데를 제외한 8방향 위치
    const sf::Vector2f positions[8] =
    {
        { left + width * 0.25f, top + height * 0.30f }, // 좌상
        { left + width * 0.50f, top + height * 0.30f }, // 상
        { left + width * 0.75f, top + height * 0.30f }, // 우상

        { left + width * 0.25f, top + height * 0.50f }, // 좌
        { left + width * 0.75f, top + height * 0.50f }, // 우

        { left + width * 0.25f, top + height * 0.70f }, // 좌하
        { left + width * 0.50f, top + height * 0.70f }, // 하
        { left + width * 0.75f, top + height * 0.70f }  // 우하
    };

    const MonsterType types[4] =
    {
        MonsterType::SLIME,
        MonsterType::SKELETON,
        MonsterType::CACTO,
        MonsterType::LICH
    };

    for (int i = 0; i < 8; i++)
    {
        entityManager->SpawnMonster(
            types[i % 4],
            MonsterColor::NONE_COLOR,
            positions[i]
        );
    }
}

void BossScene::CheckBossPhase()
{
    if (boss == nullptr)
    {
        return;
    }

    if (boss->ConsumePhase3Entered())
    {
        SpawnPhase3Liches();
    }
}

void BossScene::SpawnPhase3Liches()
{
    if (roomSprite == nullptr)
    {
        return;
    }

    const sf::FloatRect bounds = roomSprite->getGlobalBounds();

    const float left = bounds.position.x;

    const float top = bounds.position.y;

    const float width = bounds.size.x;

    const float height = bounds.size.y;

    const sf::Vector2f positions[4] =
    {
        {
            left + width * 0.13f,
            top + height * 0.15f
        },

        {
            left + width * 0.87f,
            top + height * 0.15f
        },

        {
            left + width * 0.13f,
            top + height * 0.79f
        },

        {
            left + width * 0.87f,
            top + height * 0.79f
        }
    };

    for (const sf::Vector2f& position : positions)
    {
        Monster* monster = entityManager->SpawnMonster(
                MonsterType::LICH,
                MonsterColor::NONE_COLOR,
                position
            );

        if (monster != nullptr)
        {
            monster->SetCanMove(false);
            monster->SetInvincible(true);
            monster->SetShowHpBar(false);
            monster->SetAttakRange(monster->GetAttakRange() * 1.2f);
        }
    }
}

void BossScene::Update(float deltaTime, sf::RenderWindow& window)
{
    Player* player = entityManager->GetPlayer();

    CheckGameOver();

    if (player == nullptr || isGameOver)
    {
        UpdateGameOver(deltaTime, window);
        return;
    }

    camera.Follow(player->GetPosition());

    const sf::Vector2f previousPosition = player->GetPosition();

    CheckBossPhase();

    entityManager->Update(deltaTime, window);

    // 보스 사망 시 엔딩씬으로 전환
    if (boss != nullptr && !boss->IsActive())
    {
        SaveManager::GetInstance().SavePlayer(entityManager->GetPlayer());

        entityManager->ClearProjectiles();
        entityManager->ClearMonsters();

        sceneManager->RequestSceneChange(ENDING);

        return;
    }

    CheckBossPhase();

    UpdateProjectileWallCollisions();

    for (const Collider& roomCollider : wallColliders)
    {
        if (player->GetBodyCollider().Collision(roomCollider))
        {
            player->MoveForce(previousPosition);

            break;
        }
    }

    UpdateUI(deltaTime, window);
}

void BossScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (isGameOver)
    {
        HandleGameOverEvent(event, window);

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
}

void BossScene::Render(sf::RenderWindow& window)
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

void BossScene::BuildRoomColliders()
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
    Collider topCollider({ playableWidth,wallThickness });

    topCollider.UpdatePosition({ playableLeft + playableWidth / 2.f,playableTop });

    wallColliders.push_back(topCollider);


    // 아래쪽 벽
    Collider bottomCollider({ playableWidth,wallThickness });

    bottomCollider.UpdatePosition({ playableLeft + playableWidth / 2.f,playableBottom });

    wallColliders.push_back(bottomCollider);


    // 왼쪽 벽
    Collider leftCollider({ wallThickness,playableHeight });

    leftCollider.UpdatePosition({ playableLeft,playableTop + playableHeight / 2.f });

    wallColliders.push_back(leftCollider);


    // 오른쪽 벽
    Collider rightCollider({ wallThickness,playableHeight });

    rightCollider.UpdatePosition({ playableRight,playableTop + playableHeight / 2.f });

    wallColliders.push_back(rightCollider);
}

void BossScene::MovePlayerToSpawn()
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

bool BossScene::CanUseEntrance()
{
    return entityManager->AreAllMonstersDead();
}

//MonsterColor BossScene::GetRandomMonsterColor()
//{
//    const int random = rand() % 12;
//
//    switch (random)
//    {
//    case 0:
//        return MonsterColor::BLACK;
//
//    case 1:
//        return MonsterColor::RED;
//
//    case 2:
//        return MonsterColor::BLUE;
//
//    case 3:
//        return MonsterColor::PURPLE;
//    }
//
//    return MonsterColor::NONE_COLOR;
//}
//
//MonsterType BossScene::GetRandomMonsterType()
//{
//    Player* player = entityManager->GetPlayer();
//
//    if (player == nullptr)
//    {
//        return MonsterType::SLIME;
//    }
//
//    const MonsterSpawnData* spawnData = DataManager::GetInstance().GetMonsterSpawnData(player->GetLevel());
//
//    if (spawnData == nullptr)
//    {
//        return MonsterType::SLIME;
//    }
//
//    const int random = rand() % 100;
//
//    int accumulatedRate = 0;
//
//    accumulatedRate += spawnData->slimeRate;
//
//    if (random < accumulatedRate)
//    {
//        return MonsterType::SLIME;
//    }
//
//    accumulatedRate += spawnData->cactoRate;
//
//    if (random < accumulatedRate)
//    {
//        return MonsterType::CACTO;
//    }
//
//    accumulatedRate += spawnData->skeletonRate;
//
//    if (random < accumulatedRate)
//    {
//        return MonsterType::SKELETON;
//    }
//
//    return MonsterType::LICH;
//}