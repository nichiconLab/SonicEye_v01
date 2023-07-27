
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <M5Stack.h>
#include <math.h>
#include "Radar.h"


#define SCAN_RANGE (1.0)  //1.0m
#define SCAN_SCALE 160    //r = 160pixels

void System_Init();
void System_Loop();
void System_Loop2();


#endif