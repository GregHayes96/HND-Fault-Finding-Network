#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

RF24 radio (9, 10);


#define PLOAD_WIDTH  32  // 32 unsigned chars TX payload
byte pip;
byte pload_width_now; // siza of data sent
byte newdata; //1 if there is new data, 0 for no data
unsigned char rx_buf[PLOAD_WIDTH]= {0};


char *systems[] = {"Radar ","Sonar","NavAids","Comms"}; //the systems that are being monitored by the pager

struct dataStruct1{
int group;
int nodeNum;
int r1; // reset value
int f1; // fault value
}transmitter1_data;

LiquidCrystal_I2C lcd(0x27,16,2);

unsigned char ADDRESS0[5]  = 
{
  0xb0,0x43,0x88,0x99,0x45
}; // Define a static TX address

void setup()
{
  Wire.begin();
  lcd.init();
  lcd.backlight();

  radio.begin();
  printf_begin();
  Serial.begin(9600);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
  radio.openReadingPipe(0,ADDRESS0);

  radio.startListening();
  radio.printDetails();
  delay(1000);
}

void loop()
{
  checkPayload();
  lcd.clear();
  if(newdata==1)
  {
  newdata=0; //reset entry conditions
  Serial.println("Data recieved ");

    if(pip==0&&pload_width_now==sizeof(transmitter1_data))
    {

    memcpy(&transmitter1_data, rx_buf, sizeof(transmitter1_data)); 
          
          if(transmitter1_data.r1 == 1){
            //do nothing
          }
          else if(transmitter1_data.f1 == 1){
          lcd.print("Alert: ");
          lcd.print(systems[transmitter1_data.nodeNum]); //this will print the appropriate system name taken from the data struct
          lcd.print(" ");
          }
        
    }
   
    Serial.println("");
  }

    delay(1000);

}

void checkPayload(){

if ( radio.available(&pip) )
    {
      Serial.println("Pip open");
      // Fetch the payload, and see if this was the last one.
      pload_width_now = radio.getDynamicPayloadSize();

      // If a corrupt dynamic payload is received, it will be flushed
      if(!pload_width_now){
        
      }
      else
      {
       
       radio.read( rx_buf, pload_width_now );

       newdata=1;

      }
    }
  Serial.println("No pipe open");
}

