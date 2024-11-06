#include <SDL.h>
#include "game.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

game *game_instance = nullptr;

int main(int argc, char* argv[])
{
    constexpr int max_fps = 60;
    constexpr int frame_delay = 1000 / max_fps;

    bool b = false;
    if (argc > 1) {
        if (strcmp(argv[1], "true") == 0 || strcmp(argv[1], "1") == 0) {
            b = true;
        } else if (strcmp(argv[1], "false") == 0 || strcmp(argv[1], "0") == 0) {
            b = false;
        } else {
            std::cerr << "Invalid argument. Use bool." << '\n';
            return 1;
        }
    }
    
    game_instance = new game();
    game_instance->init("BI Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false, b);

    while (game_instance->running())
    {
        const Uint32 frame_start = SDL_GetTicks();
        
        game_instance->handle_events();
        game_instance->update();
        game_instance->render();

        const int frame_time = SDL_GetTicks() - frame_start;

        if(frame_delay > frame_time)
        {
            SDL_Delay(frame_delay - frame_time);
        }
    }

    game_instance->clean();
    
    return 0;
}