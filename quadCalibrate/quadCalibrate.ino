#include <EEPROM.h>
#include <Wire.h>

int addr = 0; // start address to write too on EEPROM
int serialInput = 0; // to accept user input
long rawDataX, rawDataY, rawDataZ;

void setup() {
 Serial.begin(9600);
  Wire.setClock(400000); // Set I2C clock speed to 400kHz
  Wire.begin(); // Start IC2 as master
  delay(250);

  Wire.beginTransmission(0x68); // Start communicating with the MPU6050
  Wire.write(0x6B); // access PWN_MGMT_1 register
  Wire.write(0); // Set all bits to 0 to wake up MPU
  Wire.endTransmission();

  Serial.println("Welcome to quadcopter calibration. Place the quadcopter on a flat surface and wait 5 seconds for sensor values to stabilize.");  
  delay(5000); // wait 5 seconds for sensor values to stabilize
  while(serialInput == 0) {
  Serial.println("Press any key to start calibration:");
  serialInput = Serial.read();
  }
  Serial.println("Beginning calibraton.Do not move quadcopter until finished...."); 
}

void loop() {

int val;
Serial.println("Calculating accelerometer value at rest....");

  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // indicate start register is ACCEL_XOUT_H
  Wire.endTransmission();

  for(int i=0; i<100; i++) {
  Wire.requestFrom(0x68,6); // request 14 bytes from MPU
  
  rawDataX = Wire.read() << 8 | Wire.read(); // shifts high bytes by 8, and ORs with low bytes
  rawDataY = Wire.read() << 8 | Wire.read();
  rawDataZ = Wire.read() << 8 | Wire.read();
  
  delayMicroseconds(4000); // Looptime is  4 milliseconds, or 4000 microseconds (1kHz)
  }

  rawDataX /=  100;
  rawDataY /=  100;
  rawDataZ /=  100;

  Serial.println("Base Acceleromter Values:");
  Serial.print("X: ");
  Serial.print(rawDataX/16384);
  Serial.print("\t Y: ");
  Serial.print(rawDataY/16384);
  Serial.print("\t Z: ");
  Serial.print(rawDataZ/16384);

  /*
  for(int j = 0; j<14; j++) {
  val = Wire.read(); // store value from IC2 into EEPROM at addr
  addr = addr + 1; // increment address by 1
  }

  if (addr == EEPROM.length()) { // prevent overflow of EEPROM
    addr = 0;
  }*/
  
  while(true);

}
