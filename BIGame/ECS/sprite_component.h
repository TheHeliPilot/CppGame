#pragma once
#include <SDL_render.h>
#include "ecs.h"

class transform_component;

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
        
        void set_tex(const char* path);

        void init() override;
        void update() override;
        void draw() override;

private:
        transform_component *transform_;
        SDL_Texture *texture_;
        SDL_Rect src_rect_, dest_rect_;
};

