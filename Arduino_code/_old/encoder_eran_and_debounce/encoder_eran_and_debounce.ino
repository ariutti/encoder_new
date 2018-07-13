#include <ButtonDebounce.h>

/*
 * This sketch is inspired by the work of eran.io
 * http://eran.io/rotary-encoder-based-cooking-timer/
 * Here's code: https://github.com/pavius/rotary-encoder-timer
 */
#include "eran_encoder.h"
#include <ButtonDebounce.h>

EranEncoder encoder;
int eValue, ePrevValue;

ButtonDebounce button(7, 50);
bool buttonState, buttonPrevState = true;


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

  // button stuff
  button.update();

  if( button.state() != buttonPrevState )
  {
    if(button.state() == LOW)
    {
      Serial.println("Clicked");
    }
    else if(button.state() == HIGH)
    {
      Serial.println("Released");
    }
    buttonPrevState = button.state();
  }


  delay(10); 
}
