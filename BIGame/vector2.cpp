#include "vector2.h"

#include <SDL_stdinc.h>

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

vector2 vector2::abs_substract(const vector2& v1, const vector2& v2)
{
    return vector2(fabs(v1.x - v2.x), fabs(v1.y - v2.y));
}

float vector2::angle(const vector2& vec, const vector2& vec2) {
    const float dot_product = vec.x * vec2.x + vec.y * vec2.y;
    const float magnitude1 = vec.length();
    const float magnitude2 = vec2.length();

    if (magnitude1 == 0 || magnitude2 == 0) {
        return 0.0f;
    }

    const float cos_theta = dot_product / (magnitude1 * magnitude2);
    // Clamp cos_theta to [-1, 1] to prevent domain errors in acos
    const float clamped_cos_theta = cos_theta < -1.0f ? -1.0f : cos_theta > 1.0f ? 1.0f : cos_theta;

    return acos(clamped_cos_theta) * (180.0/3.141592653589793238463);
}

vector2 vector2::rotate_vector(const vector2 vec, const float angleDeg)
{
    float angleRad = angleDeg * (M_PI / 180.0);
    
    float x = vec.x;
    float y = vec.y;
    
    float x_prime = x * cos(angleRad) - y * sin(angleRad);
    float y_prime = x * sin(angleRad) + y * cos(angleRad);
    
    return {x_prime, y_prime};
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
