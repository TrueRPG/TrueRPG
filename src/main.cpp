#include "client/window/Window.h"
#include "RTime.h"
#include "Game.h"

// Никто не забыт, ничто не забыто

// D.E.L.E.T.E.D
// summonDestructor3000 
// 2021-2021
// Покойся с миром

int main()
{
    // Создание окна
    auto &window = Window::getInstance(1280, 720, "TRUE RPG");

    Game game;

    // Game timer
    RTime time(0.0f, 0.0f, 0.0f);

    while (window.isOpen())
    {
        game.update(time.getDeltaTime());

        // Обмен содержимым front- и back- буферов.
        // Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
        window.swapBuffers();
        window.pollEvents();
    }
    
    game.destroy();
    window.destroy();

    return 0;
}
