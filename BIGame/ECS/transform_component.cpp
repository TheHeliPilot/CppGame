#include "transform_component.h"

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