#pragma once

#include "components.h"
//#include "../vector2.h"

class transform_component final : public component
{
    
public:
    vector2 position;

    bool replicates = false;

    int height = 16;
    int width = 16;
    int scale = 1;
    
    transform_component()
    {
        position.x = 0.0f;
        position.y = 0.0f;
    }
    transform_component(const int scale)
    {
        position.x = 0.0f;
        position.y = 0.0f;
        this->scale = scale;
    }
    transform_component(const float x, const float y)
    {
        position.x = x;
        position.y = y;
    }
    transform_component(const float x, const float y, const int h, const int w, const int scale)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        this->scale = scale;
    }
    transform_component(const vector2& position)
    {
        this->position = position;
    }
    transform_component(const vector2& position, const int scale)
    {
        this->position = position;
        this->scale = scale;
    }
};
