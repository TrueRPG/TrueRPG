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
    auto &window = Engine::getWindow("TRUE RPG");

    auto &graphicsContext = Engine::getGraphicsContext();
    graphicsContext.init();

//    Game game;
//
//    GameTimer time;
//
//    while (window.isOpen())
//    {
//        game.update(time.getDeltaTime());
//        graphicsContext.swapBuffers();
//        window.pollEvents();
//    }
//
//    game.destroy();
    graphicsContext.destroy();
    window.destroy();

    return 0;
}
