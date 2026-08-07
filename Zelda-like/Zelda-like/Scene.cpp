#include "Scene.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SceneType.h"

Scene::Scene(SceneManager* manager, EntityManager* entityManager, bool isInGame)
    : sceneManager(manager), entityManager(entityManager)
{
    isGameOver = false;

    this->sceneManager = sceneManager;
    this->entityManager = entityManager;

    if (isInGame)
    {
        sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

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

            restartButton = new sf::RectangleShape({ 300.f, 70.f });

            restartButton->setOrigin({
                150.f,
                35.f
                });

            restartButton->setPosition({
                Constants::CENTER_X,
                Constants::CENTER_Y - 20.f
                });

            restartButton->setFillColor(
                sf::Color(60, 60, 60)
            );

            restartButton->setOutlineThickness(3.f);

            restartButton->setOutlineColor(
                sf::Color::White
            );

            restartText = new sf::Text(*font);

            restartText->setString("Restart");
            restartText->setCharacterSize(36);
            restartText->setFillColor(sf::Color::White);

            bounds = restartText->getLocalBounds();

            restartText->setOrigin({
                bounds.position.x + bounds.size.x / 2.f,
                bounds.position.y + bounds.size.y / 2.f
                });

            restartText->setPosition(
                restartButton->getPosition()
            );

            // ---------------- Exit Button ----------------

            exitButton = new sf::RectangleShape({ 300.f, 70.f });

            exitButton->setOrigin({
                150.f,
                35.f
                });

            exitButton->setPosition({
                Constants::CENTER_X,
                Constants::CENTER_Y + 80.f
                });

            exitButton->setFillColor(
                sf::Color(60, 60, 60)
            );

            exitButton->setOutlineThickness(3.f);

            exitButton->setOutlineColor(
                sf::Color::White
            );

            exitText = new sf::Text(*font);

            exitText->setString("Exit");
            exitText->setCharacterSize(36);
            exitText->setFillColor(sf::Color::White);

            bounds = exitText->getLocalBounds();

            exitText->setOrigin({
                bounds.position.x + bounds.size.x / 2.f,
                bounds.position.y + bounds.size.y / 2.f
                });

            exitText->setPosition(
                exitButton->getPosition()
            );
        }
    }
    else
    {
        gameOverOverlay = nullptr;
        gameOverText = nullptr;
        restartButton = nullptr;
        exitButton = nullptr;
    }
}

//Scene::~Scene()
//{
//
//}

bool Scene::CheckGameOver()
{
    Player* player = entityManager->GetPlayer();

    isGameOver = ((player != nullptr && !player->IsActive()) ? true : false);

    return isGameOver;
}

void Scene::HandleGameOverEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    if (!isGameOver)
    {
        return;
    }

    InputManager& input =
        InputManager::GetInstance();

    if (!input.IsLeftMouseClicked())
    {
        return;
    }

    sf::Vector2f mousePosition =
        input.GetMouseClickWorldPosition(window);

    // Restart
    if (restartButton != nullptr &&
        restartButton->getGlobalBounds().contains(mousePosition))
    {
        // 기존 몬스터 제거
        entityManager->ClearMonsters();

        // 기존 Player 제거
        entityManager->DeletePlayer();

        // TODO
        // 새로운 게임을 어떻게 시작할지에 따라
        // 여기서 Player를 다시 생성하거나
        // Title / JobSelect Scene으로 이동

        sceneManager->RequestSceneChange(TITLE);

        return;
    }

    // Exit
    if (exitButton != nullptr &&
        exitButton->getGlobalBounds().contains(mousePosition))
    {
        window.close();

        return;
    }
}

void Scene::UpdateGameOver(float deltaTime, sf::RenderWindow& window)
{
    restartButton->setOutlineColor(sf::Color::White);
    exitButton->setOutlineColor(sf::Color::White);

    InputManager& input = InputManager::GetInstance();

    if (restartButton->getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        restartButton->setOutlineColor(sf::Color::Yellow);
    }
    else if (exitButton->getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        exitButton->setOutlineColor(sf::Color::Yellow);
    }
}


void Scene::RenderGameOver(sf::RenderWindow& window)
{
    if (!isGameOver)
    {
        return;
    }

    // 화면을 어둡게 만드는 반투명 배경
    if (gameOverOverlay != nullptr)
    {
        window.draw(*gameOverOverlay);
    }

    // GAME OVER 텍스트
    if (gameOverText != nullptr)
    {
        window.draw(*gameOverText);
    }

    // Restart 버튼
    if (restartButton != nullptr)
    {
        window.draw(*restartButton);
    }

    if (restartText != nullptr)
    {
        window.draw(*restartText);
    }

    // Exit 버튼
    if (exitButton != nullptr)
    {
        window.draw(*exitButton);
    }

    if (exitText != nullptr)
    {
        window.draw(*exitText);
    }
}