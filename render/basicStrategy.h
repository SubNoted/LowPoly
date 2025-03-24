#ifndef BASIC_RENDER_STRATEGY
#define BASIC_RENDER_STRATEGY

#include "renderer.h"
#include "debug.h"

class BasicRendererStrategy : public IRendererStrategy {
public:
    void renderScene(std::vector<Entity*>& entities, TFT_eSPI& tft, TFT_eSprite* canvas, uint16_t** cnvsPtr) override;

    void IRAM_ATTR renderPolygons(std::vector<Entity*>& entities, TFT_eSPI& tft, TFT_eSprite* canvas, uint16_t** cnvsPtr); 

    uint8_t* z_buffer;
    BasicRendererStrategy()
    {
        z_buffer = (uint8_t*)heap_caps_calloc(SCRN_WIDTH*SCRN_HEIGHT, sizeof(uint8_t), MALLOC_CAP_INTERNAL);
    };

    void pushColorTriangleToCanvas(Vector2_16 v0, Vector2_16 v1, Vector2_16 v2, \
                                Vector vn0, Vector vn1, Vector vn2, \
                               const Vector &lightDir, uint16_t* _img, const uint16_t &color);

    void IRAM_ATTR pushImageTriangleToCanvas(Vector_16 v0, Vector_16 v1, Vector_16 v2, \
                                Vector2_u8 uv0, Vector2_u8 uv1, Vector2_u8 uv2, \
                                int16_t light0, int16_t light1, int16_t light2, \
                                uint16_t* _img, uint8_t* data, uint8_t texWeight);
};

#endif // BASIC_RENDER_STRATEGY