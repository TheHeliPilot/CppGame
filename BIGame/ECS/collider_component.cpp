#include "collider_component.h"

#include "transform_component.h"
#include "../game.h"

collider_component::collider_component()
= default;

collider_component::~collider_component()
= default;

void collider_component::update()
{
    collider.x = static_cast<int>(transform->position.x);
    collider.y = static_cast<int>(transform->position.y);
    collider.w = transform->width * transform->scale;
    collider.h = transform->height * transform->scale;
}


void collider_component::collided(collider_component& collider, const char* tag)
{
    current_collision_tags_.emplace_back(tag);
    current_collision_components_.push_back(collider);
}

void collider_component::init()
{
    if(!entity->has_component<transform_component>())
    {
        entity->add_component<transform_component>();
    }
    transform = &entity->get_component<transform_component>();

    game::colliders.emplace_back(this);
}


