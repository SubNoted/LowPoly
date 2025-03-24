#ifndef _VECTOR_
#define _VECTOR_

// #include <lowPoly.h>
#include <devSetup.h>

class Vector
{
    public:
    float x, y, z;
    Vector(void);
    Vector(float x, float y, float z);
    Vector(float x, float y);
    Vector(float x);

    bool isEqual(Vector v),
        isEqual(Vector v, float r),
        isExist();

    float L(void),
        L(Vector v),
        L(float x, float y, float z),
        L(float x, float y);

    Vector Summ(Vector s);
    Vector Delta(Vector s);
    Vector Delta(float x, float y);

    void Plus(const Vector &v),
        Plus(const float &x, const float &y, const float &z),
        Plus(const float &x, const float &y);

    void Minus(const Vector &v),
        Minus(const float &x, const float &y, const float &z),
        Minus(const float &x, const float &y);

    void Multiply(const Vector &v),
        Multiply(const float &x, const float &y, const float &z),
        Multiply(const float &x, const float &y),
        Multiply(const float &a);

    void Equals(const Vector &v),
        Equals(const float &x, const float &y, const float &z),
        Equals(const float &x, const float &y);

    void Normalize();
    Vector getNormalized();

    float ScalarProd(Vector v);

    void setRotation(Vector alp);
    void Rotate(Vector alp);
    Vector setRotation(Vector alp, Vector o);
    Vector Rotate(Vector alp, Vector o);

    Vector toFOV_XY(const float& fov);

    void Reverse();
    void Round();
    void Reset();

    static Vector Normal(Vector& a, Vector& b, Vector& c);
};

#include <cstdint>  // Include the cstdint library for uint8_t type

class Vector_16
{
    public:
    int16_t x = 0, y = 0, z = 0;
    Vector_16(void)
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    };
    Vector_16(int16_t x, int16_t y)
    {
        this->x = x;
        this->y = y;
        this->z = 0;
    };
    Vector_16(int16_t x, int16_t y, int16_t z)
    {
        this->x = (int16_t)x;
        this->y = (int16_t)y;
        this->z = (int16_t)z;
    };
    
    Vector_16(const Vector& v)
    {
        this->x = (int16_t)(v.x*255);
        this->y = (int16_t)(v.y*255);
        this->z = (int16_t)(v.z*255);
    };

    int16_t L(void);
    Vector_16 getNormalized();
};
class Vector_8
{
public:
    int8_t x, y, z;
    Vector_8(void);
    Vector_8(int8_t x, int8_t y, int8_t z);
    Vector_8(Vector& v);
    Vector_8(Vector& a, Vector& b, Vector& c);//normal

    bool isEqual(Vector_8 v),
        isExist();

    void Equals(Vector_8 v),
        Equals(int8_t x, int8_t y, int8_t z);

    int8_t ScalarProd(Vector_8 v);
    
    int8_t L(void);
    Vector_8 getNormalized();

    void setRotation(Vector alp);
    void Rotate(Vector alp);
    void Reverse();
    void Reset();
};

// class Vector_u16
// {
//     public:
//     uint16_t x, y, z;
//     Vector_u16(void);
//     Vector_u16(uint16_t x, uint16_t y, uint16_t z)
//     {
//         this->x = x;
//         this->y = y;
//         this->z = z;
//     };
// };
// class Vector_u8
// {
//     public:
//     uint8_t x, y, z;
//     Vector_u8(void);
//     Vector_u8(uint8_t x, uint8_t y, uint8_t z)
//     {
//         this->x = x;
//         this->y = y;
//         this->z = z;
//     };
// };

#endif