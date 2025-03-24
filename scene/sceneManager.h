#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H


#include <memory>
#include "scene.h"

#include <render/renderer.h>
#include <render/basicStrategy.h>

#include "debug.h"

class SceneManager {
public:

    // static void init();
    
    void init();

    void changeScene(std::shared_ptr<Scene> newScene);
    void render();
    void exit();

    std::shared_ptr<Scene> currentScene = nullptr;

private:

    GraphicsRenderer renderer;

    ////////tools/////////
    TaskHandle_t updateBufferTaskHandle = NULL;
    TaskHandle_t updatePhysicsTaskHandle = NULL;


};


#endif // SCENE_MANAGER_H
