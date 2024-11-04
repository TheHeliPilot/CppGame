#pragma once
#include "game.h"

class game_object
{
public:
    game_object(const char* texture_sheet, int x, int y);
    ~game_object();

    void update();
    void render() const;

private:
    int x_pos_;
    int y_pos_;

    SDL_Texture* obj_texture_;
    SDL_Rect src_rect_, dest_rect_;
};
