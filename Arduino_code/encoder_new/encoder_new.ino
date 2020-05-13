// SERIAL STUFF ////////////////////////////////////////////////////////
bool bSendSerial = true;

// ENCODER STUFF /////////////////////////////////////////////////////// 
// Encoder code is inspired by the work of eran.io
// http://eran.io/rotary-encoder-based-cooking-timer/
// Here's code: https://github.com/pavius/rotary-encoder-timer

#include "eran_encoder.h"

EranEncoder encoder;

void cwStep(void){
  if( bSendSerial) { Serial.print( "CW halfStep: "); Serial.println( encoder.getCounter() );}
}

void ccwStep(void){
  if( bSendSerial) { Serial.print( "CCW halfStep: "); Serial.println( encoder.getCounter() );}
}

// BUTTON STUFF ////////////////////////////////////////////////////////
// (ButtonDebounce Library is written and maintained 
// by Maykon L. Capellari <maykonluiscapellari@gmail.com>
// https://github.com/maykon/ButtonDebounce

#include "ButtonDebounce.h"

ButtonDebounce push(10, 50);

void pushChanged(int state){
  if( !state )
  {
    if( bSendSerial) Serial.write('c');
  }
}

// SETUP ///////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  while( !Serial ) {};
  encoder.init(0,1);
  encoder.setCallbackCW(cwStep);
  encoder.setCallbackCCW(ccwStep);

  push.setCallback(pushChanged);
}

// LOOP ////////////////////////////////////////////////////////////////
void loop() 
{  
  encoder.update();
  push.update();

  delay(5); 
}
