#include "client/window/Window.h"
#include "utils/GameTimer.h"
#include "Game.h"

// Nobody is forgotten, nothing is forgotten

// D.E.L.E.T.E.D
// summonDestructor3000 
// 2021-2021
// Rest in peace

int main()
{
    // Create a window
    auto &window = Window::getInstance(1280, 720, "TRUE RPG");

    Game game;

    GameTimer time(0.0f, 0.0f, 0.0f);

	constexpr float S_PER_TICK = 1.0f / 60.0f;
	float tickDelta = 0;
    while (window.isOpen())
    {
		float deltaTime = time.getDeltaTime();
		tickDelta += deltaTime / S_PER_TICK;

		while (tickDelta >= 1.0)
		{
			game.fixedUpdate();
			tickDelta--;
		}

        game.update(deltaTime);
        window.swapBuffers();
        window.pollEvents();
    }
    
    game.destroy();
    window.destroy();

    return 0;
}
