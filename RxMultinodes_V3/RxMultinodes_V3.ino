#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

RF24 radio (9, 10);


#define PLOAD_WIDTH  32  // 32 unsigned chars TX payload
byte pip;
byte pload_width_now; // siza of data sent
byte newdata; //1 if there is new data, 0 for no data
unsigned char rx_buf[PLOAD_WIDTH]= {0};

struct dataStruct1{
int group;
int nodeNum;
int r1; // reset value
int f1; // fault value
}transmitter1_data;

struct dataStruct2{
int group;
int nodeNum;
int r1;
int f1; 
}transmitter2_data;

struct dataStruct3{
int group;
int nodeNum;
int r1;
int f1; 
}transmitter3_data;

unsigned char ADDRESS2[1]= {0xb2};	
unsigned char ADDRESS3[1]= {0xb3};	
unsigned char ADDRESS4[1]= {0xb4};	
unsigned char ADDRESS5[1]= {0xb5};	


unsigned char ADDRESS1[5]  = 
{
   0xb1,0x43,0x88,0x99,0x45
}; // Define a static TX address

unsigned char ADDRESS0[5]  = 
{
  0xb0,0x43,0x88,0x99,0x45
}; // Define a static TX address

void setup()
{
 
  radio.begin();
  printf_begin();
  Serial.begin(9600);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
    radio.openWritingPipe(ADDRESS0);
    radio.openReadingPipe(0,ADDRESS0);
    radio.openReadingPipe(1,ADDRESS1);
    radio.openReadingPipe(2,ADDRESS2);
    radio.openReadingPipe(3,ADDRESS3);
    radio.openReadingPipe(4,ADDRESS4);
    radio.openReadingPipe(5,ADDRESS5);

  radio.startListening();
  radio.printDetails();
  delay(1000);
}

void loop()
{
  checkPayload();

  if(newdata==1)
  {
  newdata=0; //reset entry conditions

    if(pip==1&&pload_width_now==sizeof(transmitter1_data))
    {


    memcpy(&transmitter1_data, rx_buf, sizeof(transmitter1_data)); 
          
          Serial.print(" Fault//Bypass ");
          Serial.print(transmitter1_data.f1);
          Serial.print(" // ");
          Serial.print(transmitter1_data.r1);
          radio.write(&transmitter1_data, sizeof(transmitter1_data));
   
    }
   
    if(pip==2&&pload_width_now==sizeof(transmitter2_data)) //pip being 2 means the data was came from reading pipe two.
    {
    memcpy(&transmitter2_data, rx_buf, sizeof(transmitter2_data)); 
     
          Serial.print("Fault//Bypass ");
          Serial.print(transmitter2_data.f1);
          Serial.print(" // ");
          Serial.print(transmitter2_data.r1);
          Serial.println("");
          radio.stopListening();
          radio.write(&transmitter2_data, sizeof(transmitter2_data));
          radio.startListening();
    }

    
  
  if(pip==3&&pload_width_now==sizeof(transmitter3_data))
    {
    memcpy(&transmitter3_data, rx_buf, sizeof(transmitter3_data)); 
     
          Serial.print("Fault//Bypass ");
          Serial.print(transmitter3_data.f1);
          Serial.print(" // ");
          Serial.print(transmitter3_data.r1);
          Serial.println("");
          radio.stopListening();
          radio.write(&transmitter3_data, sizeof(transmitter3_data));
          radio.startListening();
    }
  }
     delay(100);
}
   



void checkPayload(){

if ( radio.available(&pip) )
    {

      // Fetch the payload, and see if this was the last one.
      pload_width_now = radio.getDynamicPayloadSize();

      // If a corrupt dynamic payload is received, it will be flushed
      if(!pload_width_now){
        
      }
      else
       {
       
      radio.read( rx_buf, pload_width_now );

      newdata=1;

      Serial.print(F("Node data = ")); // This could be altered to change to system name potentially
      Serial.print(pip);
      Serial.print(" ");
      }
    }

}


