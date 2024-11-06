#pragma once
#include "../ECS/keyboard_handler.h"
#include "../ECS/ecs.h"
#include "../ECS/physics_component.h"
#include "../ECS/collider_component.h"
#include "wall_script.h"
#include "../vector2.h"
#include "../network_handler.h"
#include "../game.h"

class collider_component;

class player_movement final : public component
{
private:
    bool can_interact_ = true;
    
public:
    explicit player_movement(const float speed)
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

        //std::cout << entity->get_component<collider_component>().is_colliding_with_tag("wall");
        
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && can_interact_ && entity->get_component<collider_component>().is_colliding_with_tag("wall"))
        {
            const auto* other = entity->get_component<collider_component>().get_collider_with_tag("wall").entity;
            other->get_component<wall_script>().is_on= !other->get_component<wall_script>().is_on;
            network_handler::send_data(game::connect_socket, network_handler::make_data("wall_switch", network_handler::serialize_bool(other->get_component<wall_script>().is_on)));
            can_interact_ = false;
        }
        else if(!entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && !can_interact_)
        {
            can_interact_ = true;
        }
    }
};
