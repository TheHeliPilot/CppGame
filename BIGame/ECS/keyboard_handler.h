#pragma once

#include "../game.h"
//#include "components.h"

#include <vector>
#include <unordered_set>

class keyboard_handler final : public component
{
public:
    keyboard_handler();
    void init() override;
    void update() override;
    void draw() override;
    bool is_key_pressed(SDL_Keycode key) const;
    const std::vector<SDL_Keycode>& get_pressed_keys() const;

    void on_key_event(SDL_Keycode key, bool is_pressed);
    
private:
    std::unordered_set<SDL_Keycode> current_keys_;
    std::vector<SDL_Keycode> pressed_keys_;
};
