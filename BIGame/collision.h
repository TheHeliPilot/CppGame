#pragma once
#include <SDL_rect.h>

class collider_component;

class collision
{
public:
    static auto aabb(const SDL_Rect* rec1, const SDL_Rect* rec2) -> bool;
    auto aabb(const collider_component& rec_a, const collider_component& rec_b) -> bool;
};
