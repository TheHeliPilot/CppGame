#include "game_object.h"
#include "texture_manager.h"
#include "game.h"

game_object::game_object(const char* texture_sheet, const int x, const int y)
{
    obj_texture_ = texture_manager::load_texture(texture_sheet);

    x_pos_ = x;
    y_pos_ = y;
}

game_object::~game_object()
= default;

void game_object::update()
{
    src_rect_.h = 16;
    src_rect_.w = 16;
    src_rect_.x = 0;
    src_rect_.y = 0;

    dest_rect_.x = x_pos_;
    dest_rect_.y = y_pos_;
    dest_rect_.w = src_rect_.w * 2;
    dest_rect_.h = src_rect_.h * 2;
}

void game_object::render() const
{
    SDL_RenderCopy(game::renderer, obj_texture_, &src_rect_, &dest_rect_);
}
