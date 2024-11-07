#pragma once
#include <SDL_render.h>
#include "ecs.h"

class transform_component;

class sprite_component final : public component
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

        int get_layer() const { return layer_; }
        int get_order_in_layer() const { return order_in_layer_; }

        void set_layer(const int layer) { layer_ = layer; }
        void set_order_in_layer(const int order) { order_in_layer_ = order; }
        
private:
        transform_component *transform_;
        SDL_Texture *texture_;
        SDL_Rect src_rect_, dest_rect_;
        
        int layer_ = 0; // Default layer
        int order_in_layer_ = 0; // Default order
};