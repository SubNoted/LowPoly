#ifndef SCENE_H
#define SCENE_H

// #include "lowPoly.h"
#include <vector>

#include <render/renderer.h>
#include "settings.h"

class SceneManager; // Forward declaration

class Scene {
public:

    virtual ~Scene() {}
    virtual void update(uint32_t deltaTime) = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;

    
    std::vector<Entity*> entities = std::vector<Entity*>(); //todo entity accessable across scenes
    
protected:

    SceneManager* sceneManager;
    friend class SceneManager;
    void setSceneManager(SceneManager* manager) { sceneManager = manager; }
  
};

#endif