#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#include "DHT.h"

#include <WiFi.h>
#include <PubSubClient.h>
/*
// Setting your network credentials
const char *ssid = "AndroidAP";
const char *password = "ap_007KJ";
const char *mqtt_server = "192.168.43.197";
*/
// Setting your network credentials
const char *ssid = "Yoomee-A1CE";           // Home network ssid = "IoT_Network"
const char *password = "wifi_007KJ";        // Home network pwd = "password"
const char *mqtt_server = "192.168.1.175";    // Home network server = "192.168.7.1"

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);


// These pins are setting for connection with 1.44" TFT_LCD module.
#define TFT_CS         5
#define TFT_RST        4 
#define TFT_DC         2

#define DHTPIN 27     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11


// variables for time management
unsigned long now;
unsigned long last = 0;
const int INTERVAL = 2000;

// variables for temperature and humidity for comparation
float am_t = 00.00; // old value of temperature for AM2320
float am_h = 00.00; // old value of humidity for AM2320

float dht_t = 00.00;  // old value of temperature for DHT11
float dht_h = 00.00;  // old value of humidity for DHT11


// Initialize the 1.44" TFT_LCD.
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Initialize AM2320 sensor.
Adafruit_AM2320 am2320 = Adafruit_AM2320();

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);


// Just for testing the TFT_LCD
void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Testing TFT_LCD !");
  delay(1000);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.println("Testing TFT_LCD !");
  // tft.fillScreen(ST7735_BLACK);
}

// Just for drawing the table on the TFT_LCD
void drawTable(){

  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);

  //  Table which displays data collect on AM2320
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(0, 30);
      tft.setTextSize(1);
      tft.println("AM2320 sensor data ");
      tft.setCursor(0, 40);
      tft.print("T: ");
    //  tft.setCursor(20, 40);
    //  tft.setTextSize(2);  tft.print(am_t); tft.println("°C");
      tft.setCursor(0, 60);
      tft.setTextSize(1); tft.print("RH: ");
    //  tft.setCursor(20, 60);
    //  tft.setTextSize(2);   tft.print(am_h); tft.println("%");  

  //  Table which displays data collect on DHT11
      tft.setTextColor(ST77XX_YELLOW);
      tft.setCursor(0, 80);
      tft.setTextSize(1);
      tft.println("DHT11 sensor data ");
      tft.setCursor(0, 90);
      tft.print("T: ");
    //  tft.setCursor(20, 90);
    //  tft.setTextSize(2);  tft.print(dht_t); tft.println("°C");
      tft.setCursor(0, 110);
      tft.setTextSize(1); tft.print("RH: ");
    //  tft.setCursor(20, 110);
    //  tft.setTextSize(2);   tft.print(dht_h); tft.println("%");
    
}

/*   Function AM2320_reading_data
  This function collects data from the AM2320 sensor and 
  displays them on the TFT_LCD and Serial_monitor
  Params: NULL
  Return: NULL

  TODO: JUST COLLECT DATA, and call other function for display 
*/
void AM2320_reading_data(){
  
  //  Read relative humidity in percentage
  float h = am2320.readHumidity();
  //  Read temperature as Celsius (the default)
  float t = am2320.readTemperature();

  // Saved old_value is compared to new_value before display
    if( t != am_t || h != am_h ) {
      // Clean the old value to display the new on
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(20, 40);
      tft.setTextSize(2);  tft.print(t); tft.println("°C");
      tft.setCursor(20, 60);
      tft.setTextSize(2);   tft.print(am_h); tft.println("%");

      //  Display data collect on TFT_LCD for local checking
      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(20, 40);
      tft.setTextSize(2);  tft.print(t); tft.println("°C");
      tft.setCursor(20, 60);
      tft.setTextSize(2);   tft.print(h); tft.println("%");


      // Convert data in String format for transmission with MQTT protocol
      static char temperatureTemp[7];
      dtostrf(t, 6, 2, temperatureTemp);
      static char humidityTemp[7];
      dtostrf(h, 6, 2, humidityTemp);

      // Send data to Raspberry Pi for broker
      client.publish("megatec/sat1/AM2320/temperature", temperatureTemp);
      client.publish("megatec/sat1/AM2320/humidity", humidityTemp);

      // Save old value for next
      am_t = t;  
      am_h = h;
    }

  //  Send those data to Serial monitor for troubleshooting
  Serial.println("AM2320 sensor data collected: ");
  Serial.print("Temperature: ");  Serial.print(t);  Serial.println("°C");
  Serial.print("Humidity: ");     Serial.print(h);  Serial.println("%");
  Serial.println();


  // Save old value for next
  // am_t = t;  
  // am_h = h;
}

/*   Function DHT11_reading_data
  This function collects data from the DHT11 sensor and
  displays them on the TFT_LCD and Serial_monitor
  Params: NULL
  Return: NULL

  TODO: JUST COLLECT DATA, and call other function for display 
 */
void DHT11_reading_date(){
  //  Read relative humidity in percentage
  float h = dht.readHumidity();
  //  Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Saved old_value is compared to new_value before display
    if( t != dht_t || h != dht_h ) {
      // Clean the old value to display the new on
      tft.setTextColor(ST77XX_BLACK);
      tft.setCursor(20, 90);
      tft.setTextSize(2);   tft.print(dht_t); tft.println("°C");
      tft.setCursor(20, 110);
      tft.setTextSize(2);   tft.print(dht_h); tft.println("%");

      //  Display data collect on TFT_LCD for local checking
      tft.setTextColor(ST77XX_YELLOW);
      tft.setCursor(20, 90);
      tft.setTextSize(2);  tft.print(t); tft.println("°C");
      tft.setCursor(20, 110);
      tft.setTextSize(2);   tft.print(h); tft.println("%");


      // Convert data in String format for transmission with MQTT protocol
      static char temperatureTemp[7];
      dtostrf(t, 6, 2, temperatureTemp);
      static char humidityTemp[7];
      dtostrf(h, 6, 2, humidityTemp);

      // Send data to Raspberry Pi for broker
      client.publish("megatec/sat1/DHT11/temperature", temperatureTemp);
      client.publish("megatec/sat1/DHT11/humidity", humidityTemp);

      // Save old value for next
      dht_t = t;  
      dht_h = h;
    }

  Serial.println("DHT11 sensor data collected: ");
  Serial.print("Temperature: "); Serial.print(t); Serial.println("°C");
  Serial.print("Humidity: "); Serial.print(h); Serial.println("%");
  Serial.println();
  

  // Save old value for next
  // dht_t = t;  
  // dht_h = h;
}


void wifi_setup(){
  Serial.println();
  //Initialisation of Connection to WiFi
  WiFi.begin(ssid, password);
  //Serial.printf("Connection status: %d \n", WiFi.status());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(" . ");
    Serial.printf("Connection status: %d \n", WiFi.status());
  }

  // Print local IP address & start ...
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
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
  // put your setup code here, to run once:

  Serial.begin(115200); // Serial port configuration with Baud rate

  Serial.print(F("Hello! ST77xx TFT Test"));

  // use this initializer if using a 1.44" TFT:
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  Serial.println(F("Initialized"));

  //  tft print function!
  drawTable();
  delay(5000);

  //  AM2320 sensor setting up
  Serial.println("Adafruit AM2320 Basic Test");
  am2320.begin();

  //  DHT11 sensor setting up
  Serial.println(F("DHTxx test!"));
  dht.begin();

  // Setup WiFi configuration
  wifi_setup();
  // Setup MQTT configuration
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:

   if (!client.connected()) {
    reconnect();
  }
  client.loop();


  // Starting the timer;
  now = millis();

  // Wait a few seconds between measurements.
  if (now - last >= INTERVAL) {
     last = now;

     // Clean the screen for new display
     // tft.fillScreen(ST7735_BLACK);

     // Function that collect data from the AM2320 sensor.
     AM2320_reading_data();

     // Function that collect data from the DHT11 sensor.
     DHT11_reading_date();
  }
  
}