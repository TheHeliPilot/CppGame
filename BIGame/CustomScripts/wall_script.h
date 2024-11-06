#pragma once
#include "../ECS/ecs.h"

//class sprite_component;

class wall_script final : public component
{
public:
    bool is_on;

    void update() override;
};
