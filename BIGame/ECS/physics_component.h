#pragma once
#include "ecs.h"
#include "../vector2.h"

class transform_component;

class physics_component final : public component {
public:
    vector2 last_velocity;
    vector2 last_pos;
    vector2 late_pos;
    
    explicit physics_component(const float drag);
    
    void init() override;
    
    void update() override;

    void add_force(const vector2 force);
    void set_velocity(const vector2 velocity);
    void set_pos(const vector2 pos) const;
    
private:
    vector2 velocity_ = vector2(0, 0);
    float drag_ = 0; //0-1
    transform_component* transform_ = nullptr;
    
};
