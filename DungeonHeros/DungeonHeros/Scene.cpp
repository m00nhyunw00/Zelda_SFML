#include "Scene.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SceneType.h"

Scene::Scene(SceneManager* manager, EntityManager* entityManager)
    : sceneManager(manager), entityManager(entityManager)
{

}

//Scene::~Scene()
//{
//
//}