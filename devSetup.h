#ifndef DEV_SETUP_H
#define DEV_SETUP_H

#include <Arduino.h>

#define DEBUG_MODE true

//////////////////microcontroller/////////////////

// #define ESP32_ON
#define ESP32_S3_ON

//////////////////display info/////////////////
#define SCRN_WIDTH 240
#define SCRN_HEIGHT 240

#define CENTR_X SCRN_WIDTH / 2
#define CENTR_Y SCRN_HEIGHT / 2

#define SCRN_ROTATION 2 // 0: landscape
#define SPLIT_SCREEN 1 //up tp 5 //todo fix

#define DEFAULT_FOV PI*0.5
#define DEFAULT_RENDER_DISTANCE 100 //todo think about metrics

/////////////////////saves//////////////////////
struct saveFile 
{
    //change as you wish
    char userName[16] = "NOMEMO";
    
    uint8_t difficulty = 0;

    uint8_t stage = 0;
    uint8_t progress = 0;
};

#endif