#include "eran_encoder.h"

void EranEncoder::init(int _cha, int _chb)
{
  CHA = _cha;
  CHB = _chb;
  pinMode(CHA, INPUT_PULLUP);
  pinMode(CHB, INPUT_PULLUP);
}


void EranEncoder::update()
{
  // buffer CH A and B
  statusChA = ((statusChA<<1) | digitalRead(CHA)) & 0x07;
  statusChB = ((statusChB<<1) | digitalRead(CHB)) & 0x07;

  /*
  if( statusChA != prevStatusChA || statusChB !=prevStatusChB ) {
    Serial.print("A:"); Serial.print(statusChA); Serial.print("-B:"); Serial.println(statusChB); 
    if(statusChA != prevStatusChA) prevStatusChA = statusChA;
    if(statusChB != prevStatusChB) prevStatusChB = statusChB;
  }
  */

  // debounce CH A and B update search pattern for the channel
  if( (statusChA == 0x00 || statusChA == 0x07) &&
      (statusChB == 0x00 || statusChB == 0x07)
  ) {
    // this is an imaginative way to calculate the transitionEvent
    transitionEvent = (statusChB | (1<<3))*3 + statusChA;

  }

  //if(statusChA == 0x07 && statusChB == 0x07) { state = STATE_IDLE_X; transitionEventIndex = 0;}
  //if(statusChA == 0x00 && statusChB == 0x00) { state = STATE_IDLE_Y; transitionEventIndex = 2;}
  

  if( transitionEvent != NO_TRANSITION_EVENT )
  {
    
    if( state == STATE_IDLE_X ) {
      if(transitionEvent == cwTransitions[0]) {
        state = STATE_DETECTING_CW;
        transitionEventIndex++;
      }
      else if(transitionEvent == ccwTransitions[0] ) {
        state = STATE_DETECTING_CCW;
        transitionEventIndex++;
      }
    }
    if( state == STATE_IDLE_Y ) {
      if(transitionEvent == cwTransitions[2]) {
        state = STATE_DETECTING_CW;
        transitionEventIndex++;
      }
      else if(transitionEvent == ccwTransitions[2]) {
        state = STATE_DETECTING_CCW;
        transitionEventIndex++;
      }
    }
    else if( state == STATE_DETECTING_CW )
    {
      if(transitionEvent == cwTransitions[transitionEventIndex])
      {
        
        if( transitionEventIndex == 1 ) {
          changeCounts++;
          if(this->callback_cw) this->callback_cw();
          state = STATE_IDLE_Y;
          transitionEventIndex = 2;
        }
        else if( transitionEventIndex == 3 ) {
          changeCounts++;
          if(this->callback_cw) this->callback_cw();
          state = STATE_IDLE_X;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE_X;
        transitionEventIndex = 0; // or 2?
      }
    }
    else if( state == STATE_DETECTING_CCW )
    {
      if(transitionEvent == ccwTransitions[transitionEventIndex] )
      {
        if( transitionEventIndex == 1 ) {
          changeCounts--;
          if(this->callback_ccw) this->callback_ccw();
          state = STATE_IDLE_Y;
          transitionEventIndex = 2;
        }
        else if( transitionEventIndex == 3) {
          changeCounts--;
          if(this->callback_ccw) this->callback_ccw();
          state = STATE_IDLE_X;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE_X;
        transitionEventIndex = 0;
      } 
    }
    // we have already processed the transition occured so
    // we must zero out the transition.
    transitionEvent = NO_TRANSITION_EVENT;
  }
}





void EranEncoder::resetCounter() {
  changeCounts = 0;
}


int EranEncoder::getCounter() {
  return changeCounts;
}


void EranEncoder::setCallbackCW(ENCODER_CALLBACK_CW) {
  this->callback_cw = callback_cw;
}


void EranEncoder::setCallbackCCW(ENCODER_CALLBACK_CCW) {
  this->callback_ccw = callback_ccw;
}
