
#include <Radar.h>

void ServoWrite(uint16_t us){
    uint16_t val = us*3.277;
    ledcWrite(LEDC_CHANNEL_0, val);
}

void Radar_Init(){
    ledcSetup(LEDC_CHANNEL_0, 50, 16);
    ledcAttachPin(SRV_PWM_PIN, LEDC_CHANNEL_0);
    ServoWrite(500);

    pinMode(RADAR_TX_PIN, OUTPUT);
    digitalWrite(RADAR_TX_PIN, LOW);
    pinMode(RADAR_RX_PIN, INPUT);
}

uint16_t Radar_Scan(uint8_t deg){
    uint16_t ServoDuty = deg*10.6+500;
    ServoWrite(ServoDuty);
    delay(50);
    digitalWrite(RADAR_TX_PIN, HIGH);
    uint32_t t0 = micros();
    delayMicroseconds(100);
    digitalWrite(RADAR_TX_PIN, LOW);
    uint16_t dt;
    while(!digitalRead(RADAR_RX_PIN)){
        dt = micros() - t0;
        if(dt > 10000){
            dt = 0;
            break;
        }
        delayMicroseconds(1);
    }
    
    return dt;
}

RadarInfo_t Radar_Scan2(uint8_t deg){
    RadarInfo_t result;
    uint16_t ServoDuty = deg*10.6+500;
    ServoWrite(ServoDuty);
    delay(50);
    digitalWrite(RADAR_TX_PIN, HIGH);
    uint32_t t0 = micros();
    delayMicroseconds(100);
    digitalWrite(RADAR_TX_PIN, LOW);
    delayMicroseconds(10);
    uint16_t dt;
    while(!digitalRead(RADAR_RX_PIN)){
        dt = micros() - t0;
        if(dt > 10000){
            dt = 0;
            break;
        }
        delayMicroseconds(1);
    }
    t0 = micros();
    result.deltaT = dt;
    
    while(digitalRead(RADAR_RX_PIN)){
        dt = micros() - t0;
        if(dt > 50){
            dt = 0;
            break;
        }
    }
    while(!digitalRead(RADAR_RX_PIN)){
        dt = micros() - t0;
        if(dt > 50){
            dt = 0;
            break;
        }
    }
    result.signalWidth = dt*4;
    
    return result;
}
