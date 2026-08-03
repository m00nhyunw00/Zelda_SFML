#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.h"

class Game
{
private:
    sf::RenderWindow window;
    SceneManager sceneManager;

public:
    Game();

    void Run();
};