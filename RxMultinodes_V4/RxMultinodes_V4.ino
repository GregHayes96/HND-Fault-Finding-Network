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

      Serial.print(" Node 1 - ");
    //alertMaintainer1();

    memcpy(&transmitter1_data, rx_buf, sizeof(transmitter1_data)); 
          
          Serial.print("Fault : ");
          Serial.print(transmitter1_data.f1);
      
      
   
    }
   
    if(pip==2&&pload_width_now==sizeof(transmitter2_data))
    {
     
      Serial.print("Node 2 ");
      alertMaintainer2();

    memcpy(&transmitter2_data, rx_buf, sizeof(transmitter2_data)); 

     radio.write(&transmitter2_data, sizeof(transmitter2_data));

          Serial.print("Fault : ");
          Serial.print(transmitter1_data.f1);
    }

     if(pip==3&&pload_width_now==sizeof(transmitter3_data))
    {

      Serial.print("Node 3 ");
      //alertMaintainer3();
    memcpy(&transmitter3_data, rx_buf, sizeof(transmitter3_data)); 
     
          Serial.print("Fault : ");
          Serial.print(transmitter1_data.f1);
    }
    Serial.println("");
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







/*
void alertMaintainer1(){

  bool ok=radio.write(&transmitter1_data, sizeof(transmitter1_data));

  if(ok == 1)
       {
         if(transmitter1_data.f1 == 1 && transmitter1_data.r1 == 0){
          Serial.println("Send maintainer alert for node ");
          Serial.println(transmitter1_data.group);
         }
         else{
          Serial.println("No alert required ");
         }
       }
      else
       {
          Serial.println("failed to send maintainer alert");
       }
  delay(1000);
}
*/
void alertMaintainer2(){
/*
  bool ok=radio.write(&transmitter2_data, sizeof(transmitter2_data));

   if(ok == 1)
       {
         if(transmitter2_data.f1 == 1 && transmitter2_data.r1 == 0){
          Serial.println("Send maintainer alert for node ");
          Serial.println(transmitter2_data.group);
         }
         else{
          Serial.println("No alert required ");
         }
       }
      else
       {
          Serial.println("failed to send maintainer alert");
       }
 
  */
}
/*
void alertMaintainer3(){

  bool ok=radio.write(&transmitter3_data, sizeof(transmitter3_data));

  if(ok == 1)
       {
         if(transmitter3_data.f1 == 1 && transmitter3_data.r1 == 0){
          Serial.println("Send maintainer alert for node ");
          Serial.println(transmitter3_data.group);
         }
         else{
          Serial.println("No alert required ");
         }
       }
      else
       {
          Serial.println("failed to send maintainer alert");
       }

  delay(1000);
}
*/