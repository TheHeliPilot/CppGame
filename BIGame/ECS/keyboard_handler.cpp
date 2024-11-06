#include "keyboard_handler.h"
#include "../game.h"

keyboard_handler::keyboard_handler()
= default;

void keyboard_handler::init()
{
}

void keyboard_handler::update()
{
    if(game::event.type == SDL_KEYDOWN)
    {
        on_key_event(game::event.key.keysym.sym, true);
    }
    if(game::event.type == SDL_KEYUP)
    {
        on_key_event(game::event.key.keysym.sym, false);
    }
    
    // Clear the list of pressed keys for the new frame
    pressed_keys_.clear();

    // Populate pressed_keys_ based on the current state of keys
    for (SDL_Keycode key : current_keys_)
    {
        pressed_keys_.push_back(key);
    }
}

void keyboard_handler::draw()
{
}

bool keyboard_handler::is_key_pressed(const SDL_Keycode key) const
{
    return current_keys_.find(key) != current_keys_.end();
}

const std::vector<SDL_Keycode>& keyboard_handler::get_pressed_keys() const
{
    return pressed_keys_;
}

void keyboard_handler::on_key_event(const SDL_Keycode key, const bool is_pressed)
{
    if (is_pressed)
    {
        current_keys_.insert(key);
    }
    else
    {
        current_keys_.erase(key);
    }
}