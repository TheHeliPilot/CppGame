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

void texture_manager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst)
{
    SDL_RenderCopy(game::renderer, tex, &src, &dst);
}
