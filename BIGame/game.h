#pragma once

#include <iostream>
#include <winsock2.h>

#include "SDL.h"
#include "collision.h"
#include "ECS/asset_manager.h"
#include "ECS/ecs.h"
//#include "ECS/transform_component.h"

class collider_component;
class physics_component;
//class transform_component;
class vector2;

class game
{
public:
    // ReSharper disable once CppInconsistentNaming
    bool is_server_;

    game();
    ~game();

    void init(const char* title, int x_pos, int y_pos, int width, int height, bool fullscreen, bool is_server);

    void handle_events();
    void update();
    void render();
    void clean();

    bool running() const { return is_running_; }

    static SDL_Renderer *renderer;
    static SDL_Event event;
    static manager manager_;
    static asset_manager* asset_manager;
    static vector2 mouse_pos;
    static std::vector<collider_component*> colliders;
    static std::vector<physics_component*> physics_components;
    //static std::vector<transform_component*> transform_components;

    static WSADATA wsa_data;

private:
    bool is_running_;
    SDL_Window *window_;
};
