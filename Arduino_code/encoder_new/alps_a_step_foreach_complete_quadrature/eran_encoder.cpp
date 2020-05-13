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
  statusChA = ((statusChA<<1) | digitalRead(CHA)) & 0x07;
  statusChB = ((statusChB<<1) | digitalRead(CHB)) & 0x07;
  //Serial.print("A: "); Serial.print(statusChA); Serial.print(" - B: "); Serial.println(statusChB); 


  // debounce CH A and update search pattern for the channel
  if( statusChA == searchPatternA ) {
    // change search pattern
    searchPatternA = (~searchPatternA) & 0x07;
    // append 0 to resutl
    transitionEvent = statusChA; // 0 or 7
  }
  // debounce CH B and update search pattern for the channel
  if( statusChB == searchPatternB ) {
    // change search pattern
    searchPatternB = (~searchPatternB) & 0x07;
    // append 1 to result
    transitionEvent = (statusChB | (1<<3)); // 8 or 15
  }

 
  // Check pattern (if something has happend)
  // the transition event is now 7, 15, 0 or 8.
  if( transitionEvent != NO_TRANSITION_EVENT )
  {
    if( state == STATE_IDLE ) {
      if(transitionEvent == cwTransitions[0] ) {
        state = STATE_DETECTING_CW;
        transitionEventIndex++;
      }
      else if(transitionEvent == ccwTransitions[0] ) {
        state = STATE_DETECTING_CCW;
        transitionEventIndex++;
      }
    }
    else if( state == STATE_DETECTING_CW )
    {
      if(transitionEvent == cwTransitions[transitionEventIndex])
      {
        transitionEventIndex++;
        if( transitionEventIndex == 4 ) {
          changeCounts++;
          if(this->callback_cw) this->callback_cw();
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
        transitionEventIndex = 0;
      }
    }
    else if( state == STATE_DETECTING_CCW )
    {
      if(transitionEvent == ccwTransitions[transitionEventIndex] )
      {
        transitionEventIndex++;
        if( transitionEventIndex == 4 ) {
          changeCounts--;
          if(this->callback_ccw) this->callback_ccw();
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
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
