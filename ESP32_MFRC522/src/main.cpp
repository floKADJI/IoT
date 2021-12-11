#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         2          // Configurable, see typical pin layout above
#define SS_PIN          5         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

/**
 * Check firmware only once at startup
 */
void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);      // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 module
  
  Serial.println(F("*****************************"));
  Serial.println(F("MFRC522 Digital self test"));
  Serial.println(F("*****************************"));
  mfrc522.PCD_DumpVersionToSerial();  // Show version of PCD - MFRC522 Card Reader
  Serial.println(F("-----------------------------"));
  Serial.println(F("Only known versions supported"));
  Serial.println(F("-----------------------------"));
  Serial.println(F("Performing test..."));
  bool result = mfrc522.PCD_PerformSelfTest(); // perform the test
  Serial.println(F("-----------------------------"));
  Serial.print(F("Result: "));
  if (result)
    Serial.println(F("OK"));
  else
    Serial.println(F("DEFECT or UNKNOWN"));
  Serial.println();
}

void loop() {} // nothing to do
