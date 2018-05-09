#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
int data[4];
int ly,lx,ry,rx;
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Serial.println("Listening...");
}

void loop() {
  if (radio.available()) {
    radio.read(data, sizeof(data));
  
    ly = data[0];
    lx = data[1];
    ry = data[2];
    rx = data[3];

    Serial.print("LX: ");
    Serial.print(lx);
    Serial.print("\t LY: ");
    Serial.print(ly);
    Serial.print("\t RX: ");
    Serial.print(rx);
    Serial.print("\t RY: ");
    Serial.println(ry);
  }
    
    
}
