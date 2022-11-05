#ifndef DisplayController_h
#define DisplayController_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_SSD1306.h>
#include "Images.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TIME_MODE 0
#define SLEEP_MODE 1
#define MUSIC_MODE 2

class DisplayController {
private:
  long deltaTime = 300;
  long lastTime;
  long sleepTimeCheck=0;
  int deltaSleepTime=200;
  int mode;
  int sleepPhase = 0;
public:
  void init();
  void begin();
  void operate();
  void displayMessage(char*);
  void displayTime(){};
  void displaySleep();
  void nextSong();
  void sleepMode(){};

  DisplayController() {
    lastTime = millis();
  };
};

void DisplayController::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
}

void DisplayController::init() {
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Ciao\n    Nadia"));
  display.display();
}

void DisplayController::operate() {
  if (CONTROL_DISPLAY == DISABLED) return;

  switch (mode){
    case TIME_MODE:
       displayTime();
       break;
    case SLEEP_MODE:
       displaySleep();
       break;
  }

}

void DisplayController::displayMessage(char* message) {
  //display.println(F(message));
}

void DisplayController::nextSong() {
  //display.println(F(message));
}


void DisplayController::displaySleep(){
  long time=millis();
  if (time - sleepTimeCheck > deltaSleepTime) {
    display.clearDisplay(); // Clear the display buffer
    
    switch (sleepPhase) {
      case 0:
        display.drawBitmap(0, 0, frame1, 128, 32, SSD1306_WHITE);
        deltaSleepTime=400;
        break;
      case 1:
        display.drawBitmap(0, 0, frame2, 128, 32, SSD1306_WHITE);
        deltaSleepTime=500;
        break;
      case 2:
        display.drawBitmap(0, 0, frame3, 128, 32, SSD1306_WHITE);
        deltaSleepTime=400;
        break;
      case 3:
        display.drawBitmap(0, 0, frame4, 128, 32, SSD1306_WHITE);
        deltaSleepTime=300;
        break;
      case 4:
      default:
        deltaSleepTime=400;
        display.drawBitmap(0, 0, frame5, 128, 32, SSD1306_WHITE);
        break;
    }
   
    sleepPhase++;
    sleepPhase%=5;
    display.display(); // Show the display buffer on the screen
    sleepTimeCheck = time;
    }

}


#endif