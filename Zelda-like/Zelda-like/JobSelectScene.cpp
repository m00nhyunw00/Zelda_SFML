#include "JobSelectScene.h"
#include "SceneManager.h"

JobSelectScene::JobSelectScene(SceneManager* manager)
{
    sceneManager = manager;
    selectedJob = 0;
}

JobSelectScene::~JobSelectScene()
{
}

void JobSelectScene::HandleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        const sf::Event::KeyPressed* keyEvent =
            event.getIf<sf::Event::KeyPressed>();

        if (keyEvent == NULL)
        {
            return;
        }

        if (keyEvent->scancode == sf::Keyboard::Scancode::Num1)
        {
            selectedJob = 1; // 전사
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Num2)
        {
            selectedJob = 2; // 궁수
        }
        else if (keyEvent->scancode == sf::Keyboard::Scancode::Num3)
        {
            selectedJob = 3; // 마법사
        }

        if (selectedJob != 0)
        {
            sceneManager->RequestSceneChange(HOME);
        }
    }
}

void JobSelectScene::Update(float deltaTime)
{
}

void JobSelectScene::Render(sf::RenderWindow& window)
{
    // 전사, 궁수, 마법사 이미지와 설명 출력
}