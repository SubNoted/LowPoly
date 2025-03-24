#ifndef I_RENDER_STRATEGY_H
#define I_RENDER_STRATEGY_H


#include <TFT_eSPI.h> 
#include <vector>

#include "selfGFX/entity.h"
#include "scene/settings.h"

class IRendererStrategy {
public:
    virtual ~IRendererStrategy() = default;
    virtual IRAM_ATTR void renderScene(std::vector<Entity*>& entities, TFT_eSPI& tft, TFT_eSprite* canvas, uint16_t** cnvsPtr) = 0;
    
    Vector_16 lightDirection = Vector_16(0,-128,-128).getNormalized(); //todo fix
    float FOV = Settings::fov;
};

#endif // I_RENDER_STRATEGY_H