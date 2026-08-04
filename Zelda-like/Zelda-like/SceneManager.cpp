#include "SceneManager.h"

#include "TitleScene.h"
#include "JobSelectScene.h"
//#include "HomeScene.h"
//#include "DungeonScene.h"
//#include "BossScene.h"
//#include "GameOverScene.h"
//#include "EndingScene.h"

SceneManager::SceneManager(Player* player) : player(player)
{
    currentScene = NULL;
    nextSceneType = TITLE;
    sceneChangeRequested = false;
}

SceneManager::~SceneManager()
{
    delete currentScene;
    currentScene = NULL;
}

void SceneManager::Start(SceneType firstScene)
{
    delete currentScene;

    currentScene = CreateScene(firstScene);
}

Scene* SceneManager::CreateScene(SceneType type)
{
    switch (type)
    {
    case TITLE:
        return new TitleScene(this, player);

    case JOB_SELECT:
        return new JobSelectScene(this, player);

    //case HOME:
    //    return new HomeScene(this);

    //case DUNGEON:
    //    return new DungeonScene(this);

    //case BOSS:
    //    return new BossScene(this);

    //case GAME_OVER:
    //    return new GameOverScene(this);

    //case ENDING:
    //    return new EndingScene(this);

    default:
        return NULL;
    }
}

void SceneManager::RequestSceneChange(SceneType type)
{
    nextSceneType = type;
    sceneChangeRequested = true;
}

void SceneManager::ApplySceneChange()
{
    if (sceneChangeRequested == false)
    {
        return;
    }

    delete currentScene;
    currentScene = NULL;

    currentScene = CreateScene(nextSceneType);

    sceneChangeRequested = false;
}

void SceneManager::HandleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (currentScene != NULL)
    {
        currentScene->HandleEvent(event, window);
    }
}

void SceneManager::Update(float deltaTime, sf::RenderWindow& window)
{
    if (currentScene != NULL)
    {
        currentScene->Update(deltaTime, window);
    }
}

void SceneManager::Render(sf::RenderWindow& window)
{
    if (currentScene != NULL)
    {
        currentScene->Render(window);
    }
}