#include "vector2.h"

vector2::vector2()
{
    x = 0.0f;
    y = 0.0f;
}

vector2::vector2(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

vector2& vector2::add(const vector2 vec)
{
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

vector2& vector2::substract(const vector2 vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

vector2& vector2::multiply(const vector2 vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

vector2& vector2::divide(const vector2 vec)
{
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

vector2 operator+(vector2& vec1, const vector2& vec2)
{
    return vec1.add(vec2);
}

vector2 operator-(vector2& vec1, const vector2& vec2)
{
    return vec1.substract(vec2);
}

vector2 operator*(vector2& vec1, const vector2& vec2)
{
    return vec1.multiply(vec2);
}

vector2 operator/(vector2& vec1, const vector2& vec2)
{
    return vec1.divide(vec2);
}

vector2& vector2::operator+=(const vector2 vec)
{
    return this->add(vec);
}

vector2& vector2::operator-=(const vector2 vec)
{
    return this->substract(vec);
}

vector2& vector2::operator*=(const vector2 vec)
{
    return this->multiply(vec);
}

vector2& vector2::operator/=(const vector2 vec)
{
    return this->divide(vec);
}

vector2& vector2::operator*(const int& i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

vector2& vector2::operator*(const float& i)
{
    this->x *= i;
    this->y *= i;
    return *this;
}

vector2& vector2::zero()
{
    this->x = 0.0f;
    this->y = 0.0f;
    return *this;
}

float vector2::length() const
{
    return sqrt(this->x * this->x + this->y * this->y);
}

vector2& vector2::normalize() const
{
    const float length = this->length();
    return *new vector2(this->x / length, this->y / length);
}

std::ostream& operator<<(std::ostream& os, const vector2 vec)
{
    os << "vec2(" << vec.x << ", " << vec.y << ")";
    return os;
}
