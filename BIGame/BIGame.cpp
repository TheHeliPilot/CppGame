#include <SDL.h>
#include "game.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

game *game_instance = nullptr;

int main(int argc, char* argv[])
{
    const int max_fps = 60;
    const int frame_delay = 1000 / max_fps;

    Uint32 frame_start;
    int frame_time;
    
    game_instance = new game();
    game_instance->init("BI Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

    while (game_instance->running())
    {
        frame_start = SDL_GetTicks();
        
        game_instance->handle_events();
        game_instance->update();
        game_instance->render();

        frame_time = SDL_GetTicks() - frame_start;

        if(frame_delay > frame_time)
        {
            SDL_Delay(frame_delay - frame_time);
        }
    }

    game_instance->clean();
    
    return 0;
}
