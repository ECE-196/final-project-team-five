#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>
// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPinBar = 29; 
int servoPinLock = 28;
/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 53
#define RESET_DIO 9
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 
Servo ServoBar; 
Servo ServoLock; 
int openBar = 180;
int closeBar = 0;
int openTeeth = 95;
int closeTeeth = 163;
unsigned char keyCard[5];
unsigned char currentCard[5];
bool isLocked = false;
bool isKey = true;
void setup()
{ 
  Serial.begin(9600);
  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();
  ServoLock.attach(servoPinLock); 
  ServoBar.attach(servoPinBar);  

  ServoLock.write(openTeeth);
  delay(1000);
  ServoBar.write(openBar); 
  Serial.println("Done");
}

void loop()
{
  Serial.println(ServoBar.read()); 
  Serial.println(ServoLock.read()); 
  /* Has a card been detected? */
  if (RC522.isCard()){
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");

    for (int i=0;i<5;i++) {
      Serial.print(RC522.serNum[i]);
      currentCard[i] = RC522.serNum[i];
    //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }

    if(isLocked){
      for(int i=0; i<5; i++){
        if(keyCard[i] != currentCard[i]){
          isKey = false;
          Serial.println(("WRONG CARD"));
          break;
        }
      }
      if(isKey){
        ServoLock.write(openTeeth);
        delay(1000);
        ServoBar.write(openBar);
        isLocked = false;
        Serial.println(("UNLOCKED"));
      }
      isKey = true;
    }else{
      for(int i=0; i<5; i++){
        keyCard[i] = currentCard[i];
      }
      ServoLock.write(openTeeth);
      delay(1000);
      ServoBar.write(openBar);
      delay(1000);
      ServoBar.write(closeBar); 
      delay(1000);
      ServoLock.write(closeTeeth);
      isLocked = true;
      Serial.println(("LOCKED"));
    }
  }
  delay(1000);
}

