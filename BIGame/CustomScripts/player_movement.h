#pragma once
#include "../ECS/keyboard_handler.h"
#include "../ECS/components.h"

class player_movement final : public component
{
public:
    explicit player_movement(float speed)
    {
        this->speed = speed;
    }
    
    float speed;

    void update() override
    {
        vector2 input;
        input.zero();
        
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_w))
        {
            input.y -= 1;
        }
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_s))
        {
            input.y += 1;
        }
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_a))
        {
            input.x -= 1;
        }
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_d))
        {
            input.x += 1;
        }

        entity->get_component<physics_component>().add_force(input * speed);
    }
};
