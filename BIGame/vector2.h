#pragma once

#include <iostream>

class vector2
{
public:
    float x;
    float y;

    vector2();
    vector2(const float x, const float y);

    vector2& add(const vector2 vec);
    vector2& substract(const vector2 vec);
    vector2& multiply(const vector2 vec);
    vector2& divide(const vector2 vec);

    friend vector2 operator+(vector2& vec1, const vector2& vec2);
    friend vector2 operator-(vector2& vec1, const vector2& vec2);
    friend vector2 operator*(vector2& vec1, const vector2& vec2);
    friend vector2 operator/(vector2& vec1, const vector2& vec2);

    vector2& operator+=(const vector2 vec);
    vector2& operator-=(const vector2 vec);
    vector2& operator*=(const vector2 vec);
    vector2& operator/=(const vector2 vec);

    vector2& operator*(const int& i);
    vector2& operator*(const float& i);
    vector2& zero();

    static float angle(const vector2& vec, const vector2& vec2);

    static vector2 rotate_vector(vector2 vec, float angleDeg);

    float length() const;
    vector2& normalize() const;

    static vector2 abs_substract(const vector2& v1, const vector2& v2);
    
    vector2 operator-() const
    {
        return vector2(-x, -y);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const vector2 vec);
};
