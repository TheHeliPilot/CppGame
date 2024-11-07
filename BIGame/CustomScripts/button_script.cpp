#include "button_script.h"
#include "../ECS/sprite_component.h"
#include "../network_handler.h"
#include "../game.h"

void button_script::update()
{
    //std::cout << is_on << '\n';
    entity->get_component<sprite_component>().set_tex(is_on ? on.c_str() : off.c_str());

    if(last_state_ != is_on && replicates_)
        network_handler::send_data(game::connect_socket, network_handler::make_data("button_switch", network_handler::serialize_bool(is_on), entity->get_id()));
    last_state_ = is_on;
}

