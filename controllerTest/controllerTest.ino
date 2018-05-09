#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
int data[4];
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {

  data[0] = analogRead(3); //ly
  data[1] = analogRead(4); //lx
  data[2] = analogRead(7); //ry
  data[3] = analogRead(6); //rx
  radio.write(data, sizeof(data));

  delay(250);
}
