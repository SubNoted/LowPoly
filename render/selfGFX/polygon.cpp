#include "polygon.h"

SimplePolygon::SimplePolygon() {}

SimplePolygon::SimplePolygon(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2) 
{
    setVertices(v0, v1, v2);
}

void SimplePolygon::setVertices(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2)  
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}

void SimplePolygon::setUV(uint8_t uvx0,uint8_t uvy0, uint8_t uvx1,uint8_t uvy1, uint8_t uvx2,uint8_t uvy2)
{
    uv[0][0] = uvx0;
    uv[0][1] = uvy0;
    uv[1][0] = uvx1;
    uv[1][1] = uvy1;
    uv[2][0] = uvx2;
    uv[2][1] = uvy2;
}

//////////////////Polygon////////////////////////////////

Polygon::Polygon() {}

Polygon::Polygon(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2, \
            const uint16_t& vt0, const uint16_t& vt1, const uint16_t& vt2,\
            const uint16_t& vn0, const uint16_t& vn1, const uint16_t& vn2,\
            const uint8_t& texid)
{
    this->v[0] = v0;
    this->v[1] = v1;
    this->v[2] = v2;
    this->vt[0] = vt0;
    this->vt[1] = vt1;
    this->vt[2] = vt2;
    this->vn[0] = vn0;
    this->vn[1] = vn1;
    this->vn[2] = vn2;
    this->texid = texid;
}
void Polygon::setVertices(const uint16_t& v0, const uint16_t& v1, const uint16_t& v2)  
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
}
void Polygon::setUV(uint16_t uv0, uint16_t uv1, uint16_t uv2)
{
    vt[0] = uv0;
    vt[1] = uv1;
    vt[2] = uv2;
}
void Polygon::setVertexNormals(const uint16_t& n0, const uint16_t& n1, const uint16_t& n2)
{
    vn[0] = n0;
    vn[1] = n1;
    vn[2] = n2;
}