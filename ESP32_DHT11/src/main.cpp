#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 2  // What pin we're connected to

#define DHTTYPE DHT11   // DHT11

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize the Serial interface
  Serial.begin(115200);
  Serial.println("DHTxx test!");

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  //  Reading Temperature and Humidity 

  //  Read Relative Humidity as pourcentage
  float h = dht.readHumidity();
  //  Read Temperature as Celsius
  float t = dht.readTemperature();

  //  Check if any reads failed and exit early (to try again)
  if( isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display on Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" % \t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
}