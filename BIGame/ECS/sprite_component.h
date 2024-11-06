#pragma once

#include "components.h"
#include "../texture_manager.h"

class sprite_component : public component
{
public:
        sprite_component() = default;

        explicit sprite_component(const char* path)
        {
                set_tex(path);
        }

        ~sprite_component() override
        {
                SDL_DestroyTexture(texture_);
        }
        
        void set_tex(const char* path)
        {
                texture_ = texture_manager::load_texture(path);
        }

        void init() override
        {
                transform_ = &entity->get_component<transform_component>();
                
                src_rect_.x = src_rect_.y = 0;
                src_rect_.w = transform_->width;
                src_rect_.h = transform_->height;
        }
        void update() override
        {
                dest_rect_.x = static_cast<int>(transform_->position.x);
                dest_rect_.y = static_cast<int>(transform_->position.y);

                dest_rect_.w = transform_->width * transform_->scale;
                dest_rect_.h = transform_->width * transform_->scale;
        }
        void draw() override
        {
                texture_manager::draw(texture_, src_rect_, dest_rect_);
        }

private:
        transform_component *transform_;
        SDL_Texture *texture_;
        SDL_Rect src_rect_, dest_rect_;
};

