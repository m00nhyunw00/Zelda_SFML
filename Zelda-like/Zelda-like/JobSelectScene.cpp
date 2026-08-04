#include "JobSelectScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"

JobSelectScene::JobSelectScene(SceneManager* manager, Player* player) : Scene(manager, player)
{
    selectedJob = PlayerType::NONE_PLAYER;

    titleText = nullptr;
    guideText = nullptr;

    warriorText = nullptr;
    archerText = nullptr;
    mageText = nullptr;

    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font != nullptr)
    {
        titleText = new sf::Text(*font);
        titleText->setString("Select Your Job");
        titleText->setCharacterSize(42);

        sf::FloatRect bounds = titleText->getLocalBounds();
        titleText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        titleText->setPosition({ 640.f, 70.f });

        guideText = new sf::Text(*font);
        guideText->setString("Press 1, 2 or 3");
        guideText->setCharacterSize(24);

        bounds = guideText->getLocalBounds();
        guideText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        guideText->setPosition({ 640.f, 650.f });

        warriorText = new sf::Text(*font);
        warriorText->setString("1. Warrior");
        warriorText->setCharacterSize(30);
        warriorText->setPosition({ 215.f, 500.f });

        archerText = new sf::Text(*font);
        archerText->setString("2. Archer");
        archerText->setCharacterSize(30);
        archerText->setPosition({ 565.f, 500.f });

        mageText = new sf::Text(*font);
        mageText->setString("3. Mage");
        mageText->setCharacterSize(30);
        mageText->setPosition({ 935.f, 500.f });
    }

    warriorPanel.setSize({ 220.f, 260.f });
    warriorPanel.setPosition({ 170.f, 180.f });
    warriorPanel.setFillColor(sf::Color(80, 80, 80));
    warriorPanel.setOutlineThickness(3.f);

    archerPanel.setSize({ 220.f, 260.f });
    archerPanel.setPosition({ 530.f, 180.f });
    archerPanel.setFillColor(sf::Color(80, 80, 80));
    archerPanel.setOutlineThickness(3.f);

    magePanel.setSize({ 220.f, 260.f });
    magePanel.setPosition({ 890.f, 180.f });
    magePanel.setFillColor(sf::Color(80, 80, 80));
    magePanel.setOutlineThickness(3.f);
}

JobSelectScene::~JobSelectScene()
{
}

void JobSelectScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input =
        InputManager::GetInstance();

    // 숫자 키로 직업 선택
    if (input.IsNum1Pressed())
    {
        sceneManager->RequestSceneChange(HOME);
        selectedJob = PlayerType::WARRIOR;;
    }
    else if (input.IsNum2Pressed())
    {
        sceneManager->RequestSceneChange(HOME);
        selectedJob = PlayerType::ARCHER;;
    }
    else if (input.IsNum3Pressed())
    {
        sceneManager->RequestSceneChange(HOME);
        selectedJob = PlayerType::MAGE;;
    }

    // 마우스로 직업 카드 선택
    if (input.IsLeftMouseClicked())
    {
        sf::Vector2f mousePosition =
            input.GetMouseClickWorldPosition(window);

        if (warriorPanel.getGlobalBounds().contains(mousePosition))
        {
            selectedJob = PlayerType::WARRIOR;
        }
        else if (archerPanel.getGlobalBounds().contains(mousePosition))
        {
            selectedJob = PlayerType::ARCHER;;
        }
        else if (magePanel.getGlobalBounds().contains(mousePosition))
        {
            selectedJob = PlayerType::MAGE;
        }
    }

    if (selectedJob != NONE_PLAYER)
    {
        PlayerType job = PlayerType::NONE_PLAYER;

        switch (selectedJob)
        {
        case 1:
            job = PlayerType::WARRIOR;
            break;

        case 2:
            job = PlayerType::ARCHER;
            break;

        case 3:
            job = PlayerType::MAGE;
            break;
        }

        player->SetJob(job);
        sceneManager->RequestSceneChange(HOME);
    }
}

void JobSelectScene::Update(float deltaTime, sf::RenderWindow& window)
{
    warriorPanel.setOutlineColor(sf::Color::White);
    archerPanel.setOutlineColor(sf::Color::White);
    magePanel.setOutlineColor(sf::Color::White);

    InputManager& input = InputManager::GetInstance();

    if (warriorPanel.getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        warriorPanel.setOutlineColor(sf::Color::Yellow);
    }
    if (archerPanel.getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        archerPanel.setOutlineColor(sf::Color::Yellow);
    }
    if (magePanel.getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        magePanel.setOutlineColor(sf::Color::Yellow);
    }
}

void JobSelectScene::Render(sf::RenderWindow& window)
{
    window.draw(warriorPanel);
    window.draw(archerPanel);
    window.draw(magePanel);

    if (titleText != nullptr)
        window.draw(*titleText);

    if (guideText != nullptr)
        window.draw(*guideText);

    if (warriorText != nullptr)
        window.draw(*warriorText);

    if (archerText != nullptr)
        window.draw(*archerText);

    if (mageText != nullptr)
        window.draw(*mageText);
}