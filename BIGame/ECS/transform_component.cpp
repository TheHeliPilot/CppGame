#include "transform_component.h"

#include "../game.h"
#include "../network_handler.h"

transform_component::transform_component()
{
    position.x = 0.0f;
    position.y = 0.0f;
}
transform_component::transform_component(const int scale)
{
    position.x = 0.0f;
    position.y = 0.0f;
    this->scale = scale;
}
transform_component::transform_component(const float x, const float y)
{
    position.x = x;
    position.y = y;
}
transform_component::transform_component(const float x, const float y, const int h, const int w, const int scale)
{
    position.x = x;
    position.y = y;
    height = h;
    width = w;
    this->scale = scale;
}
transform_component::transform_component(const vector2& position)
{
    this->position = position;
}
transform_component::transform_component(const vector2& position, const int scale)
{
    this->position = position;
    this->scale = scale;
}

transform_component::transform_component(const vector2& position, const int scale, const float rotation)
{
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
}

vector2 transform_component::up() const
{
    return vector2::rotate_vector({0,1}, rotation);
}

vector2 transform_component::right() const
{
    return vector2::rotate_vector({1,0}, rotation);
}