/*
 * This sketch is inspired by the work of eran.io
 * http://eran.io/rotary-encoder-based-cooking-timer/
 * Here's code: https://github.com/pavius/rotary-encoder-timer
 */
#include "eran_encoder.h"

EranEncoder encoder;
int eValue, ePrevValue;

// SETUP ///////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  encoder.init(2,3);
}

// LOOP ////////////////////////////////////////////////////////////////
void loop() 
{
  encoder.update();
  eValue = encoder.getCounter();
  if( eValue != ePrevValue )
  {
    Serial.println( eValue );
    ePrevValue = eValue;
  }
  delay(10); 
}
