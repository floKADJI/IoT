#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Setting your network credentials to access your local network

// There are global variable used to establish connection to AP
const char *ssid = "megatec_AP";         // Home network ssid = "Yoomee-A1CE"
const char *password = "password";        // Home network pwd = "wifi_007KJ"

// There are global variable used to establish connection to Server
const char *mqtt_server = "192.168.4.1";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

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
  // Initialisation of Connection to WiFi
  WiFi.begin(ssid, password);
  // Serial.printf("Connection status: %d \n", WiFi.status());
  // TODO: Find best solution

  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(" . ");
      Serial.printf("Connection status: %d \n", WiFi.status());
  }

  // Print local IP address
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("Connection status: %d \n", WiFi.status());
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (client.connect("espClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //  client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("megatec/config");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  
  // Configure the LED_BUILTIN to ligth during the setup step to help debugging
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Just for checking, starting setup

  Serial.begin(115200);   // Serial Monitor Baud rate setup

  // Call wifi_setup() fonction for WIFI configuration
  wifi_setup();
  // Setup MQTT configuration
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  digitalWrite(LED_BUILTIN, LOW);   // Just for checking that we have done with Setup
}

void loop() {
  
  // Starting the timer;
  now = millis();
  
  //TODO: MANAGE ALL CONNECTION THERE

  // Process to chek if connection WiFi connection is loss, then try to reconnect
  if (now - last_conn >= INTERVAL_CONNECTION) {
    last_conn = now;
    /*
    // Check connection
    if (WiFi.status() != WL_CONNECTED) {
      wifi_setup();
      delay(1000);
      Serial.println(".");
    //  Serial.printf("Connection status: %d \n", WiFi.status());
    }
    */
    // Visual checking of transmission
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);

  }

}