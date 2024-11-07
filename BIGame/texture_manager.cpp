#include "texture_manager.h"
#include "game.h"

#include <SDL_image.h>

SDL_Texture* texture_manager::load_texture(const char* file_name)
{
    SDL_Surface* temp_surf = IMG_Load(file_name);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(game::renderer, temp_surf);
    SDL_FreeSurface(temp_surf);

    return tex;
}

void texture_manager::draw(SDL_Texture* tex, const SDL_Rect src, const SDL_Rect dst, const double angle, const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE)
{
    SDL_RenderCopyEx(game::renderer, tex, &src, &dst, angle, center, flip);
}
