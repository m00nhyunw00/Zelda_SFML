#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.h"
#include "InputManager.h"
#include "EntityManager.h"

class Game
{
private:
    sf::RenderWindow window;       // 게임 윈도우

    EntityManager entityManager;
    SceneManager sceneManager;      

public:
    Game();

    void Run();
};