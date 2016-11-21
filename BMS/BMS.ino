#include "vcc.h"
#include <SoftwareSerial.h>
SoftwareSerial Serial(3,4); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  float x;
  x=readVccpow(-0.045,1.1882,-0.1432,50);
  Serial.println(x);
  delay(500);
  
}
