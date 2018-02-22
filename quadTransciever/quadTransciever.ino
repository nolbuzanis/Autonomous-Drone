
long currentTime;
byte channel_1_signal = 0;
byte channel_2_signal = 0;
byte channel_3_signal = 0;
byte channel_4_signal = 0;

byte channel_1_currentTimer;
byte channel_2_currentTimer;
byte channel_3_currentTimer;
byte channel_4_currentTimer;

int transInput[4];

void setup() {

  // Pin change interrupts for transiever module
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1<<PCINT3 | 1<<PCINT2 | 1<<PCINT1 | 1<<PCINT0);
  
  // Setting tranciever pins as inputs
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// Main Code /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  byte throttle = transInput[1];
  byte roll = transInput[2];
  byte pitch = transInput[3];
  byte yaw = transInput[4];
  
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// ISP for transciever ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ISR (PCINT0_vect) {
  currentTime = millis();
  if (PINB & B00000001) { // digital pin 8 high
    if (channel_1_signal == 0) {
      channel_1_currentTimer = currentTime; // get current currentTime
      channel_1_signal = 1; // set channel_1 flag = 1
      } 
  } else if (channel_1_signal) { // if digital pin 8 goes low, and channel_1 flag = 1, it is the end of the pulse
    transInput[1] = currentTime - channel_1_currentTimer; // store pulse width in transciever array
    channel_1_signal = 0; // clear flag
    }

  if (PINB & B00000010) { // digital pin 9 high
    if (channel_2_signal == 0) {
      channel_2_currentTimer = currentTime; // get current currentTime
      channel_2_signal = 1; // set channel_2 flag = 1
      } 
  } else if (channel_2_signal) { // if digital pin 9 goes low, and channel_2 flag = 1, it is the end of the pulse
    transInput[2] = currentTime - channel_2_currentTimer; // store pulse width in transciever array
    channel_2_signal = 0; // clear flag
    }

    if (PINB & B00000100) { // digital pin 10 high
    if (channel_3_signal == 0) {
      channel_3_currentTimer = currentTime; // get current currentTime
      channel_3_signal = 1; // set channel_1 flag = 1
      } 
  } else if (channel_3_signal) { // if digital pin 10 goes low, and channel_3 flag = 1, it is the end of the pulse
    transInput[3] = currentTime - channel_3_currentTimer; // store pulse width in transciever array
    channel_3_signal = 0; // clear flag
    }

    if (PINB & B00001000) { // digital pin 11 high
    if (channel_4_signal == 0) {
      channel_4_currentTimer = currentTime; // get current currentTime
      channel_4_signal = 1; // set channel_4 flag = 1
      } 
  } else if (channel_4_signal) { // if digital pin 11 goes low, and channel_4 flag = 1, it is the end of the pulse
    transInput[4] = currentTime - channel_4_currentTimer; // store pulse width in transciever array
    channel_4_signal = 0; // clear flag
    }
  }
