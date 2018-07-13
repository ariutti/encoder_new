/*
 * Using an external library to debounce 
 * the encoder push button
 */

#include <ButtonDebounce.h>

ButtonDebounce button(7, 50);
bool buttonState, buttonPrevState = true;


void setup() 
{
  Serial.begin(9600);
}


void loop() 
{
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
}
