#ifndef _VECTOR2_
#define _VECTOR2_


#include <devSetup.h>
#include "vector.h"
#include <cstdint> 

class Vector2_16
{
    public:
    int16_t x = 0, y = 0;
    Vector2_16(void)
    {
        this->x = 0;
        this->y = 0;
    };
    Vector2_16(int16_t x, int16_t y)
    {
        this->x = x;
        this->y = y;
    };

    void Equals(const int16_t &x, const int16_t &y);
};

class Vector2_8
{
    public:
    int8_t x=0, y=0;
    Vector2_8(void)
    {
        this->x = 0;
        this->y = 0;
    };
    Vector2_8(int8_t x, int8_t y)
    {
        this->x = x;
        this->y = y;
    };
};

// class Vector2_u16
// {
//     public:
//     uint16_t x, y;
//     Vector2_u16(void);
//     Vector2_u16(uint16_t x, uint16_t y)
//     {
//         this->x = x;
//         this->y = y;
//     };
// };

class Vector2_u8
{
    public:
    uint8_t x = 0, y = 0;
    Vector2_u8(void)
    {
        this->x = 0;
        this->y = 0;
    };
    Vector2_u8(uint8_t x, uint8_t y)
    {
        this->x = x;
        this->y = y;
    };
};


class Vector2
{
    public:
    float x, y;
    Vector2(void);
    Vector2(Vector v);
    Vector2(float x, float y);
    Vector toVector3();

    bool isEqual(Vector2 v),
        isEqual(Vector2 v, float r);

    bool isExist();

    float L(void),
        L(Vector2 v),
        L(float x, float y);

    Vector2 Summ(Vector2 s);
    Vector2 Summ(Vector s);
    Vector2 Delta(Vector2 s);
    Vector2 Delta(Vector s);

    void Plus(Vector2 v),
        Plus(float x, float y);

    void Minus(Vector2 v),
        Minus(float x, float y);

    void Multiply(Vector2 v),
        Multiply(float x, float y);

    void Equals(Vector2 v),
        Equals(Vector v),
        Equals(float x, float y);

        
    void Rotate(float alp);
    void Round();
    void Reset();
};
#endif