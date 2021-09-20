#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

#include "DHT.h"

#include <WiFi.h> // Library for WiFi

// Setting your network credentials
const char *ssid = "AndroidAP";
const char *password = "ap_007KJ";
//const char *mqtt_server = "192.168.43.197";

boolean Wifi_flag = false; // Flag linked to connection to AP


// These pins are setting for connection with 1.44" TFT_LCD module.
#define TFT_CS         5
#define TFT_RST        4 
#define TFT_DC         2

#define DHTPIN 27     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

// Constant of color
#define BG_color ST77XX_BLACK
#define Text_color_1 ST77XX_GREEN
#define Text_color_2 ST77XX_YELLOW

// variables for time management
unsigned long now;
unsigned long current;
unsigned long last = 0;
const int INTERVAL = 2000;   // Time between data collection

unsigned long last_conn = 0;  // Time between reconnection to AP, if it's lost
const int INTERVAL_CONNECTION = 60000; // 1 min

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
  tft.fillScreen(BG_color);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Testing TFT_LCD !");
  delay(1000);
  tft.setCursor(0, 0);
  tft.setTextColor(BG_color);
  tft.setTextSize(1);
  tft.println("Testing TFT_LCD !");
  // tft.fillScreen(ST7735_BLACK);
}

void drawTable(){

  tft.setTextWrap(false);
  tft.fillScreen(BG_color);

  //  Table which displays data collect on AM2320
      tft.setTextColor(Text_color_1);
      tft.setCursor(0, 30);
      tft.setTextSize(1);
      tft.println("AM2320 sensor data ");
      tft.setCursor(0, 40);
      tft.setTextSize(2);
      tft.print("T");
    //  tft.setCursor(20, 40);
    //  tft.setTextSize(2);  tft.print(am_t); tft.println("°C");
      tft.setCursor(0, 60);
      tft.setTextSize(2);
      tft.print("RH");
    //  tft.setCursor(20, 60);
    //  tft.setTextSize(2);   tft.print(am_h); tft.println("%");  

  //  Table which displays data collect on DHT11
      tft.setTextColor(Text_color_2);
      tft.setCursor(0, 80);
      tft.setTextSize(1);
      tft.println("DHT11 sensor data ");
      tft.setTextSize(2);
      tft.setCursor(0, 90);
      tft.print("T");
    //  tft.setCursor(20, 90);
    //  tft.setTextSize(2);  tft.print(dht_t); tft.println("°C");
      tft.setCursor(0, 110);
      tft.setTextSize(2);
      tft.print("RH");
    //  tft.setCursor(20, 110);
    //  tft.setTextSize(2);   tft.print(dht_h); tft.println("%");
    
}

/*   Function AM2320_reading_data
  This function collects data from the AM2320 sensor and 
  displays them on the TFT_LCD and Serial_monitor
  Params: NULL
  Return: NULL
  Modified: am_t, am_h
  TODO: JUST COLLECT DATA, and call other function for display 
*/
void AM2320_reading_data(){
  
  //  Read relative humidity in percentage
  float h = am2320.readHumidity();
  //  Read temperature as Celsius (the default)
  float t = am2320.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from AM2320 sensor!"));
    return;
  }

  // Saved old_value is compared to new_value before display
    if( t != am_t || h != am_h ) {
      // Clean the old value to display the new on
      tft.setTextColor(BG_color);
      tft.setCursor(30, 40);
      tft.setTextSize(2);  tft.print(am_t); tft.println("Deg");
      tft.setCursor(30, 60);
      tft.setTextSize(2);  tft.print(am_h); tft.println("%");

      //  Display data collect on TFT_LCD for local checking
      tft.setTextColor(Text_color_1);
      tft.setCursor(30, 40);
      tft.setTextSize(2);  tft.print(t); tft.println("Deg");
      tft.setCursor(30, 60);
      tft.setTextSize(2);   tft.print(h); tft.println("%");

      // Save old value for next
      am_t = t;  
      am_h = h;
    }

  //  Send those data to Serial monitor for troubleshooting
  Serial.println("AM2320 sensor data collected: ");
  Serial.print("Temperature: ");  Serial.print(t);  Serial.println("°C");
  Serial.print("Humidity: ");     Serial.print(h);  Serial.println("%");
  Serial.println();

}

/*   Function DHT11_reading_data
  This function collects data from the DHT11 sensor and
  displays them on the TFT_LCD and Serial_monitor
  Params: NULL
  Return: NULL
  Modified: dht_t, dht_h
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
      tft.setTextColor(BG_color);
      tft.setCursor(30, 90);
      tft.setTextSize(2);   tft.print(dht_t); tft.println("Deg");
      tft.setCursor(30, 110);
      tft.setTextSize(2);   tft.print(dht_h); tft.println("%");

      //  Display data collect on TFT_LCD for local checking
      tft.setTextColor(Text_color_2);
      tft.setCursor(30, 90);
      tft.setTextSize(2);  tft.print(t); tft.println("Deg");
      tft.setCursor(30, 110);
      tft.setTextSize(2);   tft.print(h); tft.println("%");

      // Save old value for next
      dht_t = t;  
      dht_h = h;
    }

  Serial.println("DHT11 sensor data collected: ");
  Serial.print("Temperature: "); Serial.print(t); Serial.println("°C");
  Serial.print("Humidity: "); Serial.print(h); Serial.println("%");
  Serial.println();

}


/*  Function Wifi_setup()
  This function try to connect to AP, if available
  It use the global variable of connection credential
  Params: NULL
  Return: NULL

  TODO: ENABLE USER TO ENTER HIS OWN CREDENTIALS
*/
void wifi_setup(){
  current = millis();

  Serial.println();
  //  Initialisation of Connection to WiFi
  WiFi.begin(ssid, password);
  //  Serial.printf("Connection status: %d \n", WiFi.status());

  // TODO: Find best solution
  /* // Time for connection

  if (current - last_conn >= INTERVAL_CONNECTION) {
    last_conn = current;

    // Print local IP address & start ...
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

  }
  */

  if (WiFi.status() != WL_CONNECTED) {
    Wifi_flag = false;
    delay(1000);
    Serial.print(" . ");
    Serial.printf("Connection status: %d \n", WiFi.status());
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
}

void loop() {
  // put your main code here, to run repeatedly:

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

  // Wait a few seconds, then retry to connect at AP if connection loss
  if (now - last_conn >= INTERVAL_CONNECTION) {
    last_conn = now;

    // Check connection
    if (WiFi.status() != WL_CONNECTED) {
      wifi_setup();
      Serial.println(".");
    //  Serial.printf("Connection status: %d \n", WiFi.status());
    }

  }
  
}