#include "wall_script.h"
#include "../ECS/sprite_component.h"

void wall_script::update()
{
    //std::cout << is_on << '\n';
    entity->get_component<sprite_component>().set_tex(is_on ? "assets/textures/player.png" : "assets/textures/wall.png");
}

