#include "button_script.h"
#include "../ECS/sprite_component.h"

void button_script::update()
{
    //std::cout << is_on << '\n';
    entity->get_component<sprite_component>().set_tex(is_on ? on.c_str() : off.c_str());
}

