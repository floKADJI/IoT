#include <Arduino.h>
#include <WiFi.h>

// Setting your network credentials to access your local network
const char *ssid = "IoT_Network";         // Home network ssid = "Yoomee-A1CE"
const char *password = "password";        // Home network pwd = "wifi_007KJ"
// There are global variable used to establish connection to AP

// Variables of time to manage the delay between differents operations
unsigned long now;
unsigned long last_conn = 0;
const int INTERVAL_CONNECTION = 60000; // 1 min


/**
 * Function to setup the WiFi connection to AP with network credentials given
 * Params: Null
 * Return: Null
 */
void wifi_setup(){
  Serial.println();
  //Initialisation of Connection to WiFi
  WiFi.begin(ssid, password);
  Serial.printf("Connection status: %d \n", WiFi.status());
  // TODO: Find best solution
  delay(1000); // Time for connection

  if (WiFi.status() == WL_CONNECTED) {
      // Print local IP address
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    //  Serial.printf("Connection status: %d \n", WiFi.status());
  }

  
}


void setup() {
  
  // Configure the LED_BUILTIN to ligth during the setup step to help debugging
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Just for checking, starting setup

  // Call wifi_setup() fonction for WIFI configuration
  wifi_setup();

  digitalWrite(LED_BUILTIN, LOW);   // Just for checking that we have done with Setup
}

void loop() {
  // put your main code here, to run repeatedly:
  // Process to chek if connection WiFi connection is loss, then try to reconnect
  if (now - last_conn >= INTERVAL_CONNECTION) {
    last_conn = now;

    // Check connection
    if (WiFi.status() != WL_CONNECTED) {
      wifi_setup();
      delay(1000);
      Serial.println(".");
    //  Serial.printf("Connection status: %d \n", WiFi.status());
    }

  }

}