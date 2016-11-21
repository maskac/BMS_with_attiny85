#include "vcc.h"
#include <SoftwareSerial.h>
SoftwareSerial SW_Serial(3,4); 

void setup() {
  //enable software serial on 9600 for debuging
  SW_Serial.begin(9600);  
}

void loop() {
  // temporary variable for reading
  float x;
  // read Vcc voltage with quadratic correction
  x=readVccpow(-0.045,1.1882,-0.1432,50);
  // print readed voltage in V
  SW_Serial.println(x);
  // pause 500ms
  delay(500);
  
}
