#include "debug.h"

uint16_t Debug::frameCalls = 0;
uint16_t Debug::physicsCalls = 0;

unsigned long Debug::pushTime = 0;
unsigned long Debug::pushTimeSum = 0;

unsigned long Debug::prerenderTime = 0;
unsigned long Debug::prerenderTimeSum = 0;
uint16_t Debug::renderCallsCount = 0;

unsigned long Debug::renderTime = 0;
unsigned long Debug::renderTimeSum = 0;
