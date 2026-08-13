#include "InGameScene.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SaveManager.h"
#include "PlayerType.h"
#include <iostream>

using namespace std;

InGameScene::InGameScene(SceneManager* sceneManager, EntityManager* entityManager) 
    : Scene(sceneManager, entityManager)
{
    isGameOver = false;

    this->sceneManager = sceneManager;
    this->entityManager = entityManager;

    // ---------------- Player Guage Bar ----------------

    Player* player = entityManager->GetPlayer();

    if (player != nullptr)
    {
        playerHpBar = new GaugeBar(
            { 300.f, 24.f },      // 크기
            { Constants::CENTER_X, Constants::WINDOW_HEIGHT * 0.875f },       // 화면 위치
            static_cast<float>(player->GetMaxHp())
        );

        playerHpBar->SetValue(static_cast<float>(player->GetHp()));
        playerHpBar->SetGaugeColor(sf::Color::Red);


        playerUltimateBar = new GaugeBar(
            { 300.f, 24.f },      // 크기
            { Constants::CENTER_X, Constants::WINDOW_HEIGHT * 0.92f },       // 화면 위치
            static_cast<float>(player->GetMaxUltimateGauge())
        );

        playerUltimateBar->SetValue(static_cast<float>(player->GetUltimateGauge()));
        playerUltimateBar->SetGaugeColor(sf::Color::Yellow);

        playerExpBar = new GaugeBar(
            { 300.f, 14.f },
            { Constants::CENTER_X,Constants::WINDOW_HEIGHT * 0.96f },
            static_cast<float>(player->GetMaxExp())
        );

        playerExpBar->SetValue(static_cast<float>(player->GetCurrentExp()));

        playerExpBar->SetGaugeColor(sf::Color::Cyan);
    }

    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font != nullptr && player != nullptr)
    {
        ResourceManager& resourceManager = ResourceManager::GetInstance();

        sf::Texture* skillIconTexture = nullptr;
        sf::Texture* ultimateIconTexture = nullptr;

        switch (player->GetJob())
        {
        case PlayerType::WARRIOR:
            skillIconTexture = resourceManager.GetTexture("PowerStrike_Icon");
            ultimateIconTexture = resourceManager.GetTexture("FlameBlade_Icon");
            break;

        case PlayerType::ARCHER:
            skillIconTexture = resourceManager.GetTexture("TripleShot_Icon");
            ultimateIconTexture = resourceManager.GetTexture("TitanArrow_Icon");
            break;

        case PlayerType::MAGE:
            // 마법사 스킬 구현 후 추가
            break;

        default:
            break;
        }

        // 스킬 아이콘 + 쿨타임 UI
        if (skillIconTexture != nullptr)
        {
            skillCooldownUI = new SkillCooldownUI(
                *font,
                *skillIconTexture,
                { 48.f, 48.f },
                { Constants::CENTER_X + 200.f, Constants::WINDOW_HEIGHT * 0.925f },
                player->GetMaxSkillCooldown()
            );

            skillCooldownUI->SetCooldown(player->GetSkillCooldown());
        }

        // 궁극기 아이콘 UI
        if (ultimateIconTexture != nullptr)
        {
            ultimateUI = new UltimateUI(
                *ultimateIconTexture,
                { 48.f, 48.f },
                { Constants::CENTER_X + 268.f, Constants::WINDOW_HEIGHT * 0.925f },
                static_cast<float>(player->GetMaxUltimateGauge())
            );

            ultimateUI->SetGauge(
                static_cast<float>(player->GetUltimateGauge())
            );
        }

        // 기존 Level Text
        levelText = new sf::Text(*font);

        levelText->setCharacterSize(22);
        levelText->setFillColor(sf::Color::White);
        levelText->setString("Lv. " + std::to_string(player->GetLevel()));

        const sf::FloatRect bounds = levelText->getLocalBounds();

        levelText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        levelText->setPosition({
            Constants::CENTER_X - 185.f,
            Constants::WINDOW_HEIGHT * 0.96f
            });
    }

    if (font != nullptr)
    {
        // ---------------- Game Over Overlay ----------------

        gameOverOverlay = new sf::RectangleShape();

        gameOverOverlay->setSize({
            static_cast<float>(Constants::WINDOW_WIDTH),
            static_cast<float>(Constants::WINDOW_HEIGHT)
            });

        gameOverOverlay->setPosition({ 0.f, 0.f });

        gameOverOverlay->setFillColor(sf::Color(0, 0, 0, 150));

        // ---------------- GAME OVER Text ----------------

        gameOverText = new sf::Text(*font);

        gameOverText->setString("GAME OVER");
        gameOverText->setCharacterSize(72);
        gameOverText->setFillColor(sf::Color::White);

        sf::FloatRect bounds = gameOverText->getLocalBounds();

        gameOverText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        gameOverText->setPosition({
            Constants::CENTER_X,
            Constants::CENTER_Y - 180.f
            });

        // ---------------- Restart Button ----------------

        restartButton = new Button(
            *font,
            "Restart",
            { 300.f, 70.f },
        {
            Constants::CENTER_X,
            Constants::CENTER_Y - 20.f
        }
        );

        // ---------------- Exit Button ----------------

        exitButton = new Button(
            *font,
            "Exit",
            { 300.f, 70.f },
        {
            Constants::CENTER_X,
            Constants::CENTER_Y + 80.f
        }
        );

        // ---------------- Ending UI ----------------

        sf::Texture* endingTexture = ResourceManager::GetInstance().GetTexture("EndingScene");

        if (endingTexture == nullptr)
        {
            cerr << "[InGameScene] Ending texture not found" << endl;
        }
        else
        {
            endingSprite = new sf::Sprite(*endingTexture);

            const sf::Vector2u textureSize = endingTexture->getSize();

            // 화면 전체 크기에 맞춤
            endingSprite->setScale({
                static_cast<float>(Constants::WINDOW_WIDTH) / static_cast<float>(textureSize.x),
                static_cast<float>(Constants::WINDOW_HEIGHT) / static_cast<float>(textureSize.y)
                });

            endingSprite->setPosition({
                0.f,
                0.f
                });

            // 처음에는 완전 투명
            endingSprite->setColor(
                sf::Color(
                    255,
                    255,
                    255,
                    0
                )
            );
        }
    }
}

InGameScene::~InGameScene()
{
    delete gameOverOverlay;
    gameOverOverlay = nullptr;

    delete gameOverText;
    gameOverText = nullptr;

    delete restartButton;
    restartButton = nullptr;

    delete exitButton;
    exitButton = nullptr;

    delete playerHpBar;
    playerHpBar = nullptr;

    delete playerUltimateBar;
    playerUltimateBar = nullptr;

    delete skillCooldownUI;
    skillCooldownUI = nullptr;

    delete ultimateUI;
    ultimateUI = nullptr;

    delete playerExpBar;
    playerExpBar = nullptr;

    delete levelText;
    levelText = nullptr;

    delete endingSprite;
    endingSprite = nullptr;
}

bool InGameScene::CheckGameOver()
{
    // 이미 GameOver에 들어갔다면 계속 유지
    if (isGameOver)
    {
        return true;
    }

    Player* player = entityManager->GetPlayer();

    if (player != nullptr && !player->IsActive())
    {
        isGameOver = true;
    }

    return isGameOver;
}

void InGameScene::HandleGameOverEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (!isGameOver)
    {
        return;
    }

    if (restartButton != nullptr)
    {
        restartButton->HandleEvent(event, window);
    }

    if (exitButton != nullptr)
    {
        exitButton->HandleEvent(event, window);
    }

    const bool restartClicked = restartButton != nullptr && restartButton->IsClicked();

    const bool exitClicked = exitButton != nullptr && exitButton->IsClicked();

    // 아무 버튼도 누르지 않았으면 아무 것도 하지 않음
    if (!restartClicked && !exitClicked)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    // Restart / Exit 공통 사망 처리
    if (player != nullptr)
    {
        player->ApplyDeathPenalty();

        SaveManager::GetInstance().SavePlayer(player);
    }

    entityManager->ClearProjectiles();
    entityManager->ClearMonsters();

    // Restart
    if (restartClicked)
    {
        sceneManager->RequestSceneChange(HOME);
        return;
    }

    // Exit
    if (exitClicked)
    {
        window.close();
        return;
    }
}

void InGameScene::StartEnding()
{
    if (isEnding)
    {
        return;
    }

    isEnding = true;

    endingAlpha = 0.f;

    if (endingSprite != nullptr)
    {
        endingSprite->setColor(
            sf::Color(
                255,
                255,
                255,
                0
            )
        );
    }

    // 버튼 위치및 텍스트 변경

    if (restartButton != nullptr)
    {
        restartButton->SetPosition({
            Constants::CENTER_X - 170.f,
            Constants::WINDOW_HEIGHT - 80.f
            });

        restartButton->SetText("Home");
    }

    if (exitButton != nullptr)
    {
        exitButton->SetPosition({
            Constants::CENTER_X + 170.f,
            Constants::WINDOW_HEIGHT - 80.f
            });
    }
}

void InGameScene::UpdateEnding(float deltaTime, sf::RenderWindow& window)
{
    if (!isEnding || endingSprite == nullptr)
    {
        return;
    }

    // 0 → 255까지 endingFadeDuration 동안 증가
    endingAlpha += (255.f / endingFadeDuration) * deltaTime;

    if (endingAlpha > 255.f)
    {
        endingAlpha = 255.f;
    }

    endingSprite->setColor(
        sf::Color(
            255,
            255,
            255,
            static_cast<std::uint8_t>(endingAlpha)
        )
    );

    if (restartButton != nullptr)
    {
        restartButton->Update(deltaTime, window);
    }

    if (exitButton != nullptr)
    {
        exitButton->Update(deltaTime, window);
    }
}

void InGameScene::HandleEndingEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (!isEnding)
    {
        return;
    }

    if (restartButton != nullptr)
    {
        restartButton->HandleEvent(event, window);
    }

    if (exitButton != nullptr)
    {
        exitButton->HandleEvent(event, window);
    }

    const bool restartClicked = restartButton != nullptr && restartButton->IsClicked();

    const bool exitClicked = exitButton != nullptr && exitButton->IsClicked();

    // 아무 버튼도 누르지 않았으면 아무 것도 하지 않음
    if (!restartClicked && !exitClicked)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();
    SaveManager::GetInstance().SavePlayer(player);

    entityManager->ClearProjectiles();
    entityManager->ClearMonsters();

    // Restart
    if (restartClicked)
    {
        sceneManager->RequestSceneChange(HOME);
        return;
    }

    // Exit
    if (exitClicked)
    {
        window.close();
        return;
    }
}

void InGameScene::RenderEnding(sf::RenderWindow& window)
{
    if (!isEnding)
    {
        return;
    }

    if (endingSprite != nullptr)
    {
        window.draw(*endingSprite);
    }

    if (restartButton != nullptr && endingAlpha == 255.f)
    {
        restartButton->Render(window);
    }

    if (exitButton != nullptr && endingAlpha == 255.f)
    {
        exitButton->Render(window);
    }
}

bool InGameScene::CheckEnding()
{
    return isEnding;
}

void InGameScene::UpdateUI(float deltaTime, sf::RenderWindow& window)
{
    UpdatePlayerHpBar(deltaTime, window);

    UpdateSkillCooldownUI(deltaTime, window);

    UpdateUltimateGauge(deltaTime, window);

    UpdateUltimateUI(deltaTime, window);

    UpdateExpBar(deltaTime, window);

    UpdateLevelText();
}

void InGameScene::UpdatePlayerHpBar(float deltaTime, sf::RenderWindow& window)
{
    if (playerHpBar == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    playerHpBar->SetValue(static_cast<float>(player->GetHp()));

    playerHpBar->Update(deltaTime, window);
}

void InGameScene::UpdateUltimateGauge(float deltaTime,sf::RenderWindow& window)
{
    if (playerUltimateBar == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    playerUltimateBar->SetValue(static_cast<float>(player->GetUltimateGauge()));

    playerUltimateBar->Update(deltaTime, window);
}

void InGameScene::UpdateUltimateUI(float deltaTime, sf::RenderWindow& window)
{
    if (ultimateUI == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    ultimateUI->SetMaxGauge(static_cast<float>(player->GetMaxUltimateGauge()));

    ultimateUI->SetGauge(static_cast<float>(player->GetUltimateGauge()));

    ultimateUI->Update(deltaTime, window);
}

void InGameScene::UpdateExpBar(float deltaTime, sf::RenderWindow& window)
{
    if (playerExpBar == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    playerExpBar->SetValue(static_cast<float>(player->GetCurrentExp()));

    playerExpBar->Update(deltaTime, window);
}

void InGameScene::UpdateLevelText()
{
    if (levelText == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    levelText->setString("Lv. " + std::to_string(player->GetLevel()));

    const sf::FloatRect bounds = levelText->getLocalBounds();

    levelText->setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });
}

void InGameScene::UpdateSkillCooldownUI(float deltaTime, sf::RenderWindow& window)
{
    if (skillCooldownUI == nullptr)
    {
        return;
    }

    Player* player = entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    skillCooldownUI->SetMaxCooldown(player->GetMaxSkillCooldown());

    skillCooldownUI->SetCooldown(player->GetSkillCooldown());

    skillCooldownUI->Update(deltaTime, window);
}

void InGameScene::UpdateGameOver(float deltaTime, sf::RenderWindow& window)
{
    if (restartButton != nullptr)
    {
        restartButton->Update(deltaTime,window);
    }

    if (exitButton != nullptr)
    {
        exitButton->Update(deltaTime,window);
    }
}

void InGameScene::RenderUI(sf::RenderWindow& window)
{
    if (playerHpBar != nullptr)
    {
        playerHpBar->Render(window);
    }

    if (playerUltimateBar != nullptr)
    {
        playerUltimateBar->Render(window);
    }

    if (skillCooldownUI != nullptr)
    {
        skillCooldownUI->Render(window);
    }

    if (ultimateUI != nullptr)
    {
        ultimateUI->Render(window);
    }

    if (playerExpBar != nullptr)
    {
        playerExpBar->Render(window);
    }

    if (levelText != nullptr)
    {
        window.draw(*levelText);
    }
}

void InGameScene::RenderGameOver(sf::RenderWindow& window)
{
    if (!isGameOver)
    {
        return;
    }

    if (gameOverOverlay != nullptr)
    {
        window.draw(*gameOverOverlay);
    }

    if (gameOverText != nullptr)
    {
        window.draw(*gameOverText);
    }

    if (restartButton != nullptr)
    {
        restartButton->Render(window);
    }

    if (exitButton != nullptr)
    {
        exitButton->Render(window);
    }
}

void InGameScene::UpdateCollisions()
{
    if (entityManager == nullptr)
    {
        return;
    }

    entityManager->CheckCreatureObjectCollisions(wallColliders);
    entityManager->CheckCreatureObjectCollisions(objectColliders);
    entityManager->CheckProjectileWallCollisions(wallColliders);
}