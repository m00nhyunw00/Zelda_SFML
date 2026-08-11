#include "EndingScene.h"
#include "ResourceManager.h"
#include "Constants.h"

#include <iostream>

using namespace std;

EndingScene::EndingScene(SceneManager* sceneManager,EntityManager* entityManager)
    : Scene(sceneManager, entityManager)
{
    sf::Texture* endingTexture =ResourceManager::GetInstance().GetTexture("Ending");

    if (endingTexture == nullptr)
    {
        cerr << "[EndingScene] Ending texture not found" << endl;

        return;
    }

    endingSprite = new sf::Sprite(*endingTexture);

    const sf::Vector2u textureSize = endingTexture->getSize();

    // 화면 전체를 이미지로 채움
    endingSprite->setScale({
        static_cast<float>(Constants::WINDOW_WIDTH) / textureSize.x,
        static_cast<float>(Constants::WINDOW_HEIGHT) / textureSize.y
        });

    endingSprite->setPosition({
        0.f,
        0.f
        });
}

EndingScene::~EndingScene()
{
    delete endingSprite;
    endingSprite = nullptr;
}

void EndingScene::HandleEvent(
    const sf::Event& event,
    sf::RenderWindow& window
)
{
    (void)event;
    (void)window;
}

void EndingScene::Update(
    float deltaTime,
    sf::RenderWindow& window
)
{
    (void)deltaTime;
    (void)window;
}

void EndingScene::Render(
    sf::RenderWindow& window
)
{
    window.setView(window.getDefaultView());

    if (endingSprite != nullptr)
    {
        window.draw(*endingSprite);
    }
}