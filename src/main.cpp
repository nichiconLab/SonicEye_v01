#include <Arduino.h>
#include "System.h"

void setup() {
  // put your setup code here, to run once:
  System_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  System_Loop();
  //System_Loop2();
}
