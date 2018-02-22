#include "Wire.h"

int16_t rawGyroX, rawGyroY, rawGyroZ, rawAccelX, rawAccelY, rawAccelZ, rawTemp;
int16_t GyroX, GyroY, GyroZ, AccelX, AccelY, AccelZ, sensorTemp;
//byte interruptFlag = 0;
byte errorTest;

/*
void mpuInterrupt() {
  interruptFlag = 1;
  }*/

void setup() {

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

  /*
  Wire.beginTransmission(0x68); 
  Wire.write(0x23); // write to register 35, FIFO enable
  Wire.write(0xF8); // set enable bits to 11111000 to enable data from accelerometer, gyro, and temp sensor
  Wire.endTransmission();

  Wire.beginTransmission(0x68); 
  Wire.write(0x6A); // User Control register
  Wire.write(0x40); // set FIFO EN bit to 1
  Wire.endTransmission();

  // Enable interrupt detection from Arduino
  attachInterrupt(0, mpuInterrupt, RISING);
  */
}

void loop() {

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

delay(100); // wait 100ms
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

