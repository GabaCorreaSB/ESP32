#include "RoboCore_MMA8452Q.h"

MMA8452Q acel;

void setup() {

  Serial.begin(115200);

  acel.init();

}

void loop() {

  acel.read();

  if((acel.x > 0.40) || (acel.y > 0.40) || (acel.z < 0.80)){
    Serial.println("ALERTA! MOVIMENTO DETECTADO");
  }

}
