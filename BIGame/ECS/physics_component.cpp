#include "physics_component.h"
#include "transform_component.h"
#include "../game.h"

physics_component::physics_component(const float drag)
{
    drag_ = drag;
}
    
void physics_component::init()
{
    transform_ = &entity->get_component<transform_component>();
    game::physics_components.emplace_back(this);
}
    
void physics_component::update()
{
    last_velocity = velocity_;
    late_pos = last_pos;
    last_pos = transform_->position;
        
    transform_->position += velocity_;
    velocity_ = velocity_ * (1 - drag_);
}

void physics_component::add_force(const vector2 force)
{
    velocity_ += force;
}
void physics_component::set_velocity(const vector2 velocity)
{
    velocity_ = velocity;
}
void physics_component::set_pos(const vector2 pos) const
{
    transform_->position = pos;
}