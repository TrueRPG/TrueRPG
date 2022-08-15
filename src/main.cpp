#include "pch.h"
#include "utils/GameTimer.h"
#include "client/Engine.h"
#include "Game.h"

// Nobody is forgotten, nothing is forgotten

// D.E.L.E.T.E.D
// summonDestructor3000 
// 2021-2021
// Rest in peace

int main()
{
    // Create a window
    auto &window = Engine::getWindow(1280, 720, "TRUE RPG");

    Game game;

    GameTimer time(0.0f, 0.0f, 0.0f);

    while (window.isOpen())
    {
        game.update(time.getDeltaTime());
        window.swapBuffers();
        window.pollEvents();
    }
    
    game.destroy();
    window.destroy();

    return 0;
}
