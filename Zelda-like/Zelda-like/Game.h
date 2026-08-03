#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.h"
#include "InputManager.h"
#include "DataManager.h"
#include "EntityManager.h"

class Game
{
private:
    sf::RenderWindow window;    // 게임 윈도우
    SceneManager sceneManager;
    DataManager dataManager;

public:
    Game();

    void Run();
};