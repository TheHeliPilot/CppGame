#pragma once
#include "../ECS/keyboard_handler.h"
#include "../ECS/ecs.h"
#include "../ECS/physics_component.h"
#include "../ECS/collider_component.h"
#include "button_script.h"
#include "../vector2.h"
#include "../network_handler.h"
#include "../game.h"
#include "../ECS/sprite_component.h"
#include "../ECS/transform_component.h"

class collider_component;

class player_movement final : public component
{
private:
    bool can_interact_ = true;
    bool can_shoot_ = true;
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
        

        //plate
        if(entity->get_component<collider_component>().is_colliding_with_tag("plate") && curr_plate_ == nullptr)
        {
            curr_plate_ = entity->get_component<collider_component>().get_collider_with_tag("plate").entity;
            if(!curr_plate_->get_component<button_script>().is_on)
            {
                curr_plate_->get_component<button_script>().is_on = true;
                //network_handler::send_data(game::connect_socket, network_handler::make_data("plate", network_handler::serialize_bool(curr_plate_->get_component<button_script>().is_on)));
            }
        }
        if(!entity->get_component<collider_component>().is_colliding_with_tag("plate") && curr_plate_ != nullptr)
        {
            curr_plate_->get_component<button_script>().is_on = false;
            //network_handler::send_data(game::connect_socket, network_handler::make_data("plate", network_handler::serialize_bool(curr_plate_->get_component<button_script>().is_on)));
            curr_plate_ = nullptr;
        }

        //button
        if(entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && can_interact_ && entity->get_component<collider_component>().is_colliding_with_tag("button"))
        {
            const auto* other = entity->get_component<collider_component>().get_collider_with_tag("button").entity;
            other->get_component<button_script>().is_on= !other->get_component<button_script>().is_on;
            can_interact_ = false;
        }
        else if(!entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && !can_interact_)
        {
            can_interact_ = true;
        }

        //arrow TODO network spawning + prefabs
        //nemozem lebo nemam prefabs a tym padom spawnovanie objektov cez network sa neda kedze nedostanem referenciu
        //na spawnuty objekt (ako client poslem len request) a tym padom viem spawnut iba holu entity (teda s transformom kedze
        //to je zaklad). Na to aby som mohol pridat dalsie komponenty by som musel mat referenciu co nemam, alebo teda prefab ze
        //predrobeny objekt so vsetkymi potrebnymi componentami a nastaveniami ktory by bol na vsetkych clientoch a proste by som ho mohol spawnut
        
       //try {
       //    if (entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && can_shoot_) {
       //        can_shoot_ = false;
       //        //std::cout << game::is_server_ << '\n';
       //
       //        class entity* arrow = asset_manager::instantiate(entity->get_component<transform_component>().position, 1, true);
       //
       //        //std::cout << "2\n";
       //
       //        if (arrow) {
       //            try {
       //                //std::cout << "Attempting to add sprite_component to entity ID " << arrow->get_id() << '\n';
       //                arrow->add_component<sprite_component>("assets/textures/arrow.png");
       //                //arrow->get_component<sprite_component>().set_layer(-1);
       //                
       //            } catch (const std::exception &ex) {
       //                std::cerr << "Error when adding component: " << ex.what() << '\n';
       //            }
       //        }
       //
       //        //std::cout << "3\n";
       //    } else if (!entity->get_component<keyboard_handler>().is_key_pressed(SDLK_SPACE) && !can_shoot_) {
       //        can_shoot_ = true;
       //    }
    
       //    //std::cout << "4\n";
       //} catch (const std::exception &ex) {
       //    std::cerr << "Exception caught: " << ex.what() << '\n';
       //} catch (...) {
       //    std::cerr << "Unknown error caught.\n";
       //}
    }
};
