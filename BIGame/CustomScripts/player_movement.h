#pragma once
#include "../ECS/keyboard_handler.h"
#include "../ECS/ecs.h"
#include "../ECS/physics_component.h"
#include "../ECS/collider_component.h"
#include "button_script.h"
#include "../vector2.h"
#include "../network_handler.h"
#include "../game.h"

class collider_component;

class player_movement final : public component
{
private:
    bool can_interact_ = true;
    class entity* curr_plate_ = nullptr;
    
public:
    explicit player_movement(const float speed)
    {
        this->speed = speed;
    }
    
    float speed;

    void init() override
    {
        curr_plate_ = nullptr;
    }
    
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

        //plate
        if(entity->get_component<collider_component>().is_colliding_with_tag("plate") && curr_plate_ == nullptr)
        {
            curr_plate_ = entity->get_component<collider_component>().get_collider_with_tag("plate").entity;
            if(!curr_plate_->get_component<button_script>().is_on)
            {
                curr_plate_->get_component<button_script>().is_on = true;
                network_handler::send_data(game::connect_socket, network_handler::make_data("plate", network_handler::serialize_bool(curr_plate_->get_component<button_script>().is_on)));
            }
        }
        if(!entity->get_component<collider_component>().is_colliding_with_tag("plate") && curr_plate_ != nullptr)
        {
            curr_plate_->get_component<button_script>().is_on = false;
            network_handler::send_data(game::connect_socket, network_handler::make_data("plate", network_handler::serialize_bool(curr_plate_->get_component<button_script>().is_on)));
            curr_plate_ = nullptr;
        }

        //button
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && can_interact_ && entity->get_component<collider_component>().is_colliding_with_tag("button"))
        {
            const auto* other = entity->get_component<collider_component>().get_collider_with_tag("button").entity;
            other->get_component<button_script>().is_on= !other->get_component<button_script>().is_on;
            network_handler::send_data(game::connect_socket, network_handler::make_data("button_switch", network_handler::serialize_bool(other->get_component<button_script>().is_on)));
            can_interact_ = false;
        }
        else if(!entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && !can_interact_)
        {
            can_interact_ = true;
        }
    }
};
