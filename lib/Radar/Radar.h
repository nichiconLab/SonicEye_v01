
#ifndef RADAR_H
#define RADAR_H

#include <Arduino.h>
#include <driver/ledc.h>
#include <driver/pcnt.h>

#define SRV_PWM_PIN  16
#define RADAR_TX_PIN 17
#define RADAR_RX_PIN 35

typedef struct{
    uint16_t deltaT;
    uint16_t signalWidth;
}RadarInfo_t;

void Radar_Init();
uint16_t Radar_Scan(uint8_t deg);
RadarInfo_t Radar_Scan2(uint8_t deg);
void Radar_Init3();
RadarInfo_t Radar_Scan3(uint8_t deg);


#endif