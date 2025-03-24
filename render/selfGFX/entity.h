#ifndef _ENTITY_
#define _ENTITY_

// #include <lowPoly.h>

// #include "vector.h"
#include "polygon.h"
// #include <devSetup.h>
// #include "vector2.h"
// #include "matrix3.h"
// #include "color.h"
// #include "texture.h"
// #include "material.h"

// #include "scene/scene.h"

class Entity
{

protected:
    friend class GraphicsRenderer;
    friend class IRendererStrategy;
    friend class BasicRendererStrategy;
    
    Vector_16* vertices_b; //buffer

    Vector* vertices;
    Vector2_u8* textureCoords;
    Vector* normals;


    Polygon* polygons;

    int16_t vertLength = 0;
    int16_t polyLength = 0;

    uint8_t *texture;
    uint8_t *textureWidths;
    uint16_t *textureLengths;

    uint8_t texturesLength;
    uint32_t textureLength;

private:

    // static Vector2 Camera;
    // static Vector2 last_Camera;

    // Vector last_O;
    // Vector last_Size;
    // Vector last_Angle;
    //uint16_t Color;
    // Vector2 last_textOffSet;
    // String last_Text[3];
    // float last_textSize;
    // bool last_textWrap;
    //uint16_t textColor;
    
    // bool firstTouched;

    Vector2 minp, maxp; //for render


    
    bool Update;

    bool isToUpdate();
    
    void createSprite(uint16_t x, uint16_t y);
    void drawCubeFaces(Vector* p, uint32_t c, bool light);
    void print(String s, Vector2 pos, float siz, uint32_t c);

    void endDraw();

    void setCube();

public:
    // static uint16_t SCREEN_WIDTH;
    // static uint16_t SCREEN_HEIGHT;

    Vector O = Vector();
    float Size = 1;
    Vector Angle;

    //Texture

    uint16_t Color; //for WO texture


    // String Text[3];
    // Vector2 textOffSet;
    // float textSize;
    // bool textWrap;
    // uint16_t textColor;

    //Physics
    Vector V;
    Vector constA;
    Vector lightDirection;

    Entity();
    Entity(std::vector<Entity*>& entities);
    void init(std::vector<Entity*>& entities);
    void reInit(std::vector<Entity*>& entities);

    void createCube(Vector o, Vector size);

    void createModel(const float v[][3], const uint8_t vt[][2], const float vn[][3], const uint16_t &vLength, const uint16_t f[][10], const uint16_t &fLength, const uint8_t* textures, const uint8_t* texWidths, const uint16_t *texLengths, const uint32_t &texLength, const uint8_t &texsLength);
    
    
    void updateBuffer();

    //set
    void setPosition(Vector v);
    void setSize(Vector v);
    void setRotation(Vector v);//todo fix
    void rotate(Vector v);
    void setColor(uint16_t c);
    // void setText(String s, uint8_t n);
    // void setTextOffSet(Vector2& v);
    // void setTextOffSet(const int16_t& x, const int16_t& y);
    // void setTextSize(float x);
    // void setTextWrap(bool b);
    // void setTextColor(uint16_t c);
    //get
    Vector getPosition();
    Vector getSize();
    Vector getAngle();
    uint16_t getColor();
    // String getText(uint8_t n);
    // Vector2 getTextOffSet();
    // float getTextSize();
    // bool getTextWrap();
    // uint16_t getTextColor();

    static Entity* ALL_ENTITIES[64];
    static uint8_t ENTITIES_COUNT;
    // static uint8_t datum; //fix
    // static void setCameraPos(const Vector &v);
    // static void plusCameraPos(const Vector &v);
    // static Vector2 getCameraPos();
    // static void endcameraFrame();
    static void processAllEntities(float &t);
    static void updateALL();


    static void firstTouchAllEntities(const Vector2 &first_touch);
    static bool isFirstTouchedAnyEntity();
    bool isFirstTouched();
    
    // static void initALL(TFT_eSprite *s, uint16_t screen_width, uint16_t screen_height, float *fov, float *last_fov, uint16_t *bg_col);
    // static void initALL(std::vector<> *s, uint16_t screen_width, uint16_t screen_height, float *fov, float *last_fov, uint16_t *bg_col);
    static void reItitAll();
    static void deleteALL();
    void processPhysics(float &t);

    void deleteEntity();

    void drawCube();
    void drawCube(float a);
    void drawParallelepiped();
    void drawParallelepiped(Vector v);

    void drawString();

    void drawCircle();

    bool isTouched(Vector2 touch);
    
};

#endif