#include "JobSelectScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"
#include "Constants.h"

#include <iostream>

JobSelectScene::JobSelectScene(SceneManager* sceneManager, EntityManager* entitymanager) : Scene(sceneManager, entitymanager)
{
    selectedJob = PlayerType::NONE_PLAYER;

    titleText = nullptr;
    guideText = nullptr;

    warriorSprite = nullptr;
    archerSprite = nullptr;
    mageSprite = nullptr;
    mageStaffSprite = nullptr;

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
        guideText->setString("Press 1, 2, 3");
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

    InitializeJobSprites();
}

JobSelectScene::~JobSelectScene()
{
    delete titleText;
    delete guideText;

    delete warriorText;
    delete archerText;
    delete mageText;

    delete warriorSprite;
    delete archerSprite;
    delete mageSprite;
    delete mageStaffSprite;

    titleText = nullptr;
    guideText = nullptr;

    warriorText = nullptr;
    archerText = nullptr;
    mageText = nullptr;

    warriorSprite = nullptr;
    archerSprite = nullptr;
    mageSprite = nullptr;
    mageStaffSprite = nullptr;
}

void JobSelectScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // 숫자 키로 직업 선택
    if (input.IsNum1Pressed())
    {
        selectedJob = PlayerType::WARRIOR;;
    }
    else if (input.IsNum2Pressed())
    {
        selectedJob = PlayerType::ARCHER;;
    }
    else if (input.IsNum3Pressed())
    {
        selectedJob = PlayerType::MAGE;;
    }

    // 마우스로 직업 선택
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

    if (selectedJob != PlayerType::NONE_PLAYER)
    {
        entityManager->CreatePlayer(
            "Player",
            selectedJob,
            { Constants::CENTER_X, Constants::CENTER_Y }
        );

        entityManager->PrintPlayerInfo();

        sceneManager->RequestSceneChange(HOME);
        return;
    }
}

void JobSelectScene::InitializeJobSprites()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance();

    // 전사 대표 이미지 -------------------------------------------------------

    sf::Texture* warriorTexture = resourceManager.GetTexture("Warrior");

    if (warriorTexture != nullptr)
    {
        warriorSprite = new sf::Sprite(
            SpriteUtil::CreateSprite(*warriorTexture, SpriteUtil::GetFrameRect(*warriorTexture, 3, 6, 0, 0), { 4.f, 4.f })
        );

        SpriteUtil::SetSpriteOriginToCenter(warriorSprite);

        warriorSprite->setPosition(
            {
            warriorPanel.getPosition().x + warriorPanel.getSize().x / 2.f - 15.f,
            warriorPanel.getPosition().y + warriorPanel.getSize().y / 2.f
            }
        );
    }

    // 궁수 대표 이미지 -------------------------------------------------------

    sf::Texture* archerTexture = resourceManager.GetTexture("Archer");

    if (archerTexture != nullptr)
    {
        archerSprite = new sf::Sprite(
            SpriteUtil::CreateSprite(*archerTexture, SpriteUtil::GetFrameRect(*archerTexture, 3, 6, 1, 0), { 4.f, 4.f })
        );

        SpriteUtil::SetSpriteOriginToCenter(archerSprite);

        archerSprite->setPosition(
            {
            archerPanel.getPosition().x + archerPanel.getSize().x / 2.f,
            archerPanel.getPosition().y + archerPanel.getSize().y / 2.f
            }
        );
    }

    // 마법사 몸체 -------------------------------------------------------

    sf::Texture* idleTexture = resourceManager.GetTexture("Player_Idle");

    if (idleTexture != nullptr)
    {
        mageSprite = new sf::Sprite(
            SpriteUtil::CreateSprite(*idleTexture, SpriteUtil::GetFrameRect(*idleTexture, 3, 4, 0, 0), { 4.f, 4.f })
        );

        SpriteUtil::SetSpriteOriginToCenter(mageSprite);

        mageSprite->setPosition(
            {
            magePanel.getPosition().x + magePanel.getSize().x / 2.f,
            magePanel.getPosition().y + magePanel.getSize().y / 2.f
            }
        );
    }

    // 마법사 지팡이 -------------------------------------------------------

    sf::Texture* staffTexture = resourceManager.GetTexture("Staff_3");

    if (staffTexture != nullptr && mageSprite != nullptr)
    {
        mageStaffSprite = new sf::Sprite(
            SpriteUtil::CreateSprite(*staffTexture, SpriteUtil::GetFrameRect(*staffTexture, 1, 1, 0, 0), { 4.f, 4.f })
        );

        sf::FloatRect bounds = mageStaffSprite->getLocalBounds();

        // 지팡이의 아래쪽 중앙을 기준점으로 잡음
        mageStaffSprite->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y
            });

        mageStaffSprite->setPosition({
            mageSprite->getPosition().x + 35.5f,
            mageSprite->getPosition().y + 30.f
            });

        // 지팡이 회전
        mageStaffSprite->setRotation(sf::degrees(-20.f));
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
    // 패널 배경
    window.draw(warriorPanel);
    window.draw(archerPanel);
    window.draw(magePanel);

    // 직업 대표 이미지
    if (warriorSprite != nullptr)
    {
        window.draw(*warriorSprite);
    }

    if (archerSprite != nullptr)
    {
        window.draw(*archerSprite);
    }

    if (mageSprite != nullptr)
    {
        window.draw(*mageSprite);
    }

    // 마법사 몸체 위에 지팡이를 그림
    if (mageStaffSprite != nullptr)
    {
        window.draw(*mageStaffSprite);
    }

    // 텍스트
    if (titleText != nullptr)
    {
        window.draw(*titleText);
    }

    if (guideText != nullptr)
    {
        window.draw(*guideText);
    }

    if (warriorText != nullptr)
    {
        window.draw(*warriorText);
    }

    if (archerText != nullptr)
    {
        window.draw(*archerText);
    }

    if (mageText != nullptr)
    {
        window.draw(*mageText);
    }
}