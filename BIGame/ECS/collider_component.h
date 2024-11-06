#pragma once
#include <SDL_rect.h>
#include <string>

#include "ecs.h"

class transform_component;

class collider_component : public component
{
private:
    std::vector<std::string> current_collision_tags_;
    std::vector<collider_component> current_collision_components_;
    
public:
    SDL_Rect collider;
    int collision_layer;
    const char* collision_tag;
    bool trigger = false;

    transform_component* transform;

    collider_component();
    explicit collider_component(const char* tag): collider(), collision_layer(0), transform(nullptr) { collision_tag = tag; }

    collider_component(const char* tag, const bool trigger): collider(), collision_layer(0), transform(nullptr)
    {
        collision_tag = tag;
        this->trigger = trigger;
    }

    ~collider_component() override;
    
    virtual void collided(collider_component& collider, const char* tag);

    bool is_colliding_with_tag(const std::string& tag) const {
        //std::cout << current_collisions_.size() << '\n';
        return std::find(current_collision_tags_.begin(), current_collision_tags_.end(), tag) != current_collision_tags_.end();
    }

    collider_component get_collider_with_tag(const std::string& tag) const
    {
        for (const auto& component : current_collision_components_) {
            if (component.collision_tag == tag) {
                return component;
            }
        }
        return collider_component("NO_COMPONENT"); // Return nullopt if no matching collider is found
    }

    void init() override;

    void update() override;

    void late_update() override
    {
        current_collision_tags_.clear();
        current_collision_components_.clear();
    }
};
