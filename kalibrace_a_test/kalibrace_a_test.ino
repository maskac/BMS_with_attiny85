#define LIN_CORR_MUL 0.848
#define LIN_CORR_ADD 0.4235

#define POL_CORR_L3 -0.045
#define POL_CORR_L2 1.1882
#define POL_CORR_L1 -0.1432

#include <SoftwareSerial.h>
SoftwareSerial Serial(3,4);   

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;
  //result = 1126400L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                 // set up serial output
}

void loop() {
  // put your main code here, to run repeatedly:
  float x; int i;
  float xlin,xpow;
  for(i=1;i<=100;i++){
    
    x+=(readVcc()/1000.0);
  }
  Serial.print(readVcc()/1000.0);
  Serial.print(" Sum:");
  Serial.println(x);
  x/=100.0;
  Serial.print("Raw Vcc:");
  Serial.println(x);
  Serial.print("Corrected Vcc:");
  xlin=x;
  xlin*=LIN_CORR_MUL;
  xlin+=LIN_CORR_ADD;
  Serial.println(xlin);
  xpow=POL_CORR_L3*x*x;
  xpow+=POL_CORR_L2*x;
  xpow+=POL_CORR_L1;
  Serial.print("Corrected power Vcc:");
  Serial.println(xpow);
  
  
  delay(500);
}
