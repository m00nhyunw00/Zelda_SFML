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

    // ---------------- Player HP Bar ----------------

    Player* player = entityManager->GetPlayer();

    if (player != nullptr)
    {
        playerHpBar = new GaugeBar(
            { 300.f, 24.f },      // 크기
            { Constants::CENTER_X, Constants::WINDOW_HEIGHT * 0.9f },       // 화면 위치
            static_cast<float>(player->GetMaxHp())
        );

        playerHpBar->SetValue(static_cast<float>(player->GetHp()));
        playerHpBar->SetGaugeColor(sf::Color::Red);


        playerUltimateBar = new GaugeBar(
            { 300.f, 24.f },      // 크기
            { Constants::CENTER_X, Constants::WINDOW_HEIGHT * 0.95f },       // 화면 위치
            static_cast<float>(player->GetMaxHp())
        );

        playerUltimateBar->SetValue(static_cast<float>(player->GetHp()));
        playerUltimateBar->SetGaugeColor(sf::Color::Yellow);
    }

    sf::Font* font =ResourceManager::GetInstance().GetFont("MainFont");

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

void InGameScene::UpdatePlayerHpBar(float deltaTime, sf::RenderWindow& window)
{
    if (playerHpBar == nullptr)
    {
        return;
    }

    Player* player =
        entityManager->GetPlayer();

    if (player == nullptr)
    {
        return;
    }

    playerHpBar->SetValue(static_cast<float>(player->GetHp()));

    playerHpBar->Update(deltaTime, window);
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

void InGameScene::RenderPlayerBar(sf::RenderWindow& window)
{
    if (playerHpBar == nullptr || playerUltimateBar == nullptr)
    {
        return;
    }

    playerHpBar->Render(window);
    playerUltimateBar->Render(window);
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