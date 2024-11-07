#pragma once
#include "ecs.h"
#include "../vector2.h"

class vector2;

class transform_component final : public component
{
    
public:
    vector2 position;
    float rotation;

    bool replicates = false;

    int height = 16;
    int width = 16;
    int scale = 1;

    transform_component();
    transform_component(const int scale);
    transform_component(const float x, const float y);
    transform_component(const float x, const float y, const int h, const int w, const int scale);
    transform_component(const vector2& position);
    transform_component(const vector2& position, const int scale);
    transform_component(const vector2& position, const int scale, float rotation);
    
    vector2 up() const;
    vector2 right() const;
};
