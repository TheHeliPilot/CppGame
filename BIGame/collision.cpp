#include "collision.h"

#include "ECS/collider_component.h"


bool collision::aabb(const SDL_Rect* rec1, const SDL_Rect* rec2)
{
    if(rec1->x + rec1->w >= rec2->x &&
        rec2->x + rec2->w >= rec1->x &&
        rec1->y + rec1->h >= rec2->y &&
        rec2->y + rec2->h >= rec1->y)
    {
        return true;
    }
    
    return false;
}

bool collision::aabb(const collider_component& rec_a, const collider_component& rec_b)
{
    return aabb(&rec_a.collider, &rec_b.collider);
}
