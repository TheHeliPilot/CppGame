#pragma once
#include <utility>

#include "../ECS/ecs.h"

//class sprite_component;

class button_script final : public component
{
public:
button_script(std::string on, std::string off): is_on(false)
{
    this->on = std::move(on);
    this->off = std::move(off);
}

    button_script(std::string on, std::string off, const bool rep): is_on(false)
{
    replicates_ = rep;
    this->on = std::move(on);
    this->off = std::move(off);
}

bool is_on;
    std::string on;
    std::string off;

    void update() override;

private:
    bool replicates_ = true;
    bool last_state_ = false;
};
