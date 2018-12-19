#include <Servo.h>

Servo esc1, esc2, esc3, esc4;
int pot, throttle, value = 1800;

void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  esc1.attach(3);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  //pot = analogRead(1);
  //throttle = map(pot, 0, 1023, 1100, 1900);
  
  esc1.writeMicroseconds(value);

  if(Serial.available()) {
    value = Serial.parseInt();
  }

  Serial.println(value);
 
  delay(200);
}
