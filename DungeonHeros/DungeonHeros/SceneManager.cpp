#include "SceneManager.h"

#include "TitleScene.h"
#include "JobSelectScene.h"
#include "HomeScene.h"
#include "DungeonScene.h"
#include "BossScene.h"
//#include "GameOverScene.h"

SceneManager::SceneManager(EntityManager* entityManager)
    : entityManager(entityManager)
{
    currentScene = nullptr;
    nextSceneType = TITLE;      // 시작 화면은 TITLE로 설정
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
        return new TitleScene(this, entityManager);

    case JOB_SELECT:
        return new JobSelectScene(this, entityManager);

    case HOME:
        return new HomeScene(this, entityManager);

    case DUNGEON:
        return new DungeonScene(this, entityManager);

    case BOSS:
        return new BossScene(this, entityManager);

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