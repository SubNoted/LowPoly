#include "SceneManager.h"
#include "esp_task_wdt.h"

void updatePhysics(void * pvParameter) 
{
    Scene* currentScene = (Scene*)pvParameter;

    uint32_t deltaTime, last4deltaTime = millis();
    for (;;) 
    {
#if DEBUG_MODE
        Debug::physicsCalls++;
#endif
        deltaTime = millis() - last4deltaTime;
        if (deltaTime > 100)
            deltaTime = 100;
        last4deltaTime = millis();

        if (currentScene) {
            currentScene->update(deltaTime);
        }
        vTaskDelay(1);
    }
}

void updateBuffer(void * pvParameter) 
{
    Scene* currentScene = (Scene*)pvParameter;
    for (;;) 
    {
        if (currentScene)
        {
            for (uint16_t i = 0; i < currentScene->entities.size(); i++) //todo test
            {
                currentScene->entities[i]->updateBuffer();
            }
        }
        vTaskDelay(1);
    }   
}

void log_func(void * pvParameter) 
{
    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // log_d("FPS: %d", Debug::frameCalls);
        // log_d("PhysCallsPS: %d", Debug::physicsCalls);
        Serial.printf("FPS: %d\n", Debug::frameCalls);
        Serial.printf("PhysCallsPS: %d\n", Debug::physicsCalls);

        if (Debug::frameCalls > 0) 
        {
            // log_d("Push time: %lumcs", Debug::pushTimeSum/Debug::frameCalls);
            // // log_d("Render time: %dmcs", Debug::renderTimeSum/Debug::renderTimeCount);
            // log_d("PreRender time: %dmcs", Debug::prerenderTimeSum/Debug::frameCalls);
            // log_d("Render time: %dmcs", Debug::renderTimeSum/Debug::frameCalls);
            Serial.printf("Push time: %lumcs\n", Debug::pushTimeSum/Debug::frameCalls);
            // Serial.printf("Render time: %dmcs", Debug::renderTimeSum/Debug::renderTimeCount);
            Serial.printf("PreRender time: %dmcs\n", Debug::prerenderTimeSum/Debug::frameCalls);
            Serial.printf("Render time: %dmcs\n", Debug::renderTimeSum/Debug::frameCalls);
        } 
        // log_d("Render calls: %d", Debug::renderTimeCount);
        
        // log_d("Memory info\n\tPSRAM - \tTotal: %u,\tUsed: %u,\tFree: %u,\tBiggest free block: %u\n\tInternal - \tTotal: %u,\tUsed: %u,\tFree: %u,\tBiggest free block: %u\n\n\n", 
        //     heap_caps_get_total_size(MALLOC_CAP_SPIRAM),
        //     heap_caps_get_total_size(MALLOC_CAP_SPIRAM)-heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        //     heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        //     heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
        //     heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
        //     heap_caps_get_total_size(MALLOC_CAP_INTERNAL)-heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        //     heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        //     heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL)
        // );
        Serial.printf("Memory info\n\tPSRAM - \tTotal: %u,\tUsed: %u,\tFree: %u,\tBiggest free block: %u\n\tInternal - \tTotal: %u,\tUsed: %u,\tFree: %u,\tBiggest free block: %u\n\n\n\n", 
            heap_caps_get_total_size(MALLOC_CAP_SPIRAM),
            heap_caps_get_total_size(MALLOC_CAP_SPIRAM)-heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
            heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
            heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
            heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
            heap_caps_get_total_size(MALLOC_CAP_INTERNAL)-heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
            heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
            heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL)
        );

        Debug::frameCalls = 0;
        Debug::physicsCalls = 0;
        Debug::pushTimeSum = 0;
        Debug::renderTimeSum = 0;
        Debug::prerenderTimeSum = 0;
        Debug::renderCallsCount = 0;
        // esp_task_wdt_reset();
    }
}

void SceneManager::init()
{
    
#if DEBUG_MODE
    log_d("Memory info (before initialization)\nPSRAM - \tTotal: %u,\tFree: %u,\tBiggest free block: %u\nInternal  - \tTotal: %u,\tFree: %u,\tBiggest free block: %u\n", 
        heap_caps_get_total_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
        heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));

#endif

    renderer.init();
    renderer.setStrategy(std::unique_ptr<IRendererStrategy>(new BasicRendererStrategy()));

#if DEBUG_MODE
    xTaskCreatePinnedToCore(log_func, "log_func", 2048, NULL, 5, NULL, 0);
#endif 
}

void SceneManager::changeScene(std::shared_ptr<Scene> newScene) {
    if (currentScene) {
        currentScene->entities.clear();
        currentScene->exit();
    }
    currentScene = std::move(newScene);

    currentScene->entities.reserve(1);

    currentScene->setSceneManager(this);
    currentScene->enter();

    if (updateBufferTaskHandle != NULL)
    {
        vTaskDelete(updateBufferTaskHandle);
        vTaskDelete(updatePhysicsTaskHandle);
        updateBufferTaskHandle = NULL;
        updatePhysicsTaskHandle = NULL;
    }
    
    xTaskCreatePinnedToCore(updateBuffer, "updateBuffer", 2048, (currentScene.get()), 5, &updateBufferTaskHandle, 0);
    xTaskCreatePinnedToCore(updatePhysics, "updatePhysics", 2048, (currentScene.get()), 5, &updatePhysicsTaskHandle, 0);
}


void SceneManager::render() {
#if DEBUG_MODE
    Debug::pushTime = micros();
#endif 

    //render
    if (currentScene) {
        renderer.renderScene(currentScene->entities);
    }

#if DEBUG_MODE    
    Debug::pushTimeSum += micros() - Debug::pushTime;
#endif
}