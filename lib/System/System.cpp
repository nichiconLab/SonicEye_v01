
#include "System.h"


void System_Init(){
    M5.begin();
    Radar_Init();
    //Radar_Init3();
    M5.Lcd.drawCircle(160, 240, 150, CYAN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(ORANGE);
    //M5.Lcd.print("Range: 1.0m");
}

static float ScanRange = 1.0;
static float NewRange = 1.0;
static float DisplayScale = 150;
static int16_t deg = 0;

void System_Loop(){
    static bool direction;
    static bool PauseFlag;
    M5.update();
    if(M5.BtnA.wasPressed()){
        if(NewRange > 0.5){
            NewRange -= 0.5;
            //DisplayScale = 150/NewRange;
        }
    }
    if(M5.BtnB.wasPressed()){
        if(PauseFlag == false){
            PauseFlag = true;
        }else{
            PauseFlag = false;
        }
    }
    if(M5.BtnC.wasPressed()){
        if(NewRange < 3.0){
            NewRange += 0.5;
            //DisplayScale = 150/NewRange;
        }
    }
    //M5.Lcd.fillRect(0, 0, 140, 25, BLACK);
    //M5.Lcd.setCursor(0, 0);
    //M5.Lcd.printf("Range: %3.1fm", NewRange);
    if(NewRange != ScanRange){
        M5.Lcd.fillRect(0, 0, 250, 25, BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.printf("Range: %3.1fm -> %3.1fm", ScanRange, NewRange);
    }else{
        M5.Lcd.fillRect(0, 0, 250, 25, BLACK);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.printf("Range: %3.1fm", ScanRange);
    }

    uint16_t t = Radar_Scan((uint8_t)deg);
    int32_t x1 = 160+148*cos(deg*(3.14/180));
    int32_t y1 = 240-148*sin(deg*(3.14/180));
    M5.Lcd.drawLine(160, 240, x1, y1, CYAN);
    float dist = 340*((float)t/1000000)/2;
    if(dist > ScanRange){
        dist = 0;
    }
    //Serial.printf("t: %d, dist: %fm\n", t, dist);
    int32_t posX = (int32_t)(160+(dist*cos(radians(deg)))*DisplayScale);
    int32_t posY = (int32_t)(240-(dist*sin(radians(deg)))*DisplayScale);
    int32_t posX2 = (int32_t)(160+(dist*cos(radians(deg-1)))*DisplayScale);
    int32_t posY2 = (int32_t)(240-(dist*sin(radians(deg-1)))*DisplayScale);
    //Serial.printf("t: %d, dist: %fm, x0: %d, y0: %d, x1: %d, y1: %d\n", t, dist, posX, posY, posX2, posY2);
    delay(20);  //15
    M5.Lcd.drawLine(160, 240, x1, y1, BLACK);
    //M5.Lcd.drawPixel(160+posX, 240-posY, GREEN);
    M5.Lcd.drawLine(posX, posY, posX2, posY2, GREEN);
    delay(10);
    /*
    deg++;
    if(deg > 180){
        delay(700);
        deg = 0;
        M5.Lcd.clear();
        M5.Lcd.drawCircle(160, 240, 150, CYAN);
    }
    */
    if(direction == false){
        deg++;
        if(deg > 180){
            ScanRange = NewRange;
            DisplayScale = 150/ScanRange;
            delay(700);
            M5.Lcd.clear();
            M5.Lcd.drawCircle(160, 240, 150, CYAN);
            direction = true;
        }
    }else{
        deg--;
        if(deg < 0){
            ScanRange = NewRange;
            DisplayScale = 150/ScanRange;
            delay(700);
            M5.Lcd.clear();
            M5.Lcd.drawCircle(160, 240, 150, CYAN);
            direction = false;
        }
    }
}

void System_Loop2(){
    int32_t posX, posY, posX2, posY2;
    RadarInfo_t result;
    result = Radar_Scan2(deg);
    //result = Radar_Scan3(deg);
    int32_t x1 = 160+148*cos(deg*(3.14/180));
    int32_t y1 = 240-148*sin(deg*(3.14/180));

    M5.Lcd.drawLine(160, 240, x1, y1, CYAN);
    float dist = 340*((float)result.deltaT/1000000)/2;
    float dist2 = 340*((float)result.signalWidth/1000000)/2;
    if(dist > 0 && dist <= SCAN_RANGE){
        if(dist2 > SCAN_RANGE){
            dist2 = SCAN_RANGE;
        }
        posX = (int32_t)(160+(dist*cos(deg*(3.14/180)))*SCAN_SCALE);
        posY = (int32_t)(240-(dist*sin(deg*(3.14/180)))*SCAN_SCALE);
        posX2 = (int32_t)(posX+(dist2*cos(radians(deg)))*SCAN_SCALE);
        posY2 = (int32_t)(posY-(dist2*sin(radians(deg)))*SCAN_SCALE);
    }
    delay(15);
    M5.Lcd.drawLine(160, 240, x1, y1, BLACK);
    if(result.deltaT > 0){
        M5.Lcd.drawLine(posX, posY, posX2, posY2, GREEN);
    }
    delay(10);
    deg++;
    if(deg > 180){
        delay(700);
        deg = 0;
        M5.Lcd.clear();
        M5.Lcd.drawCircle(160, 240, 150, CYAN);
    }
}
