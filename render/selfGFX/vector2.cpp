#include "vector2.h"




void Vector2_16::Equals(const int16_t& x, const int16_t& y)
{
    this->x = x;
    this->y = y;
}

////////////////////////////////////////////VECTOR2/////////////////////////////////////////////////////

Vector2::Vector2(void)
{
    x = 0;
    y = 0;
}
Vector2::Vector2(Vector v)
{
    this->x = v.x;
    this->y = v.y;
}
Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}
Vector Vector2::toVector3()
{
    return Vector(this->x, this->y);
}

bool Vector2::isEqual(Vector2 v)
{
    if (this->x == v.x && this->y == v.y)
        return true;
    return false;
}

bool Vector2::isEqual(Vector2 v, float r)
{
    if (abs(this->x - v.x) < r && abs(this->y - v.y) < r)
        return true;
    return false;
}

bool Vector2::isExist()
{
    if (this->x != 0 || this->y != 0)
        return true;
    return false;
}

float Vector2::L(void)
{
    return sqrt(x*x + y*y);
}
float Vector2::L(Vector2 v)
{
    return sqrt(pow(this->x-v.x, 2) + pow(this->y-v.y, 2));
}
float Vector2::L(float x, float y)
{
    return sqrt(pow(this->x-x, 2) + pow(this->y-y, 2));
}

Vector2 Vector2::Summ(Vector2 s)
{
    return Vector2(this->x+s.x, this->y+s.y);
}
Vector2 Vector2::Summ(Vector s)
{
    return Vector2(this->x+s.x, this->y+s.y);
}
Vector2 Vector2::Delta(Vector2 s)
{
    return Vector2(this->x-s.x, this->y-s.y);
}
Vector2 Vector2::Delta(Vector s)
{
    return Vector2(this->x-s.x, this->y-s.y);
}

void Vector2::Plus(Vector2 v)
{
    this->x += v.x;
    this->y += v.y;
}
void Vector2::Plus(float x, float y)
{
    this->x += x;
    this->y += y;
}

void Vector2::Minus(Vector2 v)
{
    this->x -= v.x;
    this->y -= v.y;
}
void Vector2::Minus(float x, float y)
{
    this->x -= x;
    this->y -= y;
}

void Vector2::Multiply(Vector2 v)
{
    this->x *= v.x;
    this->y *= v.y;
}
void Vector2::Multiply(float x, float y)
{
    this->x *= x;
    this->y *= y;
}

void Vector2::Equals(Vector2 v)
{
    this->x = v.x;
    this->y = v.y;
}
void Vector2::Equals(Vector v)
{
    this->x = v.x;
    this->y = v.y;
}
void Vector2::Equals(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vector2::Rotate(float d)
{
    this->Equals(
        this->x * cos(d) - this->y * sin(d),
        this->x * sin(d) + this->y * cos(d)
    );
}

void Vector2::Round()
{
    this->x = (int)this->x;
    this->y = (int)this->y;
}

void Vector2::Reset()
{
    this->x = 0;
    this->y = 0;
}

