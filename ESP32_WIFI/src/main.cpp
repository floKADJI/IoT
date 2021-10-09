#include <Arduino.h>
#include <WiFi.h>

// Setting your network credentials to access your local network
const char *ssid = "IoT_Network";         // Home network ssid = "Yoomee-A1CE"
const char *password = "password";        // Home network pwd = "wifi_007KJ"
// There are global variable used to establish connection to AP

void WiFiEvent(WiFiEvent_t event) {
  switch(event) 
  {
    case SYSTEM_EVENT_STA_GOT_IP:
    {
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      break;
    }
    case SYSTEM_EVENT_STA_DISCONNECTED:
    {
      Serial.println("WiFi lost connection");
      break;
    }
    case SYSTEM_EVENT_AP_STACONNECTED:
    {
      break;
    }
    default : break;
  }
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);

    // delete old config
    WiFi.disconnect(true);

    delay(1000);

    // Examples of different ways to register wifi events
    WiFi.onEvent(WiFiEvent);
    //WiFi.onEvent(WiFiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
    
    WiFi.begin(ssid, password);

    Serial.println("Wait for WiFi... ");
}

void loop()
{
    delay(1000);
}