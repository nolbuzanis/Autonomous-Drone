#include "Wire.h"

// sensor/data vars

int16_t rawGyroX, rawGyroY, rawGyroZ, rawAccelX, rawAccelY, rawAccelZ, rawTemp;
int16_t GyroX, GyroY, GyroZ, AccelX, AccelY, AccelZ, sensorTemp;
byte errorTest;
byte throttle, pitch, roll, yaw;

// transciever vars

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

  Serial.begin(38400);
  Serial.println("Initializing I2C devices...");
  Wire.setClock(400000); // Set I2C clock speed to 400kHz
  Wire.begin(); // Start IC2 as master
  Wire.beginTransmission(0x68); // Start communicating with the MPU6050 at address 105
  delay(250);
  errorTest = Wire.endTransmission();
  
  if (!errorTest) Serial.println("MPU Connection sucessful.");
  else Serial.println("Error connecting to MPU!");

  Serial.println("Powering on MPU...");
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B); // access PWN_MGMT_1 register
  Wire.write(1); // Wake up MPU and configure to use X axis Gyro as clock reference for increased stability
  Wire.endTransmission();
}

void loop() {

  throttle = transInput[1];
  roll = transInput[2];
  pitch = transInput[3];
  yaw = transInput[4];

readMPU();  
printData();

delayMicroseconds(4000); // wait 4000us or 4 milliseconds (250 Hz)
}

// Read sensor values from the MPU6050

void readMPU() {
  
  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // indicate start register is ACCEL_XOUT_H
  Wire.endTransmission();
  Wire.requestFrom(0x68,14); // request 14 bytes from MPU

  rawAccelX = Wire.read() << 8 | Wire.read(); // shifts high bytes by 8, and ORs with low bytes
  rawAccelY = Wire.read() << 8 | Wire.read(); 
  rawAccelZ = Wire.read() << 8 | Wire.read(); 
  rawTemp = Wire.read() << 8 | Wire.read(); 
  rawGyroX = Wire.read() << 8 | Wire.read(); 
  rawGyroY = Wire.read() << 8 | Wire.read(); 
  rawGyroZ = Wire.read() << 8 | Wire.read(); 

  sensorTemp = convertTo2s(rawTemp)/340 + 36.58; // Converting raw temp value to degree celsius as stated in the datasheet
  AccelX = rawAccelX / 16384; // the sensitivity scale factor is 16384 LSB/g
  AccelY = rawAccelY / 16384; // Puts the raw values into g's
  AccelZ = rawAccelZ / 16384;  
  GyroX = rawGyroX / 131; // converts raw values to degrees/sec
  GyroY = rawGyroY / 131;
  GyroZ = rawGyroZ / 131;
  }

// Print sensor values on serial monitor

void printData(){
  
  Serial.println("Accelerometer: ");
  Serial.print("X: ");
  Serial.print(AccelX);
  Serial.print("\t Y: ");
  Serial.print(AccelY);
  Serial.print("\t Z: ");
  Serial.print(AccelZ);
  Serial.println("Temperature: ");
  Serial.print("Temp (C): ");
  Serial.print(sensorTemp);
  Serial.println("Gyroscope: ");
  Serial.print("X: ");
  Serial.print(GyroX);
  Serial.print("\t Y: ");
  Serial.print(GyroY);
  Serial.print("\t Z: ");
  Serial.print(GyroZ);
  }


// Converts signed binary values to 2's complement

int convertTo2s(int x){
  
  if (x >> 15 & 1) {
  byte lowX, highX;
  highX = (x >> 8) & B01111111; // Change the sign bit to a 0 and take upper 8 bits of x
  lowX = x; // take lower 8 bits of x
  highX = B01111111 - highX; // invert upper binary digits of x
  lowX = B11111111 - lowX + 1; // invert lower binary digits of x and add 1
  
  x = highX << 8 | lowX;
  }
  return x;
  }

// Interupt Service Routine (ISR) for transciever

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

