// 2018-07-06
// La TV

// encoder stuff
#define CHA 2
#define CHB 3
#define PRESS 7

volatile byte chA, chB = 0;
volatile byte bCurrent = 0;
volatile byte bPrevious = 0;
volatile long integrator = 0;
volatile long prevIntegrator = 0;

bool status_PRESS, prevStatus_PRESS = false;


volatile enum STATE 
{
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} state;

// buttons stuff
#define BUTTON_A 8
#define BUTTON_B 9

bool status_A, prevStatus_A = false;
bool status_B, prevStatus_B = false;

// serial stuff
bool sendSerial = false;


// SETUP /////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  pinMode(PRESS, INPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), chA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB), chB_ISR, CHANGE);
  // when a black dot is in sight, the signal from the GROVE sensor
  // will fall down. So we are using the FALLING edge.
  //attachInterrupt(digitalPinToInterrupt(PRESS), press_ISR, FALLING);

  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
}


// DRAW //////////////////////////////////////////
void loop() 
{
  getSerialData();

  if(integrator != prevIntegrator && integrator!=0)
  {
    if(sendSerial) {
      if(state == ENC_CW) {
        Serial.write('1');
      } else if(state == ENC_CCW ){
        Serial.write('0');
      }
      //Serial.print( "E-" );
      //Serial.print(integrator);
      //Serial.println(";");
    }
    prevIntegrator = integrator;
  }


  status_PRESS = digitalRead( PRESS );

  if(status_PRESS != prevStatus_PRESS){
    if( sendSerial) { 
      if( !status_PRESS )
        Serial.write('c'); 
      }
    prevStatus_PRESS = status_PRESS;
  }
  
  

  status_A = digitalRead(BUTTON_A);
  status_B = digitalRead(BUTTON_B);

  if(status_A != prevStatus_A){
    if( sendSerial) {
      if( !status_A )
        Serial.write('r');   
      /*print( "A-" ); Serial.print( !status_A ); Serial.println(";");*/ 
      }
    prevStatus_A = status_A;
    
  }

  if(status_B != prevStatus_B){
    if( sendSerial) { 
      if( !status_B )
        Serial.write('l'); 
      /*Serial.print( "B-" );Serial.print( !status_B ); Serial.println(";");*/ 
      }
    prevStatus_B = status_B;
  }
  
  delay(10);
}


/************************************************************************************
 * SERIAL UTILITY FUNCTIONS
 ***********************************************************************************/
void getSerialData()
{
  if(Serial.available()) {
    char user_input = Serial.read(); // Read user input and trigger appropriate function
      
    if (user_input =='1')  {
       sendSerial = true;
    }
    else if(user_input =='0')  {
      sendSerial = false;
    }
  }
}



// OTHER STUFF ///////////////////////////////////
void chA_ISR()
{
  bCurrent = (PIND & B00000011)>>0;
  decode();
}

void chB_ISR()
{
  bCurrent = (PIND & B00000011)>>0;
  decode();
}


void press_ISR()
{
  //Serial.write('c');  
  integrator = 0;
}



void decode() 
{
  //Serial.println(bCurrent);
  
  // encoder stuff
  if( bCurrent != bPrevious )
  {
    if( ( bCurrent == 2 && bPrevious == 0 ) ||
        ( bCurrent == 3 && bPrevious == 2 ) ||
        ( bCurrent == 1 && bPrevious == 3 ) ||
        ( bCurrent == 0 && bPrevious == 1 )  
      )
    {
      state = ENC_CW;
      integrator ++;
      //Serial.println(integrator);
      //Serial.write('1');
    }
    else if( ( bCurrent == 1 && bPrevious == 0 ) ||
             ( bCurrent == 3 && bPrevious == 1 ) ||
             ( bCurrent == 2 && bPrevious == 3 ) ||
             ( bCurrent == 0 && bPrevious == 2 ) 
           )
    {
      state = ENC_CCW;
      integrator --;
      //Serial.println(integrator);
      //Serial.write('0');
    }
    bPrevious = bCurrent;
  }  
}

