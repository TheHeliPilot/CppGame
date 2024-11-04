#pragma once
#include <SDL_render.h>

class texture_manager
{
public:
    static SDL_Texture* load_texture(const char* file_name);
    static void draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst);
};
