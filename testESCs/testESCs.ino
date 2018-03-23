#include <Servo.h>

Servo myServo;
int pot, throttle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myServo.attach(5);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
 
  pot = analogRead(1);
  Serial.println(pot);
  delay(200);
  throttle = map(pot, 0, 1023, 1100, 1900);
  myServo.writeMicroseconds(throttle);
  Serial.println(throttle);
}
