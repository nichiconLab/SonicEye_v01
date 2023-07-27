
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <M5Stack.h>
#include <math.h>
#include "Radar.h"

//#define SCAN_RANGE (2.0)
//#define SCAN_SCALE 85
#define SCAN_RANGE (1.0)
#define SCAN_SCALE 160

void System_Init();
void System_Loop();
void System_Loop2();


#endif