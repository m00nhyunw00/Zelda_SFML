#include "InGameScene.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "InputManager.h"

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
        skillCooldownUI =
            new SkillCooldownUI(
                *font,
                { 64.f, 64.f },
                {Constants::CENTER_X + 200.f, Constants::WINDOW_HEIGHT * 0.925f},
                player->GetMaxSkillCooldown()
            );

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

        gameOverOverlay->setFillColor(
            sf::Color(0, 0, 0, 150)
        );

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

    delete playerExpBar;
    playerExpBar = nullptr;

    delete levelText;
    levelText = nullptr;
}

bool InGameScene::CheckGameOver()
{
    Player* player = entityManager->GetPlayer();

    isGameOver = ((player != nullptr && !player->IsActive()) ? true : false);

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

    // Restart
    if (restartButton != nullptr &&
        restartButton->IsClicked())
    {
        entityManager->ClearMonsters();
        entityManager->DeletePlayer();

        sceneManager->RequestSceneChange(TITLE);

        return;
    }

    // Exit
    if (exitButton != nullptr &&
        exitButton->IsClicked())
    {
        window.close();

        return;
    }
}

void InGameScene::UpdateUI(float deltaTime, sf::RenderWindow& window)
{
    UpdatePlayerHpBar(deltaTime, window);

    UpdateSkillCooldownUI(deltaTime, window);

    UpdateUltimateGauge(deltaTime, window);

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

    skillCooldownUI->SetCooldown(player->GetSkillCooldown());

    skillCooldownUI->Update(deltaTime, window);
}

void InGameScene::UpdateGameOver(
    float deltaTime,
    sf::RenderWindow& window)
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

void InGameScene::UpdateProjectileWallCollisions()
{
    if (entityManager == nullptr)
    {
        return;
    }

    entityManager->CheckProjectileWallCollisions(wallColliders);
}