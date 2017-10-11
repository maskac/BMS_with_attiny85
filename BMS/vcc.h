// read 1.1V reference computed back to Vcc voltage. single raw data
long readVcc();

float readVccpow(float kof1, float kof2, float kof3, int presnost);
// quadratic corection of readed Vcc
// Y=AX^2 + BX + C  where A is kof1 , B is kof2, C is kof3
// presnost is number of reades for average ADC 
float readVccpow(float kof1, float kof2, float kof3, int presnost)
{
    int i;
    float rawVoltage,corrVoltage;

    for(i=1;i<=presnost;i++){    //read n-times for better accurance
    rawVoltage+=(readVcc()/1000.0);
    }
    
    //calculate average
    rawVoltage/=(float)presnost;
    
   //calucate quadratic correction
   corrVoltage=kof1*rawVoltage*rawVoltage;
   corrVoltage+=kof2*rawVoltage;
   corrVoltage+=kof3;

    //return corrected voltage
   return corrVoltage;
}

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
  
  //for attiny85 maybe from some sources 1024 not 1023
  //atmega 1023 is ok
 //result = 1126400L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1024*1000
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  
  return result; // Vcc in millivolts
}
