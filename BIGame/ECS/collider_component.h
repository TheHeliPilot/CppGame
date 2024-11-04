#pragma once

#include "components.h"

class collider_component final : public component
{
public:
    SDL_Rect collider;
    int collision_layer;

    transform_component* transform;

    void init() override
    {
        if(!entity->has_component<transform_component>())
        {
            entity->add_component<transform_component>();
        }
        transform = &entity->get_component<transform_component>();

        game::colliders.push_back(this);
    }

    void update() override
    {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};
