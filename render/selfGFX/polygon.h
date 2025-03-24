#ifndef _POLYGON_
#define _POLYGON_

#include "vector2.h"
// #include <scene/scene.h>
#include <vector>

class PolygonWOTex
{
private:

public:
    uint16_t vertices[3]; //number of vertices in scene
    
    PolygonWOTex(void);
    PolygonWOTex(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2);
    void setVertices(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2);
};

class SimplePolygon
{
private:

public:
    uint16_t vertices[3]; //number of vertices in scene
    uint8_t uv[3][2];
    
    SimplePolygon(void);
    SimplePolygon(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2);
    void setVertices(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2);
    void setUV(uint8_t uvx0,uint8_t uvy0, uint8_t uvx1,uint8_t uvy1, uint8_t uvx2,uint8_t uvy2);
};
class Polygon
{
private:

public:
    uint16_t v[3];
    uint16_t vt[3];
    uint16_t vn[3];
    int8_t texid;
    
    Polygon(void);
    Polygon(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2, \
            const uint16_t& vt0, const uint16_t& vt1, const uint16_t& vt2,\
            const uint16_t& vn0, const uint16_t& vn1, const uint16_t& vn2,\
            const uint8_t& texid);
    void setVertices(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2);
    void setUV(uint16_t uv0, uint16_t uv1, uint16_t uv2);
    void setVertexNormals(const uint16_t& n0, const uint16_t& n1, const uint16_t& n2);
};


#endif