
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
    delayMicroseconds(100);  //500
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
    //dt = micros() - t0;
    return dt;
}

RadarInfo_t Radar_Scan2(uint8_t deg){
    RadarInfo_t result;
    uint16_t ServoDuty = deg*10.6+500;
    ServoWrite(ServoDuty);
    delay(50);
    digitalWrite(RADAR_TX_PIN, HIGH);
    uint32_t t0 = micros();
    delayMicroseconds(100);  //250us 10pulses
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
    /*
    while(digitalRead(RADAR_RX_PIN)){
        dt = micros() - t0;
        if(dt > 150){  //150
            dt = 0;
            break;
        }
        delayMicroseconds(1);
    }
    */
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
    /*
    for(uint8_t n=0; n<3; n++){
        while(digitalRead(RADAR_RX_PIN)){
            dt = micros() - t0;
            if(dt > 150){
                dt = 0;
                break;
            }
        }
        while(!digitalRead(RADAR_RX_PIN)){
            dt = micros() - t0;
            if(dt > 150){
                dt = 0;
                break;
            }
        }
    }
    result.signalWidth = dt;
    */
    return result;
}


void Radar_Init3(){
    ledcSetup(LEDC_CHANNEL_0, 50, 16);
    ledcAttachPin(SRV_PWM_PIN, LEDC_CHANNEL_0);
    ServoWrite(500);

    pinMode(RADAR_TX_PIN, OUTPUT);
    digitalWrite(RADAR_TX_PIN, LOW);
    pinMode(RADAR_RX_PIN, INPUT);

    pcnt_config_t configA;
    configA.pulse_gpio_num = RADAR_RX_PIN;
    configA.ctrl_gpio_num = PCNT_PIN_NOT_USED;
    configA.lctrl_mode = PCNT_MODE_REVERSE;
    configA.hctrl_mode = PCNT_MODE_KEEP;
    configA.channel = PCNT_CHANNEL_0;
    configA.unit = PCNT_UNIT_0;
    configA.pos_mode = PCNT_COUNT_DEC;
    //configA.pos_mode = PCNT_COUNT_INC;
    configA.neg_mode = PCNT_COUNT_DIS;
    configA.counter_h_lim = 32768;
    configA.counter_l_lim = -32768;

    pcnt_unit_config(&configA);
    pcnt_counter_pause(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);
    /*
    digitalWrite(RADAR_TX_PIN, HIGH);
    delayMicroseconds(100);  //40 pulses
    digitalWrite(RADAR_TX_PIN, LOW);
    pcnt_counter_resume(PCNT_UNIT_0);
    */
}

RadarInfo_t Radar_Scan3(uint8_t deg){
    RadarInfo_t result;
    uint16_t ServoDuty = deg*10.6+500;
    ServoWrite(ServoDuty);
    delay(50);
    digitalWrite(RADAR_TX_PIN, HIGH);
    uint32_t t0 = micros();
    delayMicroseconds(500);  //500, 100
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
    pcnt_counter_clear(PCNT_UNIT_0);
    pcnt_counter_resume(PCNT_UNIT_0);
    result.deltaT = dt;
    delayMicroseconds(250);
    pcnt_counter_pause(PCNT_UNIT_0);
    int16_t count;
    pcnt_get_counter_value(PCNT_UNIT_0, &count);
    pcnt_counter_pause(PCNT_UNIT_0);
    Serial.println(count);
    //result.signalWidth = dt*2;
    result.signalWidth = (count/10)*500;
    return result;
}
