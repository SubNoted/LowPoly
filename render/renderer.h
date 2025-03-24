#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "iRendererStrategy.h"

class GraphicsRenderer {
public:

    // GraphicsRenderer();

    void init();

    void IRAM_ATTR renderScene(std::vector<Entity*>& entities);
    
    void setStrategy(std::unique_ptr<IRendererStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void setGlobalLightDirection(Vector& lightDirection) {
        strategy->lightDirection = lightDirection;//setGlobalLightDirection(lightDirection);
    }

    void setFOV(float& newFOV) {
        strategy->FOV = newFOV;
    }

    std::unique_ptr<IRendererStrategy> strategy = nullptr;
    
private:

    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite canvas[5] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)}; //todo fix to split_scrn
    uint16_t* cnvsPtr[SPLIT_SCREEN];
};

#endif // GRAPHICS_RENDERER_H