#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
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
// MPU6050 Object
Adafruit_MPU6050 mpu;

int servoPinBar = 29; 
int servoPinLock = 28;

int redLED = 31;
int greenLED = 30;
int siren = 27;
#define TRIG_PIN 32  // Define the pin for Trig
#define ECHO_PIN 33 // Define the pin for Echo

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 53
#define RESET_DIO 9
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 
Servo ServoBar; 
Servo ServoLock; 
float lastAcc;
int openBar = 180;
int closeBar = 80;
int openTeeth = 90;
int closeTeeth = 130;
unsigned char keyCard[5];
unsigned char currentCard[5];
bool isLocked = false;
bool isKey = true;


sensors_event_t a, g, temp;

void setup()
{ 
  Serial.begin(9600);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  pinMode(TRIG_PIN, OUTPUT); // Set Trig as an OUTPUT
  pinMode(ECHO_PIN, INPUT);  // Set Echo as an INPUT

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }else{
    Serial.println("MPU6050 initialized.");
  }

  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();
  ServoLock.attach(servoPinLock); 
  ServoBar.attach(servoPinBar);  

  if(ServoLock.read() != openTeeth){
    ServoLock.write(openTeeth);
  }
  delay(1000);
  if(ServoBar.read() != openBar){
    ServoBar.write(openBar);
  }
  
  lastAcc = a.acceleration.x;
  Serial.println("Done");
}

void loop()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo pin and measure the duration of the pulse
  long distance = pulseIn(ECHO_PIN, HIGH);

    mpu.getEvent(&a, &g, &temp);

    Serial.println(distance);
    if(isLocked){
      if( distance > 2000){
        digitalWrite(redLED, HIGH);
        delay(1000);
        digitalWrite(redLED, LOW);
        delay(1000);
        digitalWrite(redLED, HIGH);
        delay(1000);
        digitalWrite(redLED, LOW);
        delay(1000);
        digitalWrite(redLED, HIGH);
      }
      if( abs(a.acceleration.x - lastAcc) >= .2) {
        digitalWrite(redLED, HIGH);
        delay(1000);
        digitalWrite(redLED, LOW);
        delay(1000);
        digitalWrite(redLED, HIGH);
        delay(1000);
        digitalWrite(redLED, LOW);
        delay(1000);
        digitalWrite(redLED, HIGH);
      }
      lastAcc = a.acceleration.x;
    }


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
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
        Serial.println(("UNLOCKED"));
      }
      isKey = true;
    }else if (distance < 2000){
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
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      Serial.println(("LOCKED"));
    }
  }
  
}
