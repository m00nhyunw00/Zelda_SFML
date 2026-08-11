#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "PlayerSaveData.h"

class Game
{
private:
    sf::RenderWindow window;       // 게임 윈도우

    EntityManager entityManager;
    SceneManager sceneManager;      

public:
    Game();

    void InitializeGame();
    void CreatePlayerFromSave(const PlayerSaveData& saveData);
    void Run();
};