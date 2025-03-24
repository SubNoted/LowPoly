#include "vector.h"
#include <math.h>

float hypotenuse(float a, float b)
{
    return sqrt(a*a + b*b);
} 
float alpha(float y, float x)
{
    return (atan(y/x) + (x<0 ? M_PI : 0));
}

Vector::Vector(void)
{
    x = 0;
    y = 0;
    z = 0;
}
Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector::Vector(float x, float y)
{
    this->x = x;
    this->y = y;
}
Vector::Vector(float x)
{
    this->x = x;
    this->y = x;
    this->z = x;
}

bool Vector::isEqual(Vector v)
{
    if (this->x == v.x && this->y == v.y && this->z == v.z)
        return true;
    return false;
}

bool Vector::isEqual(Vector v, float r)
{
    if (abs(this->x - v.x) < r && abs(this->y - v.y) < r && abs(this->z - v.z) < r)
        return true;
    return false;
}

bool Vector::isExist()
{
    if (this->x != 0 || this->z != 0 || this->y != 0)
        return true;
    return false;
}

float Vector::L(void)
{
    return sqrt(x*x + y*y + z*z);
}
float Vector::L(Vector v)
{
    return sqrt(pow(this->x-v.x, 2) + pow(this->y-v.y, 2) + pow(this->z-v.z, 2));
}
float Vector::L(float x, float y, float z)
{
    return sqrt(pow(this->x-x, 2) + pow(this->y-y, 2) + pow(this->z-z, 2));
}
float Vector::L(float x, float y)
{
    return sqrt(pow(this->x-x, 2) + pow(this->y-y, 2));
}

Vector Vector::Summ(Vector s)
{
    return Vector(this->x+s.x, this->y+s.y, this->z+s.z);
}
Vector Vector::Delta(Vector d)
{
    return Vector((this->x-d.x), (this->y-d.y), (this->z-d.z));
}
Vector Vector::Delta(float x, float y)
{
    return Vector((this->x-x), (this->y-y), this->z);
}

void Vector::Plus(const Vector &v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
}
void Vector::Plus(const float &x, const float &y, const float &z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}
void Vector::Plus(const float &x, const float &y)
{
    this->x += x;
    this->y += y;
}

void Vector::Minus(const Vector &v)
{
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
}
void Vector::Minus(const float &x, const float &y, const float &z)
{
    this->x -= x;
    this->y -= y;
    this->z -= z;
}
void Vector::Minus(const float &x, const float &y)
{
    this->x -= x;
    this->y -= y;
}

void Vector::Multiply(const Vector &v)
{
    this->x *= v.x;
    this->y *= v.y;
    this->z *= v.z;
}
void Vector::Multiply(const float &x, const float &y, const float &z)
{
    this->x *= x;
    this->y *= y;
    this->z *= z;
}
void Vector::Multiply(const float &x, const float &y)
{
    this->x *= x;
    this->y *= y;
}

void Vector::Multiply(const float &a)
{
    this->x *= a;
    this->y *= a;
    this->z *= a;
}

void Vector::Equals(const Vector& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}
void Vector::Equals(const float &x, const float &y, const float &z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
void Vector::Equals(const float &x, const float &y)
{
    this->x = x;
    this->y = y;
}

void Vector::Normalize()
{
    float m = this->L();
        
    this->x = this->x/m;
    this->y = this->y/m;
    this->z = this->z/m;
}
Vector Vector::getNormalized()
{
    float m = this->L();
    return Vector(this->x/m, this->y/m, this->z/m);
}

float Vector::ScalarProd(Vector v)
{
    Vector v0 = Vector(*this);
    v0.Normalize();
    v.Normalize();
    return (v0.x*v.x + v0.y*v.y + v0.z*v.z);
}

void Vector::setRotation(Vector d)
{
    this->Equals(this->L(), 0, 0);
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
}

void Vector::Rotate(Vector d)
{
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
}

Vector Vector::setRotation(Vector d, Vector o)
{
    this->Minus(o);
    this->Equals(this->L(), 0, 0);
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
    this->Plus(o);
    return *this;
}

Vector Vector::Rotate(Vector d, Vector o)
{
    this->Minus(o);
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
    this->Plus(o);
    return *this;
}

Vector Vector::toFOV_XY(const float& fov)
{
    this->Equals(CENTR_X  + (this->x - CENTR_X)/(this->z*tan(fov/2)/SCRN_WIDTH+1), 
            CENTR_Y + (this->y - CENTR_Y)/(this->z*tan(fov/2)/SCRN_HEIGHT+1));
    return *this;
}

void Vector::Reverse()
{
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
}

void Vector::Round()
{
    this->x = (int)this->x;
    this->y = (int)this->y;
    this->z = (int)this->z;
}

void Vector::Reset()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector Vector::Normal(Vector& a, Vector& b, Vector& c)
{
    Vector ab = b.Delta(a), ac = c.Delta(a);
    return Vector(ab.y * ac.z - ab.z * ac.y, ab.z * ac.x - ab.x * ac.z, ab.x * ac.y - ab.y * ac.x);
}

////////////////////////////////////////////Vector_16/////////////////////////////////////////


int16_t Vector_16::L(void)
{
    return sqrt(x*x + y*y + z*z);
}

Vector_16 Vector_16::getNormalized()
{
    int16_t m = this->L();
    return Vector_16(this->x*127/m, this->y*127/m, this->z*127/m);//todo fix
}

////////////////////////////////////////////Vector_8/////////////////////////////////////////

Vector_8::Vector_8(int8_t x, int8_t y, int8_t z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector_8::Vector_8()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Vector_8::Vector_8(Vector& v)
{
    v.Normalize();
    this->x = uint8_t(v.x);
    this->y = uint8_t(v.y);
    this->z = uint8_t(v.z);
}
Vector_8::Vector_8(Vector& a, Vector& b, Vector& c)//normal
{
    Vector ab = b.Delta(a), ac = c.Delta(a);
    this->x = uint8_t((ab.y * ac.z - ab.z * ac.y) );
    this->y = uint8_t((ab.z * ac.x - ab.x * ac.z) );
    this->z = uint8_t((ab.x * ac.y - ab.y * ac.x) );
}

bool Vector_8::isEqual(Vector_8 v)
{
    if (this->x == v.x && this->y == v.y && this->z == v.z)
        return true;
    return false;
}
bool Vector_8::isExist()
{
    if (this->x != 0 || this->y != 0 || this->z != 0)
        return true;
    return false;
}
void Vector_8::Equals(int8_t x, int8_t y, int8_t z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
int8_t Vector_8::ScalarProd(Vector_8 v)
{
    return ((*this).x*v.x + (*this).y*v.y + (*this).z*v.z)/255;
}
void Vector_8::setRotation(Vector d)
{
    this->Equals(1, 0, 0);
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
}
void Vector_8::Rotate(Vector d)
{
    this->Equals(
        this->x * cos(d.x) - this->y * sin(d.x),
        this->x * sin(d.x) + this->y * cos(d.x),
        this->z
    );
    
    this->Equals(
        this->x * cos(d.y) - this->z * sin(d.y),
        this->y,
        this->x * sin(d.y) + this->z * cos(d.y)
    );
    
    this->Equals(
        this->x,
        this->z * sin(d.z) + this->y * cos(d.z),
        this->z * cos(d.z) - this->y * sin(d.z)
    );
}
void Vector_8::Reverse()
{
    this->x = -this->x;
    this->y = -this->y;
    this->z = -this->z;
}
void Vector_8::Reset()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

int8_t Vector_8::L(void)
{
    return sqrt(x*x + y*y + z*z);
}

Vector_8 Vector_8::getNormalized()
{
    int8_t m = this->L();
    return Vector_8(this->x*127/m, this->y*127/m, this->z*127/m);
}
