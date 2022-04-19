#include <Arduino.h>


// The SPIFFS (FLASH filing system) is used to hold configuration file
#include "SPIFFS.h"
#include "FS.h"

// EEPROM added to save password type on the keypad from touch screen
#include <EEPROM.h>
#include <SPI.h>


// This is the file name used to store the configuration data
// The SPIFFS file name must start with "/".
#define CONFIG_FILE "/config.txt"

// Set REPEAT_CONFIG to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CONFIG false

void file_configure();

void setup() {
  // put your setup code here, to run once:
  // Use serial port
  Serial.begin(115200);

  //EEPROM.begin(eeprom_size);

  // Configuration stored in a File
  file_configure();
}

void loop() {
  // put your main code here, to run repeatedly:
}

//------------------------------------------------------------------------------------------

void file_configure()
{
//  uint16_t calData[5];
//  uint8_t calDataOK = 0;
  String content;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CONFIG_FILE)) {
    if (REPEAT_CONFIG)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CONFIG_FILE);
    }
    else
    {
      File f = SPIFFS.open(CONFIG_FILE, "r");
      if (f) {
        content = f.readString();

        Serial.print(content);
        /*
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
        */
      }
    }
  }
  /*
  if (calDataOK && !REPEAT_CONFIG) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CONFIG) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CONFIG to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CONFIG_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
  */
}

//------------------------------------------------------------------------------------------
