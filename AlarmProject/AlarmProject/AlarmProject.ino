#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include "FastLED.h"
#include <Keypad.h>

#else

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include "FastLED.h"
#include <Keypad.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
#endif


#define LED_PIN 11

#define SWITCH_PIN 10

CRGB leds[1];

int buzz=12; // Buzzer Pin



const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {2, 3, 4, 5};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int attempts = 0;

String USERS[] = {"airmonster", "bluekey"};
String UUIDS[] = {"F6 BD 3E F3","D0 F1 65 A3"};
int PASSW[2][5] = {{65,65,65,65,35},{61,61,61,61,35}};

void setup()
{
    Serial.begin(9600);
    Serial.println("NDEF Reader");
    nfc.begin();
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, 1);
    FastLED.setBrightness(30);
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    Serial.println("Please scan your badge on the reader...\n");
}


int keypadWork(int uuidPosition){
  Serial.println("Enter your 4 digit pin...\n");
  int password[5];

  
  
  for (int i = 0; i < 5; ++i){
    while((password[i] = keypad.getKey())==NO_KEY) {
      delay(1);
    } 
    
    while(keypad.getKey() != NO_KEY) {
      delay(1);
    }
    siren(1500); 

  }





  for (int i=0; i < 5; i++){
    if (password[i] != PASSW[uuidPosition][i]){
      return -1;
    }

  }


  return 0;
  
}



void siren(int alarmCode){

  if (alarmCode == 1500){
    tone(buzz, 1500, 50);
    return;
  }
  if (alarmCode == 200){
    tone(buzz, 200, 50);
    delay(100);
    tone(buzz, 200, 50);
    delay(100);
    tone(buzz, 200, 50);
    return;
  }
  if (alarmCode == 3000){
    tone(buzz, 1200, 50);
    delay(100);
    tone(buzz, 1500, 50);
    delay(100);
    tone(buzz, 1700, 50);
    return;
  }

  int count = 0;

  while (count < 10){
        
  for(int hz = 440; hz < 1000; hz+=25)
  {
    tone(buzz, hz, 50);
    delay(5);
  }
  for(int hz = 1000; hz > 440; hz-=25)
  {
    tone(buzz, hz, 50);
    delay(5);
  }
  count++;
  }
return;
}

int nfcWork(){


    NfcTag tag = nfc.read();
    Serial.println(tag.getTagType());
    Serial.print("UID: ");Serial.println(tag.getUidString());


    for (int x=0;x < sizeof(UUIDS) / sizeof(UUIDS[0]); x++){
      if (UUIDS[x] == tag.getUidString()){
        Serial.println("Hello, " + USERS[x] + "!\n");
        if (keypadWork(x) != 0){
        return -1;
     } 
    } 
   }
  
    
        
        leds[0] = CRGB(150, 0, 0);
        FastLED.show();
        siren(3000);
        Serial.println("Access Granted.");



    if (tag.hasNdefMessage()) // every tag won't have a message
    {

      NdefMessage message = tag.getNdefMessage();
      Serial.print("\nThis NFC Tag contains an NDEF Message with ");
      Serial.print(message.getRecordCount());
      Serial.print(" NDEF Record");
      if (message.getRecordCount() != 1) {
        Serial.print("s");
      }
      Serial.println(".");

      // cycle through the records, printing some info from each
      int recordCount = message.getRecordCount();
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print("\nNDEF Record ");Serial.println(i+1);
        NdefRecord record = message.getRecord(i);
        // NdefRecord record = message[i]; // alternate syntax

        Serial.print("  TNF: ");Serial.println(record.getTnf());
        Serial.print("  Type: ");Serial.println(record.getType()); // will be "" for TNF_EMPTY

        // The TNF and Type should be used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);

        // Print the Hex and Printable Characters
        Serial.print("  Payload (HEX): ");
        PrintHexChar(payload, payloadLength);

        // Force the data into a String (might work depending on the content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Serial.print("  Payload (as String): ");
        Serial.println(payloadAsString);

        // id is probably blank and will return ""
        String uid = record.getId();
        if (uid != "") {
          Serial.print("  ID: ");Serial.println(uid);
        }
      }
    }
  delay(1500);
  return 0;
}

 

void loop(){

  //C:\Users\x\Documents\Arduino\libraries\PN532\PN532.h
  //162 bool readPassiveTargetID(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength, uint16_t timeout = 10, bool inlist = false);
  //changed timeout from 1000 to 10 to allow both keypad and rfid to respond without delay

   leds[0] = CRGB(0, 0, 150);
   FastLED.show();



   if (nfc.tagPresent()){
      siren(1500);
      if (nfcWork() != 0){
        leds[0] = CRGB(0, 150, 0);
        FastLED.show();
        siren(200);
        attempts++;
        Serial.print("Authentication Failed. ");

        if (attempts > 2){
          Serial.println("Failed Authentication Limit Reached. Alarming\n");
          siren(100);
          attempts = 0;
         }

        Serial.println("Please Wait 5 Seconds to Re-Badge...\n");
        delay(5000);
      } else {
        attempts = 0;
      } 
   }


  
}
  
