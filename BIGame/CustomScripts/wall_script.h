#pragma once
#include "../ECS/components.h"

class wall_script final : public component
{
public:
    bool is_on;

    void update() override
    {
        //std::cout << is_on << '\n';
        entity->get_component<sprite_component>().set_tex(is_on ? "assets/textures/player.png" : "assets/textures/wall.png");
    }
};
