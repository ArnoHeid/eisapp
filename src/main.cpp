#include <EEPROM.h>
#include <SPI.h>
#include <stdlib.h>
#include <Arduino.h>
#include <MFRC522.h>  

class Data
{
public:
  String user;
  String userRfid;
  float amount;
  void buyIce(){amount -=1;}
};

double icePrice = 1;
size_t count = 2;
Data *users = new Data[count];

byte readCard[4]; // Stores scanned ID read from RFID Module
#define SS_PIN 15
#define RST_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
void blinkLed(int goip){
      digitalWrite(goip, HIGH); 
      delay(500);            
      digitalWrite(goip, LOW);
}

void getID()
{

  mfrc522.PICC_ReadCardSerial();

  Serial.println(F("Scanned PICC's UID:"));
  for (uint8_t i = 0; i < 4; i++)
  { //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  Serial.println("Done Reading");
  mfrc522.PICC_HaltA(); // Stop reading
}

Data& getUser(String RfidCode)
{
  for (size_t i = 0; i < count; i++)
  {
    if (RfidCode == users[i].userRfid)
    {
      return users[i];
    }
  }
}

///////////////////////////////////////// Setup ///////////////////////////////////
void setup()
{
  Serial.println("Start up");
  Serial.println(F("-------------------"));
  //Protocol Configuration
  Serial.begin(115200); // Initialize serial communications with PC
  SPI.begin();          // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();   // Initialize MFRC522 Hardware

  Serial.println("RFID Started");
  Serial.println(F("-------------------"));
  Serial.println(F("Init Basic Array"));

  users[0].amount = 5;
  users[0].user = "Arno";
  users[0].userRfid = "4291fba";

  users[1].amount = 0;
  users[1].user = "Lorenz";
  users[1].userRfid = "fa80d116";

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH); 
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW); 
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 


  Serial.println(F("Everything is ready"));
  Serial.println(F("Waiting PICCs to be scanned"));
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop()
{
  if (mfrc522.PICC_IsNewCardPresent())
  {
    digitalWrite(5, LOW); 
    getID();
    String stringOne =  String(readCard[0], HEX)+String(readCard[1], HEX)+String(readCard[2], HEX)+String(readCard[3], HEX);
    Data& currentUser = getUser(stringOne);
    Serial.println(String(currentUser.user));
    if (currentUser.amount >= icePrice)
    {
      currentUser.buyIce();
      Serial.println(String(currentUser.amount));
      blinkLed(0);
    }
    else
    {
      blinkLed(2);
    }
    
    delay(1000);
    digitalWrite(5, HIGH); 
  }
}
