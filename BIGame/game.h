#pragma once

#include <iostream>
#include <winsock2.h>

#include "SDL.h"
#include "collision.h"
#include "ECS/ecs.h"

class collider_component;
class physics_component;

class game
{
public:
    game();
    ~game();

    void init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen);

    void handle_events();
    void update();
    void render();
    void clean();

    bool running() const { return is_running_; }

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static manager manager_;
    static std::vector<collider_component*> colliders;
    static std::vector<physics_component*> physics_components;

    static WSADATA wsa_data;
    
private:
    bool is_running_;
    bool is_server_;
    SDL_Window *window_;
};
