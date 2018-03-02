#include "Wire.h"

int16_t rawGyroX, rawGyroY, rawGyroZ, rawAccelX, rawAccelY, rawAccelZ, rawTemp;
float GyroX, GyroY, GyroZ, AccelX, AccelY, AccelZ, sensorTemp;
//byte interruptFlag = 0;
byte errorTest;

/*
void mpuInterrupt() {
  interruptFlag = 1;
  }*/

void setup() {

  Serial.begin(9600);
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

sensorTemp = rawTemp/340.0 + 36.58; // Converting raw temp value to degree celsius as stated in the datasheet
AccelX = rawAccelX / 16384.0; // the sensitivity scale factor is 16384 LSB/g
AccelY = rawAccelY / 16384.0; // Puts the raw values into g's
AccelZ = rawAccelZ / 16384.0;  
GyroX = rawGyroX / 131.0; // converts raw values to degrees/sec
GyroY = rawGyroY / 131.0;
GyroZ = rawGyroZ / 131.0;

Serial.print("Accelerometer: ");
Serial.print("X: ");
Serial.print(AccelX);
Serial.print("\t Y: ");
Serial.print(AccelY);
Serial.print("\t Z: ");
Serial.println(AccelZ);
Serial.print("Temperature: ");
Serial.println(sensorTemp);
Serial.print("Gyroscope: ");
Serial.print("X: ");
Serial.print(GyroX);
Serial.print("\t Y: ");
Serial.print(GyroY);
Serial.print("\t Z: ");
Serial.println(GyroZ);

delay(500); // wait 100ms
}

