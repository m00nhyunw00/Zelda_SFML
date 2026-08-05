#include "HomeScene.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerType.h"
#include "SpriteUtil.h"

#include <iostream>

HomeScene::HomeScene(SceneManager* sceneManager, EntityManager* entitymanager) : Scene(sceneManager, entitymanager)
{
    sf::Font* font = ResourceManager::GetInstance().GetFont("MainFont");

    if (font != nullptr)
    {
        /*titleText = new sf::Text(*font);
        titleText->setString("Select Your Job");
        titleText->setCharacterSize(42);

        sf::FloatRect bounds = titleText->getLocalBounds();
        titleText->setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
            });

        titleText->setPosition({ 640.f, 70.f });*/

    }

    //warriorPanel.setSize({ 220.f, 260.f });
    //warriorPanel.setPosition({ 170.f, 180.f });
    //warriorPanel.setFillColor(sf::Color(80, 80, 80));
    //warriorPanel.setOutlineThickness(3.f);

   
}

HomeScene::~HomeScene()
{
    //delete titleText;

    //titleText = nullptr;
}

void HomeScene::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    (void)event;

    InputManager& input = InputManager::GetInstance();

    // 숫자 키로 직업 선택
    //if (input.IsNum1Pressed())
    //{

    //}


    // 마우스로 직업 선택
    if (input.IsLeftMouseClicked())
    {
        sf::Vector2f mousePosition = input.GetMouseClickWorldPosition(window);

        //if (warriorPanel.getGlobalBounds().contains(mousePosition))
        //{
        //}

    }

    
}

void HomeScene::Update(float deltaTime, sf::RenderWindow& window)
{
    entityManager->Update(deltaTime, window);
    
}

void HomeScene::Render(sf::RenderWindow& window)
{
    Player* player = entityManager->GetPlayer();

    player->Render(window);
}