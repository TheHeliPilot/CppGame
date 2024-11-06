#include "collider_component.h"

collider_component::collider_component()
= default;

collider_component::~collider_component()
= default;

void collider_component::collided(collider_component& collider, const char* tag)
{
    current_collision_tags_.emplace_back(tag);
    current_collision_components_.push_back(collider);
}
