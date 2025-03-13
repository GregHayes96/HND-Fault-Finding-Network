#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

#include <Wire.h>

int address = 0x28; // 28 is the address
byte byte1, byte2, byte3, byte4;

RF24 radio (9, 10); // CE & CSE pin


struct dataStruct{
const int group = 1;
const int nodeNum = 2;
int r1; // reset value
int f1; // fault value
}transmitter1_data;

unsigned char ADDRESS0[5]  = 
{
  0xb3,0x43,0x88,0x99,0x45
}; // Define a static TX address
//just change b1 to b2 or b3 to send to other pip on reciever

void setup()
{
 // Wire.begin();
  radio.begin();
  Serial.begin(9600);
  pinMode(5,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  printf_begin();
  radio.setRetries(15,15);
  radio.enableDynamicPayloads();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS0);
  radio.openReadingPipe(0,ADDRESS0);
  radio.stopListening();
  radio.printDetails();

}

void loop()
{

 
   transmitter1_data.r1=digitalRead(5);
   transmitter1_data.f1=digitalRead(3);
   

   bool ok=radio.write(&transmitter1_data, sizeof(transmitter1_data));

   
     if(ok)
       {
 
          Serial.println("Pipe 2");
          Serial.println(transmitter1_data.r1);
          Serial.println(transmitter1_data.f1);
         
       }
      else
       {
          Serial.println("it failed to send");
       }

      delay(1000);
   
}


void updateData(){

  

}
