#include "TitleScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SaveManager.h"
#include "Constants.h"

#include <iostream>
#include <string>

using namespace std;

TitleScene::TitleScene(
    SceneManager* sceneManager,
    EntityManager* entityManager
)
    : Scene(sceneManager, entityManager)
{
    backgroundSprite = nullptr;
    titleText = nullptr;
    startText = nullptr;

    // 배경 이미지 ------------------------------------------------

    sf::Texture* backgroundTexture = ResourceManager::GetInstance().GetTexture("TitleScene");

    if (backgroundTexture == nullptr)
    {
        cerr << "Error : TitleScene texture not found." << endl;
    }
    else
    {
        backgroundSprite = new sf::Sprite(*backgroundTexture);

        const sf::Vector2u textureSize = backgroundTexture->getSize();

        // 배경 이미지를 게임 화면 전체 크기에 맞춤
        backgroundSprite->setScale({
            static_cast<float>(Constants::WINDOW_WIDTH) / static_cast<float>(textureSize.x),
            static_cast<float>(Constants::WINDOW_HEIGHT) / static_cast<float>(textureSize.y)
            });

        backgroundSprite->setPosition({
            0.f,
            0.f
            });
    }

    // ------------------------------------------------------------

    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font == nullptr)
    {
        cerr << "Error : Failed to load font." << endl;
    }
    else
    {
        // 게임 제목
        titleText = new sf::Text(*font);

        titleText->setString("Dungeon Heroes");

        titleText->setCharacterSize(50);

        sf::FloatRect titleBounds = titleText->getLocalBounds();

        // 텍스트 좌표의 기준을 텍스트 박스의 중앙으로 변경
        titleText->setOrigin({
            titleBounds.position.x + titleBounds.size.x / 2.f,
            titleBounds.position.y + titleBounds.size.y / 2.f
            });

        titleText->setPosition({
            Constants::CENTER_X,
            220.f
            });

        // ---------------------------------------------------------

        // 시작 버튼
        startButton.setSize({
            300.f,
            70.f
            });

        startButton.setOrigin({
            startButton.getSize().x / 2.f,
            startButton.getSize().y / 2.f
            });

        startButton.setPosition({
            Constants::CENTER_X,
            430.f
            });

        startButton.setFillColor(sf::Color(70, 70, 70, 180));

        startButton.setOutlineThickness(3.f);

        startButton.setOutlineColor(sf::Color::White);

        // ---------------------------------------------------------

        // 시작 버튼 안의 텍스트
        startText = new sf::Text(*font);

        startText->setString("Press Enter");

        startText->setCharacterSize(30);

        sf::FloatRect startBounds = startText->getLocalBounds();

        startText->setOrigin({
            startBounds.position.x + startBounds.size.x / 2.f,
            startBounds.position.y + startBounds.size.y / 2.f
            });

        startText->setPosition({
            Constants::CENTER_X,
            430.f
            });
    }
}

TitleScene::~TitleScene()
{
    delete backgroundSprite;
    backgroundSprite = nullptr;

    delete titleText;
    titleText = nullptr;

    delete startText;
    startText = nullptr;
}

void TitleScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // Enter 버튼을 눌렀는지 확인
    if (input.IsEnterPressed())
    {
        CheckSaveDate();
    }

    // 버튼을 마우스로 클릭했는지 확인
    if (input.IsLeftMouseClicked())
    {
        CheckSaveDate();
    }
}

void TitleScene::Update(float deltaTime, sf::RenderWindow& window)
{
    startButton.setOutlineColor(sf::Color::White);
    
    InputManager& input = InputManager::GetInstance();

    if (startButton.getGlobalBounds().contains(
        { static_cast<float>(input.GetMouseWorldPosition(window).x),
          static_cast<float>(input.GetMouseWorldPosition(window).y) }))
    {
        startButton.setOutlineColor(sf::Color::Magenta);
    }
}

void TitleScene::Render(sf::RenderWindow& window)
{
    // 배경
    if (backgroundSprite != nullptr)
    {
        window.draw(*backgroundSprite);
    }

    // 게임 제목
    if (titleText != nullptr)
    {
        window.draw(*titleText);
    }

    // 시작 버튼
    window.draw(startButton);

    // 시작 버튼 텍스트
    if (startText != nullptr)
    {
        window.draw(*startText);
    }
}

void TitleScene::CheckSaveDate()
{
    SaveManager& saveManager = SaveManager::GetInstance();
    PlayerSaveData saveData;

    if (saveManager.LoadPlayerSaveData(saveData))
    {

        DataManager& dataManager = DataManager::GetInstance();
        const PlayerData* basicData = dataManager.GetPlayerData(saveData.job);
        const PlayerLevelData* levelData = dataManager.GetPlayerLevelData(saveData.job, saveData.level);

        if (basicData == nullptr || levelData == nullptr)
        {
            return;
        }

        entityManager->CreatePlayer("Player", saveData, { Constants::CENTER_X, Constants::CENTER_Y });

        sceneManager->RequestSceneChange(HOME);
    }
    else
    {
        // 세이브 데이터 없는 경우
        sceneManager->RequestSceneChange(JOB_SELECT);
    }
}