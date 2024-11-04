#pragma once

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "transform_component.h"
#include "../game.h"


class physics_component final : public component {
public:
    vector2 last_velocity;
    vector2 last_pos;
    vector2 late_pos;
    
    explicit physics_component(const float drag)
    {
        drag_ = drag;
    }
    
    void init() override
    {
        transform_ = &entity->get_component<transform_component>();
        game::physics_components.push_back(this);
    }
    
    void update() override
    {
        last_velocity = velocity_;
        late_pos = last_pos;
        last_pos = transform_->position;
        
        transform_->position += velocity_;
        velocity_ = velocity_ * (1 - drag_);
    }

    void add_force(const vector2 force)
    {
        velocity_ += force;
    }
    void set_velocity(const vector2 velocity)
    {
        velocity_ = velocity;
    }
    
private:
    vector2 velocity_ = vector2(0, 0);
    float drag_ = 0; //0-1
    transform_component* transform_ = nullptr;
    
};

#endif