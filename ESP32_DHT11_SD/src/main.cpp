#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "DHT.h"

#define DHTPIN 27     // Digital pin connected to the DHT sensor

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// variables
unsigned long now;
unsigned long last = 0;
const int INTERVAL = 5000;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Just to checking, starting setup

  Serial.begin(115200);

  Serial.println(F("DHTxx test!"));

  dht.begin();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin()) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // Just for checking that we are done with Setup
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  // make a string for assembling the data to log:
  String dataString = "";

  // Starting the timer;
  now = millis();

  // Wait a few seconds between measurements.
  if (now - last >= INTERVAL) {
    last = now;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Read humidity as percentage
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Save data on variable to send them on SD card
    dataString += String("T: ");
    dataString += String(t);
    dataString += String("°C \t");
    dataString += String("RH: ");
    dataString += String(h);
    dataString += String("%");

    /*
    // Display those data also via Serial Monitor
    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%"));
    Serial.println();
    */

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("/datalog.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);

    // Visual checking of Saving
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  /*
  // Visual checking of transmission
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  */
  }

}