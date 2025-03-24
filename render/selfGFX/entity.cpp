#include "entity.h"
#include <math.h>

uint16_t SCREEN_WIDTH;
uint16_t SCREEN_HEIGHT;

#define CENTR_X SCREEN_WIDTH/2
#define CENTR_Y SCREEN_HEIGHT/2


Entity* Entity::ALL_ENTITIES[64];
uint8_t Entity::ENTITIES_COUNT = 0;

bool Entity::isFirstTouchedAnyEntity()
{
    for(int i = 0; i < ENTITIES_COUNT; i++)
        if(ALL_ENTITIES[i]->isFirstTouched())
            return true;
    return false;
}

void Entity::processPhysics(float &t)
{
    if (!this->O.isExist())
        return;
    this->V.x += this->constA.x*t/60;
    this->V.y += this->constA.y*t/60;
    this->V.z += this->constA.z*t/60;
    this->O.Plus(this->V);
    if (constA.z != 0 && this->O.z * this->constA.z > 0)
    {
        this->O.z = 0;
        this->V.z = 0;
    }
    if (this->O.isEqual(Vector(), 1))
        this->O.Equals(Vector());
    if (this->V.isEqual(Vector(), 1))
        this->V.Equals(Vector());
    else
        this->V.Multiply(0.98);
}

// bool Entity::isTouched(Vector2 touch)
// {
//     if (!touch.isExist())
//         return false;
//     if (touch.x >= minp.x - 10 + O.x - Camera.x && touch.x <= maxp.x + 10 + O.x - Camera.x 
//     && touch.y >= minp.y - 10 + O.y - Camera.y && touch.y <= maxp.y + 10 + O.y - Camera.y)
//     {
//         return true;
//     }
//     return false;
// }

Entity::Entity()
{
    this->O = Vector(0, 0, 0);
    this->V = Vector(0, 0, 0);
    this->Size = 1;
    this->Angle = Vector(0, 0, 0);
    this->Color = 0;
    this->Update = false;
}

Entity::Entity(std::vector<Entity*>& entities)
{
    init(entities);
    this->O = Vector(0, 0, 0);
    this->V = Vector(0, 0, 0);
    this->Size = 1;
    this->Angle = Vector(0, 0, 0);
    this->Color = 0;
    this->Update = false;
}


void Entity::init(std::vector<Entity*>& entities)
{
    entities.push_back(this);
}


void Entity::createModel(const float v[][3], const uint8_t vt[][2], const float vn[][3], const uint16_t &vLength, const uint16_t f[][10], const uint16_t &fLength, const uint8_t* tex, const uint8_t* texWidths, const uint16_t* texLengths, const uint32_t &texLength, const uint8_t &texsLength)
{
    this->vertLength = vLength;
    this->polyLength = fLength;

    this->vertices = new Vector[this->vertLength];
    this->textureCoords = new Vector2_u8[this->vertLength];
    this->normals = new Vector[this->vertLength];

    this->vertices_b = (Vector_16*)heap_caps_calloc(this->vertLength, sizeof(Vector_16), MALLOC_CAP_INTERNAL);

    for (int i = 0; i < this->vertLength; i++)
    {
        this->vertices[i].Equals(v[i][0], v[i][1], v[i][2]);

        this->textureCoords[i].x = vt[i][0];
        this->textureCoords[i].y = vt[i][1];

        this->normals[i].Equals(vn[i][0], vn[i][1], vn[i][2]);
    }
    

    this->polygons = new Polygon[this->polyLength];
    for (int i = 0; i < this->polyLength; i++)
    {
        this->polygons[i] = Polygon(f[i][0], f[i][1], f[i][2], f[i][3], f[i][4], f[i][5], f[i][6], f[i][7], f[i][8], f[i][9]);
    }

    this->textureLength = texLength;
    this->texturesLength = texsLength;

    this->texture = new uint8_t[this->textureLength];
    for (int i = 0; i < this->textureLength; i++)
    {
        this->texture[i] = tex[i];
    }
    this->textureWidths = new uint8_t[this->texturesLength];
    this->textureLengths = new uint16_t[this->texturesLength];
    for (int i = 0; i < this->texturesLength; i++)
    {
        this->textureWidths[i] = texWidths[i];
        this->textureLengths[i] = texLengths[i];
    }
    updateBuffer();
}
void Entity::updateBuffer()
{
    for (uint16_t i = 0; i < this->vertLength; i++)
    {
        this->vertices_b[i].x = this->vertices[i].x*this->Size + O.x; 
        this->vertices_b[i].y = this->vertices[i].y*this->Size + O.y;
        this->vertices_b[i].z = this->vertices[i].z*this->Size + O.z;
    }
}

void Entity::createCube(Vector o, Vector size)
{
//     //todo deinit if exists
//     this->O.Equals(o); 
//     this->Size.Equals(size);
//     this->Update = true;
//     this->vertSize = 8;
//     this->polySize = 12;
//     this->vertices = new Vector[vertSize];
//     this->polygons = new Polygon[polySize];

// //          4----------5         
// //        /          / |        
// //     /          /    |         
// //   0----------1      |         
// //   |      7   |      6      
// //   |          |    /           
// //   |          | /          
// //   3----------2     


//     vertices[0] = (Vector(O.x - Size.x/2, O.y + Size.y/2, O.z + Size.z/2));//0
//     vertices[1] = (Vector(O.x + Size.x/2, O.y + Size.y/2, O.z + Size.z/2));//1
//     vertices[2] = (Vector(O.x + Size.x/2, O.y - Size.y/2, O.z + Size.z/2));//2
//     vertices[3] = (Vector(O.x - Size.x/2, O.y - Size.y/2, O.z + Size.z/2));//3

//     vertices[4] = (Vector(O.x - Size.x/2, O.y + Size.y/2, O.z - Size.z/2));//4
//     vertices[5] = (Vector(O.x + Size.x/2, O.y + Size.y/2, O.z - Size.z/2));//5
//     vertices[6] = (Vector(O.x + Size.x/2, O.y - Size.y/2, O.z - Size.z/2));//6
//     vertices[7] = (Vector(O.x - Size.x/2, O.y - Size.y/2, O.z - Size.z/2));//7


//     for (int i = 0; i < 4; i++)
//     {
//         polygons[i*2] = (SimplePolygon(i, 4 + (i + 1) % 4, (i+1)%4));//todo setVert
//         polygons[i*2].setUV(0, 0, 255, 255, 255, 0);
//         polygons[i*2 + 1] = (SimplePolygon(i, 4 + i, 4 + (i + 1) % 4));
//         polygons[i*2 + 1].setUV(0, 0, 0, 255, 255, 255);
//     }

//     //0-1-2-3
//     polygons[8] = (SimplePolygon(0, 1, 2));
//     polygons[8].setUV(0, 255, 255, 255, 255, 0);
//     polygons[9] = (SimplePolygon(0, 2, 3));
//     polygons[9].setUV(0, 255, 255, 0, 0, 0);

//     //4-5-6-7
//     polygons[10] = (SimplePolygon(4, 6, 5));
//     polygons[10].setUV(0, 255, 255, 255, 255, 0);//todo fix mapping
//     polygons[11] = (SimplePolygon(4, 7, 6));
//     polygons[11].setUV(0, 255, 255, 0, 0, 0);//todo fix mapping

}


void Entity::setCube()
{
    // vertices[0] = (Vector(O.x - Size.x/2, O.y + Size.y/2, O.z + Size.z/2));//0
    // vertices[1] = (Vector(O.x + Size.x/2, O.y + Size.y/2, O.z + Size.z/2));//1
    // vertices[2] = (Vector(O.x + Size.x/2, O.y - Size.y/2, O.z + Size.z/2));//2
    // vertices[3] = (Vector(O.x - Size.x/2, O.y - Size.y/2, O.z + Size.z/2));//3

    // vertices[4] = (Vector(O.x - Size.x/2, O.y + Size.y/2, O.z - Size.z/2));//4
    // vertices[5] = (Vector(O.x + Size.x/2, O.y + Size.y/2, O.z - Size.z/2));//5
    // vertices[6] = (Vector(O.x + Size.x/2, O.y - Size.y/2, O.z - Size.z/2));//6
    // vertices[7] = (Vector(O.x - Size.x/2, O.y - Size.y/2, O.z - Size.z/2));//7

}

void Entity::setPosition(Vector v)
{
    if (v.isEqual(O))
        return;
    this->O.Equals(v);
    Update = true;
}
Vector Entity::getPosition()
{
    return this->O;
}

void Entity::setRotation(Vector a)
{    
    if (a.isEqual(Angle))
        return;
    this->rotate(a.Delta(this->Angle));
}
void Entity::rotate(Vector a)
{
    if (!a.isExist())
        return;
    this->Angle.Plus(a);
    for (int i = 0; i < vertLength; i++)
    {
        vertices[i].Rotate(a);
        normals[i].Rotate(a);
    }
}
Vector Entity::getAngle()
{
    return this->Angle;
}
void Entity::setColor(uint16_t c)
{
    if (c == Color)
        return;
    Color = c;
    Update = true;
}
uint16_t Entity::getColor()
{
    return Color;
}
// void Entity::setTextSize(float x)
// {
//     if (x == textSize)
//         return;
//     textSize = x;
//     Update = true;
// }
// float Entity::getTextSize()
// {
//     return textSize;
// }
// void Entity::setText(String s, uint8_t n)
// {
//     if(s != Text[n])
//     {
//         Text[n] = s;
//         Update = true;
//     }
// }
// String Entity::getText(uint8_t n)
// {
//     return Text[n];
// }
// void Entity::setTextOffSet(Vector2& v)
// {
//     if(v.isEqual(textOffSet))
//         return;
//     textOffSet.Equals(v);
//     Update = true;
// }
// void Entity::setTextOffSet(const int16_t& x, const int16_t& y)
// {
//     if(x == this->textOffSet.x && y == this->textOffSet.y)
//         return;
//     textOffSet.Equals(x ,y);
//     Update = true;
// }
// Vector2 Entity::getTextOffSet()
// {
//     return textOffSet;
// }
// void Entity::setTextWrap(bool b)
// {
//     if (b == textWrap)
//         return;
//     textWrap = b;
//     Update = true;
// }
// bool Entity::getTextWrap()
// {
//     return textWrap;
// }
// void Entity::setTextColor(uint16_t c)
// {
//     if (textColor == c)
//         return;
//     textColor = c;
//     Update = true;
// }
// uint16_t Entity::getTextColor()
// {
//     return textColor;
// }

//////////////private////////////
// void Entity::createSprite(uint16_t x, uint16_t y)
// {
//     //sprite->setColorDepth(d);
// 	sprite->createSprite(x, y);
//     sprite->fillScreen(TFT_TRANSPARENT);
// 	sprite->setSwapBytes(true);

//     sprite->setTextDatum(datum);
// }
// bool Entity::isToUpdate()
// {
//     if (this->O.y - this->Size.y*2 > Camera.y + SCREEN_HEIGHT || this->O.y + this->Size.y*2 < Camera.y || this->O.x - this->Size.x*2 > Camera.x + SCREEN_WIDTH || this->O.x + this->Size.x*2 < Camera.x)
//         return false;
//     if (this->O.z > 1500 || this->O.z < -1500)
//         return false;
//     if (Update || !O.isEqual(last_O) || !Size.isEqual(last_Size) || !Angle.isEqual(last_Angle) 
//     || Text[0] != last_Text[0] || Text[1] != last_Text[1] || Text[2] != last_Text[2] || textSize != last_textSize || textWrap != last_textWrap)
//         return true;
//     return false;
// }
// void Entity::endDraw()
// {
//     last_O.Equals(O);
//     last_Size.Equals(Size);
//     last_Angle.Equals(Angle);
//     last_Text[0] = Text[0];
//     last_Text[1] = Text[1];
//     last_Text[2] = Text[2];
//     last_textOffSet = textOffSet;
//     last_textSize = textSize;
//     last_textWrap = textWrap;
//     this->Update = false;
// }

// void Entity::drawCubeFaces(Vector* p, const uint32_t c, bool light)
// {
//     uint32_t col = c;
//     float shade;
//     Vector normal;
//     for(int i=0;i<4;i++)
//     {
//         //facing camera (normal.z < 0)
//         col = c;
//         normal = Vector::Normal(p[i], p[(i+1)%4], p[(i+1)%4+4]);
//         if (light)
//         {
//             shade = (normal.ScalarProd(this->lightDirection)/3)+0.5;
//             col = sprite->alphaBlend(shade*255,col, TFT_BLACK);
//         }
//         if (normal.z > 0)
//         {
//             if (O.z > 500 && O.z < 1500)
//                 col = sprite->alphaBlend(255 - (O.z - 500)/4,col, *BG_COL);
//             else if (O.z >= 1500)
//                 col = *BG_COL;
//             sprite->fillTriangle(p[i].x, p[i].y, p[(i+1)%4].x, p[(i+1)%4].y, p[(i+1)%4+4].x, p[(i+1)%4+4].y, col);
//             sprite->fillTriangle(p[i].x, p[i].y, p[i+4].x,p[i+4].y, p[(i+1)%4+4].x, p[(i+1)%4+4].y, col);
//         }
//     }
//     col = c;
//     normal = Vector::Normal(p[1], p[0], p[2]);
//     if (light)
//     {
//         shade = (normal.ScalarProd(this->lightDirection)/3)+0.5;
//         col = sprite->alphaBlend(shade*255,col, TFT_BLACK);
//     }
//     if (normal.z > 0)
//     {
//         if (O.z > 500 && O.z < 1500)
//             col = sprite->alphaBlend(255 - (O.z - 500)/4,col, *BG_COL);
//         else if (O.z >= 1500)
//             col = *BG_COL;
//         sprite->fillTriangle(p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, col);
//         sprite->fillTriangle(p[0].x, p[0].y, p[3].x, p[3].y, p[2].x, p[2].y, col);
//     }
    
//     col = c;
//     normal = Vector::Normal(p[5], p[6], p[4]);
//     if (light)
//     {
//         shade = (normal.ScalarProd(this->lightDirection)/3)+0.5;
//         col = sprite->alphaBlend(shade*255,col, TFT_BLACK);
//     }
//     if (normal.z > 0)
//     {
//         if (O.z > 500 && O.z < 1500)
//             col = sprite->alphaBlend(255 - (O.z - 500)/4,col, *BG_COL);
//         else if (O.z >= 1500)
//             col = *BG_COL;
//         sprite->fillTriangle(p[4].x, p[4].y, p[5].x, p[5].y, p[6].x, p[6].y, col);
//         sprite->fillTriangle(p[4].x, p[4].y, p[7].x, p[7].y, p[6].x, p[6].y, col);
//     }
// }

// void Entity::drawParallelepiped()
// {
//     if (!isToUpdate())
//         return;

//     Vector o = O.Delta(Camera.x, Camera.y);
//     Vector last_o = last_O.Delta(last_Camera.x, last_Camera.y);
//     o.Round();
//     last_o.Round();

//     //split render to 2 sprites
//     bool split = abs(o.x - last_o.x) > 200 || abs(o.y - last_o.y) > 200;

//     const uint16_t n = 8;
    
//     minp.Equals(999,999), maxp.Equals(-999,-999);   
                                        
//         //          4----------5         
//         //        /          / |        
//         //     /          /    |         
//         //   0----------1      |         
//         //   |      7   |      6      
//         //   |          |    /           
//         //   |          | /          
//         //   3----------2                
                                        
//     Vector last_p[n];
//     last_p[0].Equals(-last_Size.x/2, -last_Size.y/2, last_Size.z/2);
//     last_p[1].Equals(last_Size.x/2, -last_Size.y/2, last_Size.z/2);
//     last_p[2].Equals(last_Size.x/2, last_Size.y/2, last_Size.z/2);
//     last_p[3].Equals(-last_Size.x/2, last_Size.y/2, last_Size.z/2);
//     last_p[4].Equals(-last_Size.x/2, -last_Size.y/2, -last_Size.z/2);
//     last_p[5].Equals(last_Size.x/2, -last_Size.y/2, -last_Size.z/2);
//     last_p[6].Equals(last_Size.x/2, last_Size.y/2, -last_Size.z/2);
//     last_p[7].Equals(-last_Size.x/2, last_Size.y/2, -last_Size.z/2);
    
//     for (int i = 0; i < n; i++)
//     {
//         last_p[i].Rotate(last_Angle);
//         last_p[i].Round();
//         toFOV_XY(&last_p[i], *last_FOV, last_o, o);
//         last_p[i].Round();

//         if(last_p[i].x < minp.x)
//             minp.x = last_p[i].x;
//         else if(last_p[i].x > maxp.x)
//             maxp.x = last_p[i].x;
//         if(last_p[i].y < minp.y)
//             minp.y = last_p[i].y;
//         else if(last_p[i].y > maxp.y)
//             maxp.y = last_p[i].y;
//     }

//     // if (split)
//     // {
//     //     this->createSprite(abs(maxp.x - minp.x) + 1, abs(maxp.y - minp.y) + 1);
//     //     this->sprite->setOrigin(-minp.x, -minp.y);
        
//     //     sprite->setTextWrap(last_textWrap);
//     //     Vector pos = Vector(last_textOffSet.x, last_textOffSet.y, -last_Size.z/2);
//     //     toFOV_XY(&pos, *last_FOV, last_o, o);
//     //     for(uint8_t i = 0; i < 3; i++)
//     //     {
//     //         pos.y += 15*(last_textSize + 1)*(int)(i>0);
//     //         print(last_Text[i], pos, (1-last_o.z/200)*(last_textSize), *BG_COL);
//     //     }
//     //     /*this->print(last_Text[0], Vector2(pos), (1-last_o.z/200)*last_textSize, *BG_COL);
//     //     this->print(last_Text[1], Vector2(pos.x, pos.y + 15*(last_textSize + 1)), (1-last_o.z/200)*last_textSize, *BG_COL);
//     //     this->print(last_Text[2], Vector2(pos.x, pos.y + 30*(last_textSize + 1)), (1-last_o.z/200)*last_textSize, *BG_COL);
//     //     */drawCubeFaces(last_p, *BG_COL, false);

//     //     sprite->pushSprite(o.x + minp.x, o.y + minp.y, TFT_TRANSPARENT);
//     //     sprite->deleteSprite();
//     //     minp.Equals(999,999), maxp.Equals(-999,-999);
//     // }

//     Vector p[n];
//     p[0].Equals(-Size.x/2, -Size.y/2, Size.z/2);
//     p[1].Equals(Size.x/2, -Size.y/2, Size.z/2);
//     p[2].Equals(Size.x/2, Size.y/2, Size.z/2);
//     p[3].Equals(-Size.x/2, Size.y/2, Size.z/2);
//     p[4].Equals(-Size.x/2, -Size.y/2, -Size.z/2);
//     p[5].Equals(Size.x/2, -Size.y/2, -Size.z/2);
//     p[6].Equals(Size.x/2, Size.y/2, -Size.z/2);
//     p[7].Equals(-Size.x/2, Size.y/2, -Size.z/2);
    
//     for (int i =0; i<n;i++)
//     {
//         p[i].Rotate(Angle);
//         p[i].Round();
//         toFOV_XY(&p[i], *FOV, o);
//         p[i].Round();

//         if(p[i].x < minp.x)
//             minp.x = p[i].x;
//         else if(p[i].x > maxp.x)
//             maxp.x = p[i].x;
//         if(p[i].y < minp.y)
//             minp.y = p[i].y;
//         else if(p[i].y > maxp.y)
//             maxp.y = p[i].y;
//     }
    
//     //draw
//     //this->createSprite(abs(maxp.x - minp.x) + 1, abs(maxp.y - minp.y) + 1);
//     this->sprite->setOrigin(-minp.x, -minp.y);

//     if (!split)
//     {
//         sprite->setTextWrap(last_textWrap);
//         Vector pos = Vector(last_textOffSet.x, last_textOffSet.y, -last_Size.z/2);
//         toFOV_XY(&pos, *last_FOV, last_o, o);
//         for(uint8_t i = 0; i < 3; i++)
//         {
//             pos.y += 15*(textSize + 1)*(int)(i>0);
//             print(last_Text[i], pos, (1-last_o.z/200)*(last_textSize), *BG_COL);
//         }
//         drawCubeFaces(last_p, *BG_COL, false);
//     }
//     drawCubeFaces(p, Color, true);
//     sprite->setTextWrap(textWrap);
//     Vector pos = Vector(textOffSet.x, textOffSet.y, -Size.z/2);
//     toFOV_XY(&pos, *FOV, o);
//     for(uint8_t i = 0; i < 3; i++)
//     {
//         pos.y += 15*(textSize + 1)*(int)(i>0);
//         print(Text[i], pos, (1-o.z/200)*(textSize), textColor);
//     }
//     /*this->print(Text[0], Vector2(pos.x, pos.y), (1-o.z/200)*textSize, textColor);
//     this->print(Text[1], Vector2(pos.x, pos.y + 15*(textSize + 1)), (1-o.z/200)*textSize, textColor);
//     this->print(Text[2], Vector2(pos.x, pos.y + 30*(textSize + 1)), (1-o.z/200)*textSize, textColor);*/
        
//     //sprite->pushSprite(o.x + minp.x, o.y + minp.y, TFT_TRANSPARENT);
//     //sprite->deleteSprite();

//     endDraw();
// }

// void Entity::print(String s, Vector2 pos, float z, uint32_t c)
// {
//     if (s == "")
//         return;
//     this->sprite->setTextColor(c);
//     if (z == 0)
//     {
//         sprite->setTextSize(1);
//         this->sprite->drawString(s, pos.x, pos.y, 1);
//         return;
//     }
//     uint8_t font = 1;
//     sprite->setTextSize((int)z + 1);
//     sprite->drawString(s, pos.x + int((z - int(z))*2) * (int)(z + 1), pos.y, font);
//     sprite->drawString(s, pos.x - int((z - int(z))*2) * (int)(z + 1), pos.y, font);
//     sprite->drawString(s, pos.x, pos.y + int((z - int(z))*2) * (int)(z + 1), font);
//     sprite->drawString(s, pos.x, pos.y - int((z - int(z))*2) * (int)(z + 1), font);
    
// }

// void Entity::drawString()
// {
//     if (!isToUpdate())
//         return;

//     Vector o = O.Delta(Camera.x, Camera.y);
//     Vector last_o = last_O.Delta(last_Camera.x, last_Camera.y);
//     o.Round();
//     last_o.Round();
//     Size.Round();
//     int deltaX = (int)(o.x - last_o.x);
//     int deltaY = (int)(o.y - last_o.y);

//     //split render to 2 sprites
//     bool split = abs(deltaX) > Size.x || abs(deltaY) > Size.y;
//     if (split)
//     {
//         createSprite(last_Size.x, last_Size.y);
//         sprite->setOrigin(last_Size.x/2, 0);
        
//         sprite->fillSprite(*BG_COL);
//         //sprite->setTextWrap(last_textWrap);
//         //Vector pos = Vector(last_textOffSet.x /*- (last_o.x - o.x)*/, last_textOffSet.y /*- (last_o.y - o.y)*/);
//         //for(uint8_t i = 0; i < 3; i++)
//         {
//             //pos.y += 15*last_textSize*i;
//             //print(last_Text[i], pos, (1-last_o.z/200)*last_textSize, *BG_COL);
//         }
        
//         sprite->pushSprite(last_o.x - last_Size.x/2, last_o.y - last_Size.y/2, TFT_TRANSPARENT);
//         sprite->deleteSprite();
//     }  

//     //draw
//     createSprite(Size.x + abs(deltaX), Size.y + abs(deltaY));
//     if (deltaX < 0)
//         deltaX = 0;
//     if (deltaY < 0)
//         deltaY = 0;
//     sprite->setOrigin(Size.x/2 + deltaX, deltaY);
//     if (!split)
//     {
//         sprite->fillSprite(*BG_COL);
//         /*sprite->setTextWrap(last_textWrap);
        
//         Vector pos = Vector(last_textOffSet.x + int(last_o.x - o.x), last_textOffSet.y + int(last_o.y - o.y));
//         for(uint8_t i = 0; i < 3; i++)
//         {
//             pos.y += 15*last_textSize*(int)(i>0);
//             print(last_Text[i], pos, (1-last_o.z/200)*last_textSize, *BG_COL);
//         }*/
//     }
//     sprite->setTextWrap(textWrap);
//     Vector pos = Vector(textOffSet.x, textOffSet.y);
//     //toFOV_XY(&pos, *FOV, o);
//     for(uint8_t i = 0; i < 3; i++)
//     {
//         pos.y += 15*textSize*(int)(i>0);
//         print(Text[i], pos, (1-o.z/200)*textSize, textColor);
//     }
//     sprite->pushSprite(o.x - (Size.x/2) - deltaX, o.y - (Size.y/2) - deltaY, TFT_TRANSPARENT);
//     sprite->deleteSprite();

//     endDraw();
// }

// void Entity::drawCircle()
// {
//     if (!isToUpdate())
//         return;

//     Vector o = O.Delta(Camera.x, Camera.y);
//     Vector last_o = last_O.Delta(last_Camera.x, last_Camera.y);
//     o.Round();
//     last_o.Round();
//     Size.Round();
//     int deltaX = (int)(o.x - last_o.x);
//     int deltaY = (int)(o.y - last_o.y);

//     bool split = abs(deltaX) > Size.x || abs(deltaY) > Size.y;
//     if (split)
//     {
//         createSprite(last_Size.x, last_Size.x);
//         sprite->setOrigin(last_Size.x/2, last_Size.x/2);
        
//         sprite->fillSprite(*BG_COL);
        
//         sprite->pushSprite(last_o.x - last_Size.x/2, last_o.y - last_Size.x/2, TFT_TRANSPARENT);
//         sprite->deleteSprite();
//     }

//     //draw
//     createSprite(Size.x + abs(deltaX), Size.y + abs(deltaY));
//     if (deltaX < 0)
//         deltaX = 0;
//     if (deltaY < 0)
//         deltaY = 0;
//     sprite->setOrigin(Size.x/2 + deltaX, deltaY);
//     if (!split)
//         sprite->fillSprite(*BG_COL);

//     Vector pos = Vector(textOffSet.x, textOffSet.y);
//     toFOV_XY(&pos, *FOV, o);
//     sprite->fillCircle(0,0,Size.x/2, Color);

//     sprite->pushSprite(O.x - (Size.x/2), O.y - (Size.x/2), TFT_TRANSPARENT);
//     sprite->deleteSprite();
//     endDraw();
// }