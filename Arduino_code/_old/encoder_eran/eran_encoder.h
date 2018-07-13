#ifndef __ERAN_ENCODER__
#define __ERAN_ENCODER__
#include "Arduino.h"

#define NO_TRANSITION_EVENT (1)

class EranEncoder 
{
private:
  // channel shift register
  uint8_t statusChA = 0x00;
  uint8_t statusChB = 0x00;
  
  uint8_t searchPatternA = 0x00;
  uint8_t searchPatternB = 0x00;
  
  typedef enum {
    STATE_IDLE,
    STATE_DETECTING_CW,
    STATE_DETECTING_CCW,
  } STATE;
  STATE state = STATE_IDLE;
  
  
  uint8_t transitionEvent = NO_TRANSITION_EVENT;
  uint8_t transitionEventIndex = 0;
    
  uint8_t cwTransitions[4]  = {8, 0, 15, 7};
  uint8_t ccwTransitions[4] = {0, 8, 7, 15}; 

  uint8_t CHA, CHB;
  
public:

  //
  int changeCounts = 0;

  // constructor
  EranEncoder() {};
  
  void init(int _cha, int _chb);
  void update();
  int getCounter();
  void resetCounter();
};

#endif // __ERAN_ENCODER__
