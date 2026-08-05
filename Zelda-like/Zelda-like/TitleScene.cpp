#include "TitleScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Constants.h"

#include <iostream>
#include <string>

using namespace std;

TitleScene::TitleScene(SceneManager* sceneManager, EntityManager* entityManager) : Scene(sceneManager, entityManager)
{
    titleText = nullptr;
    startText = nullptr;

    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font == nullptr)
    {
        cerr << "Error : Failed to load font." << endl;
    }
    else
    {
        // 게임 제목
        titleText = new sf::Text(*font);
        titleText->setString("ZELDA-LIKE RPG");
        titleText->setCharacterSize(50);

        sf::FloatRect titleBounds = titleText->getLocalBounds();
        // 텍스트 좌표의 기준을 텍스트 박스의 중앙으로 변경
        titleText->setOrigin({
            titleBounds.position.x + titleBounds.size.x / 2.f,
            titleBounds.position.y + titleBounds.size.y / 2.f
            });
        titleText->setPosition({ Constants::CENTER_X, 220.f });

        // ------------------------------------------------------------------------------

        // 시작 버튼
        startButton.setSize({ 300.f, 70.f });
        // 사각형 좌표의 기준을 사각형의 중앙으로 변경
        startButton.setOrigin({
            startButton.getSize().x / 2.f,
            startButton.getSize().y / 2.f
            });
        startButton.setPosition({ Constants::CENTER_X, 430.f });
        startButton.setFillColor(sf::Color(70, 70, 70));
        startButton.setOutlineThickness(3.f);
        startButton.setOutlineColor(sf::Color::White);

        // ------------------------------------------------------------------------------

        // 시작 버튼 안의 텍스트
        startText = new sf::Text(*font);
        startText->setString("Press Enter");
        startText->setCharacterSize(30);

        sf::FloatRect startBounds = startText->getLocalBounds();
        // 텍스트 좌표의 기준을 텍스트 박스의 중앙으로 변경
        startText->setOrigin({
            startBounds.position.x + startBounds.size.x / 2.f,
            startBounds.position.y + startBounds.size.y / 2.f
            });
        startText->setPosition({ Constants::CENTER_X, 430.f });
    }
}

TitleScene::~TitleScene()
{
    delete titleText;
    delete startText;

    titleText = NULL;
    startText = NULL;
}

void TitleScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // Enter 버튼을 눌렀는지 확인
    if (input.IsEnterPressed())
    {
        sceneManager->RequestSceneChange(JOB_SELECT);
        return;
    }

    // 버튼을 마우스로 클릭했는지 확인
    if (input.IsLeftMouseClicked())
    {
        sf::Vector2f mousePos = input.GetMouseClickWorldPosition(window);

        if (startButton.getGlobalBounds().contains(mousePos))
        {
            sceneManager->RequestSceneChange(JOB_SELECT);
        }
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
        startButton.setOutlineColor(sf::Color::Yellow);
    }
}

void TitleScene::Render(sf::RenderWindow& window)
{
    if (titleText != NULL)
    {
        window.draw(*titleText);
    }

    window.draw(startButton);

    if (startText != NULL)
    {
        window.draw(*startText);
    }
}