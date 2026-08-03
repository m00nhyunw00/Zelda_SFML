#include "TitleScene.h"
#include "SceneManager.h"

TitleScene::TitleScene(SceneManager* manager)
{
    sceneManager = manager;

    titleText = NULL;
    startText = NULL;

    if (font.openFromFile("Assets/Fonts/font.ttf"))
    {
        titleText = new sf::Text(font);
        titleText->setString("ZELDA-LIKE RPG");
        titleText->setCharacterSize(50);
        titleText->setPosition(sf::Vector2f(220.f, 180.f));

        startText = new sf::Text(font);
        startText->setString("Press Enter");
        startText->setCharacterSize(30);
        startText->setPosition(sf::Vector2f(300.f, 350.f));
    }
}

TitleScene::~TitleScene()
{
    delete titleText;
    delete startText;

    titleText = NULL;
    startText = NULL;
}

void TitleScene::HandleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::KeyPressed>())
    {
        const sf::Event::KeyPressed* keyEvent =
            event.getIf<sf::Event::KeyPressed>();

        if (keyEvent != NULL)
        {
            if (keyEvent->scancode == sf::Keyboard::Scancode::Enter)
            {
                sceneManager->RequestSceneChange(JOB_SELECT);
            }
        }
    }
}

void TitleScene::Update(float deltaTime)
{
    // 타이틀 화면에서 별도의 로직이 없다면 비워둔다.
}

void TitleScene::Render(sf::RenderWindow& window)
{
    if (titleText != NULL)
    {
        window.draw(*titleText);
    }

    if (startText != NULL)
    {
        window.draw(*startText);
    }
}