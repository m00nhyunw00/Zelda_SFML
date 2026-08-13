#include "Game.h"
#include <Windows.h>
#include <cstdlib>
#include <ctime>

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(static_cast<unsigned int>(time(nullptr)));

    Game game;

    game.Run();

    return 0;
}