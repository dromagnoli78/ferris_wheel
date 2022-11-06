#ifndef DisplayController_h
#define DisplayController_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#define SSD1306_NO_SPLASH 1
#include <Adafruit_SSD1306.h>
#include "Images.h"
#include "Songs.h"

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
#define MUSIC_MODE 1
#define SLEEP_MODE 2
#define PRE_SLEEP_MODE 3

class DisplayController {
private:
  long deltaTime = 300;
  long lastTime;
  long sleepTimeCheck=0; // Timer for sleepPhasesCheck
  int deltaSleepTime=200;
  int mode;
  int lastMode;
  int songIndex;
  int sleepPhase = 0;
  long songTitleTimeBeingShown = 0; // Timer for song title being shown initially (used to switch to clock after a while)
  bool isScrolling = false; 
  long sleepModeStartTime = 0; // When sleep Mode started
  long sleepModePeriod; // Timer for sleepMode interval to show the clock
  bool preSleep = false;
  SongGetter songGetter;
   
public:
  void init();
  void begin();
  void operate();
  void displayMessage(const char*);
  void displayTime(){};
  void displaySleep();
  void displayPreSleep();
  void displaySong(int index);
  void nextSong(int songIndex);
  void sleeping(bool isSleeping);

  DisplayController() {
    lastTime = millis();
    songGetter = SongGetter();
  };
};

void DisplayController::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
}

void DisplayController::init() {
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.setCursor(10, 0);
  display.println(F("Ciao\n    Nadia"));
  display.display();
}

void DisplayController::operate() {
  if (CONTROL_DISPLAY == DISABLED) return;

  long time = millis();
  if (time - lastTime > deltaTime) {
    switch (mode){
      case TIME_MODE:
        displayTime();
        break;
      case PRE_SLEEP_MODE:
        displayPreSleep();
        break;
      case SLEEP_MODE:
        displaySleep();
        break;
      case MUSIC_MODE:
        displaySong(songIndex);
        break;
    }
  }
}

void DisplayController::displaySong(int index) {
  if (!isScrolling)
    displayMessage(songGetter.getTrack(index));
  long time = millis();
  if (time - songTitleTimeBeingShown > 60000) {
    mode = TIME_MODE;
    isScrolling = false;
  }
}

  

void DisplayController::displayMessage(const char* message) {
  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.println(message);
  display.display();
  display.startscrollright(0x00, 0x0F);
  isScrolling = true;
}

void DisplayController::nextSong(int index) {
  if (CURRENT_MODE == DEBUG_MODE) Serial.println("DisplayController: Requested NextSong");
  mode = MUSIC_MODE;
  songIndex = index;
  songTitleTimeBeingShown = millis();
  isScrolling = false;
}


void DisplayController::displayPreSleep() {
  long time=millis();
  long deltaTime = time - sleepModeStartTime;
  if (preSleep && (deltaTime > 500 && deltaTime < 1000)) {
    display.clearDisplay(); 
    display.println(F("Buonanotte"));
    display.display();
    preSleep = false;
  }

  if (time - sleepModeStartTime > 5000) {
    mode = SLEEP_MODE;
    display.stopscroll();
    isScrolling = false;
  }
   
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

void DisplayController::sleeping(bool sleeping) {

  if (CURRENT_MODE == DEBUG_MODE) Serial.println("DisplayController: SleepMode triggered");
  if (sleeping) {
    lastMode = mode;
    mode = PRE_SLEEP_MODE;
    sleepModeStartTime = millis();
    isScrolling = false;
    preSleep = true;
  } else {
    mode = lastMode;
  }
}




#endif