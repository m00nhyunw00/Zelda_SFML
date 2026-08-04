#include "Scene.h"

Scene::Scene(SceneManager* manager, Player* player)
    : sceneManager(manager), player(player)
{
    entityManager.SetPlayer(player);
}