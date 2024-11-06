#include "sprite_component.h"

#include "transform_component.h"
#include "../texture_manager.h"
        
void sprite_component::set_tex(const char* path)
{
    texture_ = texture_manager::load_texture(path);
}

void sprite_component::init()
{
    transform_ = &entity->get_component<transform_component>();
                
    src_rect_.x = src_rect_.y = 0;
    src_rect_.w = transform_->width;
    src_rect_.h = transform_->height;
}
void sprite_component::update()
{
    dest_rect_.x = static_cast<int>(transform_->position.x);
    dest_rect_.y = static_cast<int>(transform_->position.y);

    dest_rect_.w = transform_->width * transform_->scale;
    dest_rect_.h = transform_->width * transform_->scale;
}
void sprite_component::draw()
{
    texture_manager::draw(texture_, src_rect_, dest_rect_);
}